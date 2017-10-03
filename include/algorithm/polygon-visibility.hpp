#pragma once

#include <algorithm>
#include <vector>

#include "polygon.hpp"
#include "line.hpp"
#include "segment.hpp"

#include "algorithm/intersection.hpp"
#include "algorithm/misc.hpp"
#include "algorithm/distance.hpp"
#include "algorithm/direction.hpp"

namespace gmt {

/** Given `p` and a `poly_list`
  *
  */
template <typename T, std::size_t n_dimension>
polygon<T, n_dimension> polygon_visibility(
	const point<T, n_dimension>& p,
	const std::vector<polygon<T, n_dimension>>& poly_list)
{

	/**
	  * class used to compare the segments on the algorithm
	  */
	class segment_comparator {
		gmt::point<T, n_dimension> point_of_reference;
	public:
		segment_comparator(
			const gmt::point<T, n_dimension>& point_of_reference)
			: point_of_reference(point_of_reference)
		{}

		~segment_comparator()
		{}

		bool operator()(
			const gmt::segment<T, n_dimension>& s0,
			const gmt::segment<T, n_dimension>& s1)
		{
			gmt::point<T, n_dimension> p0 = s0.from;
			gmt::point<T, n_dimension> p1 = s1.from;

			bool p0_is_above = is_above(point_of_reference, p0);
			bool p1_is_above = is_above(point_of_reference, p1);

			if(p0_is_above && !p1_is_above)
				return true;

			if(!p0_is_above && p1_is_above)
				return false;

		}

	};
}

}
