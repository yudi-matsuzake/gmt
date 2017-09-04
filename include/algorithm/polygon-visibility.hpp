#pragma once

#include <algorithm>
#include <vector>

#include "polygon.hpp"
#include "line.hpp"
#include "segment.hpp"

#include "algorithm/intersection.hpp"
#include "algorithm/misc.hpp"
#include "algorithm/distance.hpp"
#include "algorithm/direction.hpp"

namespace gmt {

template <typename T, std::size_t n_dimension>
class ray_comparator {
public:
	vec<T, n_dimension> x_axis()
	{
		vec<T, n_dimension> v;

		v.x() = 1;

		return v;
	}

	bool operator() (
		const vec<T, n_dimension>& v0,
		const vec<T, n_dimension>& v1)
	{
		if(v0 == 0.0)
			return true;
		if(v1 == 0.0)
			return false;

		vec<T, n_dimension> x = x_axis();

		double v0_angle = angle_to_left(x, v0);
		double v1_angle = angle_to_left(x, v1);
		return v0_angle < v1_angle;

		/* direction d = direction_in(v0, v1); */
		/* return d == LEFT; */
	}
};

template<typename T, std::size_t n_dimension>
bool ray_continues(
	const line<T, n_dimension>& l,
	const polygon<T, n_dimension>& poly,
	const size_t index)
{
	size_t next_index = (index+1)% poly.size();
	size_t prev_index = (index == 0)
				? poly.size()-1
				: index-1;

	segment<T, n_dimension> s(
		poly[next_index],
		poly[prev_index]
	);

	return intersect(l, s) != intersection::PROPER;
}

template <typename T, std::size_t n_dimension>
point<T, n_dimension> cast_ray(
	const point<T, n_dimension>& p,
	const std::vector<polygon<T, n_dimension>>& poly_list,
	size_t poly_index,
	size_t vertex_index)
{
	line<T, n_dimension> l(p, poly_list[poly_index][vertex_index]);

	T min = -1;
	point<T, n_dimension> closest;

	for(size_t i=0; i<poly_list.size(); i++){
		for(size_t j=1; j<=poly_list[i].size(); j++){
			size_t p0 = j - 1;
			size_t p1 = j%(poly_list[i].size());

			try{
				segment<T, n_dimension> seg(
					poly_list[i][p0],
					poly_list[i][p1]
				);

				point<T, n_dimension> intersection
					= point_of_intersection(l, seg);

				if(is_between<T, n_dimension>(
					poly_list[poly_index][vertex_index],
					intersection,
					p)){
					continue;
				}

				if(i == poly_index && (p0 == vertex_index
					|| p1 == vertex_index)
					&& ray_continues(
						l,
						poly_list[i],
						vertex_index)){
					continue;
				}

				T d = distance(p, intersection);
				if((min == -1 || d < min)){
					min = d;
					closest = intersection;
				}

			}catch(line_does_not_intersect_segment& e){}
		}
	}

	return closest;
}


template <typename T, std::size_t n_dimension>
polygon<T, n_dimension> polygon_visibility(
	const point<T, n_dimension>& p,
	const std::vector<polygon<T, n_dimension>>& poly_list)
{

	std::vector<vec<T, n_dimension>> rays;

	for(size_t i=0; i<poly_list.size(); i++){
		for(size_t j=0; j<poly_list[i].size(); j++){

			point<T, n_dimension> closest;
			closest = cast_ray(p, poly_list, i, j);

/* 			if(closest != poly_list[i][j] */
/* 				&& !is_between<T, n_dimension>(p, poly_list[i][j], closest)){ */

/* 				rays.push_back(poly_list[i][j] - p); */
/* 				rays.push_back(closest - p); */
/* 			}else{ */
/* 				rays.push_back(closest - p); */
/* 			} */

			rays.push_back(closest - p);

		}
	}

	std::stable_sort(rays.begin(), rays.end(), ray_comparator<T, n_dimension>());

	polygon<T, n_dimension> visibility;

	for(auto v : rays)
		visibility.push_back(p + v);

	return visibility;
}

}
