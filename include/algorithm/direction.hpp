#pragma once

#include "vec.hpp"
#include "misc.hpp"
#include "pi.hpp"

namespace gmt {

typedef enum direction {
	LEFT,
	RIGHT,
	ON
} direction;

/** Calculates the direction of point `p2` in reference
  * of segment of `p0` and `p1`
  *
  */
template<typename T>
direction direction_in(
	const vec<T, 2>& v0,
	const vec<T, 2>& v1,
	double e = 0.0)
{
	double tmp = signed_parallelogram_area(v0, v1);

	if(is_equal(tmp, 0.0, e))
		return ON;
	else if(tmp < 0.0)
		return RIGHT;
	else
		return LEFT;
}

template<typename T>
direction direction_in(
	const point<T, 2>& p0,
	const point<T, 2>& p1,
	const point<T, 2>& p2,
	double e = 0.0)
{
	return direction_in(vec<T, 2>(p0, p1), vec<T, 2>(p0, p2), e);
}

template<typename T>
direction direction_in(
	const segment<T, 2>& s,
	const point<T, 2>& p,
	const T& e = 0.0)
{
	return direction_in(s.from, s.to, p, e);
}

/**
  * calculates the angle of two vectors going to left
  */
template<typename T, std::size_t n_dimension>
double angle_to_left(
	const vec<T, n_dimension>& v0,
	const vec<T, n_dimension>& v1)
{
	double minor_angle = v0.angle(v1);

	if(direction_in(v0, v1) == RIGHT)
		return 2*pi - minor_angle;

	return minor_angle;
}

}
