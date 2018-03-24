#pragma once

#include <gmt/point.hpp>

namespace gmt {
class axis_comparator {
protected:
	size_t axis;
	bool asc;

public:
	axis_comparator(size_t axis = 0, bool asc = true)
		: axis(axis), asc(asc)
	{}

	~axis_comparator()
	{}

	template<typename T>
	bool operator()(const T& a, const T& b)
	{
		return asc
			?(a[axis] < b[axis])
			:(b[axis] < a[axis]);
	}

};
}
