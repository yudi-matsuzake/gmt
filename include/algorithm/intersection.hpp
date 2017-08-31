#pragma once

#include "polygon.hpp"
#include "direction.hpp"

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

}
