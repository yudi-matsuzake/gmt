#pragma once

#include "polygon.hpp"
#include "direction.hpp"
#include "line.hpp"
#include "algorithm/linear-algebra.hpp"

namespace gmt{ 

namespace intersection{
typedef enum type {
	NONE,
	PROPER,
	IMPROPER
} type;
}

/**
  * check if the vertex `a` and `b` of the polygon `poly` intersects
  * any segment of the polygon
  */
template<typename T, int n_dimension>
bool intersect_in_polygon(
	const polygon<T, n_dimension>& poly,
	std::size_t a,
	std::size_t b)
{

	segment<T, n_dimension> s0(poly[a], poly[b]);

	if(poly.size() >= 2){
		for(std::size_t i=0; i<poly.size(); i++){
			segment<T, n_dimension> s1(poly[i], poly[(i+1)%poly.size()]);

			if(intersect_proper(s0, s1))
				return true;

		}
	}

	return false;
}

template<typename T, std::size_t n_dimension>
bool intersect_proper(
	const segment<T, n_dimension>& s0,
	const segment<T, n_dimension>& s1,
	const T& e = 0)
{
	direction d_from = direction_in(s0, s1.from, e);
	direction d_to = direction_in(s0, s1.to, e);

	if(d_from == ON || d_to == ON)
		return false;

	if(d_from == d_to)
		return false;

	d_from = direction_in(s1, s0.from, e);
	d_to = direction_in(s1, s0.to, e);

	if(d_from == ON || d_to == ON)
		return false;

	if(d_from == d_to)
		return false;

	return true;
}

template<typename T, std::size_t n_dimension>
bool intersect_improper(
	const segment<T, n_dimension>& s0,
	const segment<T, n_dimension>& s1,
	const T& e = 0)
{
	direction d_from = direction_in(s0, s1.from, e);
	if(d_from == ON && (is_between<T, n_dimension>(s0.to, s0.from, s1.from)
			|| is_between<T, n_dimension>(s1.to, s1.from, s0.from)))
		return true;

	direction d_to = direction_in(s0, s1.to, e);
	if(d_to == ON && (is_between<T, n_dimension>(s0.to, s0.from, s1.to)
			|| is_between<T, n_dimension>(s1.to, s1.from, s0.to)))
		return true;

	return false;
}

/**
  * Checks whether two segments intersects. The intersection can be
  * proper, improper and none.
  *
  * @return	intersection::NONE in case the two segments not intersect
  * 		intersection::PROPER in case the two segments intersects
  * 		intersection::IMPROPER in case the two segments intersects
  * 		in the improper form
  */
template<typename T, std::size_t n_dimension>
intersection::type intersect(
	const segment<T, n_dimension>& s0,
	const segment<T, n_dimension>& s1,
	const T& e = 0.0)
{
	if(intersect_proper(s0, s1, e))
		return intersection::PROPER;
	else if(intersect_improper(s0, s1, e))
		return intersection::IMPROPER;
	
	return intersection::NONE;
}

template<typename T, std::size_t n_dimension>
intersection::type intersect(
	const line<T, n_dimension>& l,
	const segment<T, n_dimension>& seg)
try{
	line<T, n_dimension> line_segment(seg);
	mat<T, n_dimension, 1> x = resolve_scalars(l, line_segment);

	T s = x[1][0];

	if(s < 0.0 || s > 1.0)
		return intersection::NONE;

	if(s == 0.0 || s == 1.0)
		return intersection::IMPROPER;

	return intersection::PROPER;
}catch(system_has_no_solution<T, n_dimension, 2>& e){
	return intersection::NONE;
}


/*
 * resolve the scalars s and t where the lines l0 and l1
 * intersects where l0 = v0 + s*v1 and l1 = v2 + t*v3
 */
template<typename T, std::size_t n_dimension>
mat<T, n_dimension, 1> resolve_scalars(
	const line<T, n_dimension>& l0,
	const line<T, n_dimension>& l1)
{
	mat<T, n_dimension, 2> m;
	mat<T, n_dimension, 1> b;

	/*
	 * builds the coefficient matrix
	 */
	for(size_t i=0; i<m.rows(); i++){
		m[i][0] = l0.sense[i];
		m[i][1] = -(l1.sense[i]);
		b[i][0] = l1.shift[i] - l0.shift[i];
	}

	return resolve(m, b);
}

/*
 * finds the point of intersection between a line
 * and a segment
 */
template<typename T, std::size_t n_dimension>
point<T, n_dimension> point_of_intersection(
	const line<T, n_dimension>& l,
	const segment<T, n_dimension>& seg)
try{
	line<T, n_dimension> line_segment(seg);
	mat<T, n_dimension, 1> x = resolve_scalars(l, line_segment);

	T s = x[1][0];

	if(s < 0.0 || s > 1.0)
		throw line_does_not_intersect_segment();

	return l.point_on_line_at(x[0][0]);
}catch(system_has_no_solution<T, n_dimension, 2>& e){
	throw line_does_not_intersect_segment();
}

/*
 * finds the point of intersection between two lines
 */
template<typename T, std::size_t n_dimension>
point<T, n_dimension> point_of_intersection(
	const line<T, n_dimension>& l0,
	const line<T, n_dimension>& l1)
{
	mat<T, n_dimension, 1> x = resolve_scalars(l0, l1);

	return l0.point_on_line_at(x[0][0]);
}

}
