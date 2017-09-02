#pragma once

#include <algorithm>

#include "vec.hpp"
#include "polygon.hpp"

#include "algorithm/misc.hpp"
#include "algorithm/direction.hpp"
#include "algorithm/intersection.hpp"
#include "algorithm/distance.hpp"
#include "algorithm/linear-algebra.hpp"

namespace gmt {

/**
  * check if an vetex `i` of the polygon `poly` is
  * reflex
  * TODO: this function consider the polygon was created
  * in anti-clockwise; need to check that
  */
template<typename T, std::size_t n_dimension>
bool is_reflex(const polygon<T, n_dimension>& poly, std::size_t i)
{
	if(poly.size() < 3)
		return false;

	size_t next_index = (i + 1) % poly.size();
	size_t prev_index = (i == 0)
				? poly.size() - 1
				: i - 1;

	direction d = direction_in(poly[i], poly[prev_index], poly[next_index]);

	return d == LEFT;
}

/** Checks whether the vertex `index` of the polygon `poly` is
  * an ear
  *
  */
template<typename T, std::size_t n_dimension>
bool is_ear(const polygon<T, n_dimension>& poly, std::size_t index)
{
	if(poly.size() < 4)
		return false;

	if(is_reflex(poly, index))
		return false;

	size_t next_index = (index + 1) % poly.size();
	size_t prev_index = (index == 0)
				? poly.size() - 1
				: index - 1;

	for(	std::size_t i = (next_index + 1)%poly.size();
		i != prev_index;
		i = (i+1)%poly.size())
	{
		if(	direction_in(poly[prev_index], poly[index], poly[i])
				== LEFT
			&& direction_in(poly[index], poly[next_index], poly[i])
				== LEFT
			&& direction_in(poly[next_index], poly[prev_index], poly[i])
				== LEFT){
			return false;
		}
	}

	return true;
}

/** Checks whether the vertex `index` of the polygon `poly` is
  * a mouth
  *
  */
template<typename T, std::size_t n_dimension>
bool is_mouth(const polygon<T, n_dimension>& poly, std::size_t index)
{
	if(poly.size() < 4)
		return false;

	if(!is_reflex(poly, index))
		return false;

	size_t next_index = (index + 1) % poly.size();
	size_t prev_index = (index == 0)
				? poly.size() - 1
				: index - 1;

	for(	std::size_t i = (next_index + 1)%poly.size();
		i != prev_index;
		i = (i+1)%poly.size())
	{
		if(	direction_in(poly[prev_index], poly[index], poly[i])
				== RIGHT
			&& direction_in(poly[index], poly[next_index], poly[i])
				== RIGHT
			&& direction_in(poly[next_index], poly[prev_index], poly[i])
				== RIGHT){
			return false;
		}
	}

	return true;
}

}
