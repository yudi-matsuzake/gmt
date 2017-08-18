#pragma once

#include "vec.hpp"
#include "polygon.hpp"

namespace gmt {

template<typename T, int n_dimension>
class segment;

typedef enum direction {
	LEFT,
	RIGHT,
	ON
} direction;

template<typename T>
double signed_parallelogram_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return a.x()*b.y() - a.y()*b.x();
}

template<typename T>
double parallelogram_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return fabs(signed_parallelogram_area(a, b));
}

template<typename T>
double signed_triangle_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return signed_parallelogram_area(a, b)/2.0;
}

template<typename T>
double triangle_area(const vec<T, 2>& a, const vec<T, 2>& b)
{
	return parallelogram_area(a, b)/2.0;
}

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

			if(s0.intersect_proper(s1))
				return true;

		}
	}

	return false;
}

}
