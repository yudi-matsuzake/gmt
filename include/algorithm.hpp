#pragma once

#include "vec.hpp"
#include "polygon.hpp"
#include "direction.hpp"
#include "intersection.hpp"

namespace gmt {

/**
  * calculates the signed area of the parallelogram. The signed area
  * is useful to calculate the "left" computational geometry directive.
  *
  * @see parallelogram_area
  */
template<typename T>
double signed_parallelogram_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return a.x()*b.y() - a.y()*b.x();
}

/**
  * calculates the area of the parallelogram.
  *
  * @see signed_parallelogram_area
  */
template<typename T>
double parallelogram_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return fabs(signed_parallelogram_area(a, b));
}

/**
  * calculates the signed area of the triangle. The signed area
  * is useful to calculate the "left" computational geometry directive.
  *
  * @see triangle_area
  */
template<typename T>
double signed_triangle_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return signed_parallelogram_area(a, b)/2.0;
}

/**
  * calculates the area of the triangle.
  *
  * @see signed_triangle_area
  */
template<typename T>
double triangle_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return parallelogram_area(a, b)/2.0;
}

/**
  * calculates the area of a triangle.
  */
template<typename T>
double triangle_area(
	const point<T, 2>& a,
	const point<T, 2>& b,
	const point<T, 2>& c)
{
	return triangle_area(vec<T, 2>(a, b), vec<T, 2>(a, c));
}

template<typename T>
bool is_equal(const T& a, const T& b, const T& e)
{
	return (e == 0.0 && a == b) || fabs(a - b) <= e;
}

/** Calculates the direction of point `p2` in reference
  * of segment of `p0` and `p1`
  *
  */
template<typename T>
direction direction_in(
	const point<T, 2>& p0,
	const point<T, 2>& p1,
	const point<T, 2>& p2,
	double e = 0.0)
{
	double tmp = signed_parallelogram_area(
			vec<T, 2>(p0, p1),
			vec<T, 2>(p0, p2));

	if(is_equal(tmp, 0.0, e))
		return ON;
	else if(tmp < 0.0)
		return RIGHT;
	else
		return LEFT;
}

template<typename T>
direction direction_in(
	const segment<T, 2>& s,
	const point<T, 2>& p,
	const T& e = 0.0)
{
	double tmp = signed_parallelogram_area(
			vec<T, 2>(s.from, s.to),
			vec<T, 2>(s.from, p));

	if(is_equal(tmp, 0.0, e))
		return ON;
	else if(tmp < 0.0)
		return RIGHT;
	else
		return LEFT;
}

template<typename T, int n_dimension>
bool is_between(
	const point<T, n_dimension>& a, 
	const point<T, n_dimension>& b, 
	const point<T, n_dimension>& c)
{
	if(a == b) return a == c;

	/*
	 * must find some different dimension to compare the
	 * betweenness
	 */
	bool found_diff_dim = false;
	for(std::size_t i=0; !found_diff_dim && i<n_dimension; i++){
		if(a[i] != b[i]){
			found_diff_dim = true;

			/*
			 * check betweenness
			 */
			return (c[i] >= a[i] && c[i] <= b[i])
				|| (c[i] <= a[i] && c[i] >= b[i]);
		}
	}

	return false;
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

/**
* calculates the distance of two points
*/
template<typename T, std::size_t n_dimension>
double distance(
const point<T, n_dimension>& p0,
const point<T, n_dimension>& p1)
{
	vec<T, n_dimension> v(p0, p1);
	return v.norm();
}

/**
  * calculates the distance of one point and a segment
  */
template<typename T, std::size_t n_dimension>
double distance(
	const point<T, n_dimension>& p,
	const segment<T, n_dimension>& seg)
{

	double a = parallelogram_area(
			vec<T, n_dimension>(seg.from, p),
			vec<T, n_dimension>(seg));

	return a/seg.norm();
}

}
