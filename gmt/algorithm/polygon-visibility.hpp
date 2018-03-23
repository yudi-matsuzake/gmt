#pragma once

#include <algorithm>
#include <vector>

#include <gmt/polygon.hpp>
#include <gmt/line.hpp>
#include <gmt/segment.hpp>

#include <gmt/algorithm/intersection.hpp>
#include <gmt/algorithm/misc.hpp>
#include <gmt/algorithm/distance.hpp>
#include <gmt/algorithm/direction.hpp>

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
		bool v0_above_xaxis = v0.x() >= 0.0;
		bool v1_above_xaxis = v1.x() >= 0.0;

		if(v0_above_xaxis == v1_above_xaxis){
			switch(direction_in(v0, v1, 1.0)){
			case RIGHT:
				return true;
				break;
			case LEFT:
				return false;
				break;
			case ON:
				return true;
				break;
			}
		}else if(v0_above_xaxis){
			return true;
		}

		return false;
	}
};

/*
 * returns 0 if the rays not continues
 * return 1 if the rays continue to the left
 * return 2 if the rays continue to the right
 */
template<typename T, std::size_t n_dimension>
int ray_continues(
	const point<T, n_dimension>& p,
	const polygon<T, n_dimension>& poly,
	const size_t index)
{
	size_t next_index = (index+1)% poly.size();
	size_t prev_index = (index == 0)
				? poly.size()-1
				: index-1;

	direction dnext = direction_in(
			p,
			poly[index],
			poly[next_index]
	);

	direction dprev = direction_in(
			p,
			poly[index],
			poly[prev_index]
	);

	if(dnext != dprev || dnext == ON)
		return 0;

	if(dnext == LEFT)
		return 1;
	return 2;

}

template <typename T, std::size_t n_dimension>
void cast_ray(
	const point<T, n_dimension>& p,
	const std::vector<polygon<T, n_dimension>>& poly_list,
	size_t poly_index,
	size_t vertex_index,
	std::vector<vec<T, n_dimension>>& rays)
{
	line<T, n_dimension> l(p, poly_list[poly_index][vertex_index]);

	T min = -1;
	point<T, n_dimension> closest;

	int ray_continuation = 0;

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
					|| p1 == vertex_index)){

					ray_continuation = ray_continues(
								p,
								poly_list[i],
								vertex_index
					);

					if(ray_continuation > 0)
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

	/* rays.push_back(closest - p); */
	if(ray_continuation == 0){
		rays.push_back(closest - p);
	}else{
		vec<T, n_dimension> c = closest - p;
		vec<T, n_dimension> v =
			poly_list[poly_index][vertex_index] - p;

		if(ray_continuation == 1){
			rays.push_back(c);
			if(v.norm() < c.norm())
				rays.push_back(v);
		}else{
			if(v.norm() < c.norm())
				rays.push_back(v);
			rays.push_back(c);
		}
	}
}


template <typename T, std::size_t n_dimension>
polygon<T, n_dimension> polygon_visibility(
	const point<T, n_dimension>& p,
	const std::vector<polygon<T, n_dimension>>& poly_list)
{

	std::vector<vec<T, n_dimension>> rays;

	for(size_t i=0; i<poly_list.size(); i++){
		for(size_t j=0; j<poly_list[i].size(); j++){
			cast_ray(p, poly_list, i, j, rays);
		}
	}

	std::stable_sort(rays.begin(), rays.end(), ray_comparator<T, n_dimension>());

	polygon<T, n_dimension> visibility;

	for(auto v : rays)
		visibility.push_back(p + v);

	return visibility;
}

}
