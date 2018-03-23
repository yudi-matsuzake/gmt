#pragma once

#include <gmt/point.hpp>
#include <gmt/vec.hpp>

namespace gmt {

/**
  * calculates the nearest point from a segment to a point
  */
template<typename T, std::size_t n_dimension>
point<T, n_dimension> nearest_point(
	const point<T, n_dimension>& p,
	const segment<T, n_dimension>& seg)
{
	gmt::vec<T, n_dimension> vseg(seg);
	double dot_product = vseg.dot_product(vec<T, n_dimension>(seg.from, p));

	double norm_squared = vseg.norm_squared();

	// s0 == s1
	if(norm_squared == 0)
		return seg.from;

	double h = dot_product/norm_squared;

	double nearest_projection = std::max(0.0, std::min(1.0, h));
	point<T, n_dimension> projection = seg.from + (vseg*nearest_projection);

	return projection;
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
	point<T, n_dimension> point_near = nearest_point(p, seg);
	return distance(p, point_near);
}

}
