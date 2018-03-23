#pragma once

#include <gmt/point.hpp>
#include <gmt/vec.hpp>

namespace gmt {

template<typename T>
bool is_equal(const T& a, const T& b, const T& e)
{
	return (e == 0.0 && a == b) || fabs(a - b) <= e;
}

/** calculates whether the point c is between
  * a and b
  */
template<typename T, size_t n_dimension>
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

}
