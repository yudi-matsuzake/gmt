#pragma once

#include "vec.hpp"

namespace gmt {

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

bool is_equal(double a, double b, double e)
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

}
