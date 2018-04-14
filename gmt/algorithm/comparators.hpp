#pragma once

#include <gmt/point.hpp>

namespace gmt {

/*
 * class to compare axes. it's used to sort and set of
 * points by the abscissa values for instance
 */
class axis_comparator {
protected:
	size_t axis;
	bool asc;
	bool resolves_tie;
public:

	/*
	 * axis comparator constructor.
	 *
	 * @param axis		is the axis to be compared
	 * @param asc		if true compare in ascendent or decresent
	 *			otherwise
	 * @param resolve_tie	if true try to resolve tie with other axes
	 *
	 */
	axis_comparator(
		size_t axis = 0,
		bool asc = true,
		bool resolves_tie = true)
		: axis(axis), asc(asc), resolves_tie(resolves_tie)
	{}

	~axis_comparator()
	{}

	template<typename T>
	bool operator()(const T& a, const T& b)
	{
		return asc
			?(a[axis] < b[axis] ||
				(a[axis] == b[axis] && a[axis+1] < b[axis+1]))
			:(b[axis] < a[axis]);
	}

};
}
