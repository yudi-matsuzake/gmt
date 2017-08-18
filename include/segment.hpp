#pragma once

#include "point.hpp"
#include "algorithm.hpp"

namespace gmt{

namespace intersection{

typedef enum type {
	NONE,
	PROPER,
	IMPROPER
} type;

}

template <typename T, int n_dimension = 2>
class segment {
public:
	segment( const point<T, n_dimension>& from,
		 const point<T, n_dimension>& to)
		: from(from), to(to)
	{}

	virtual ~segment()
	{}

	bool intersect_proper(
		const segment<T, n_dimension>& s,
		const T& e = 0.0) const
	{
		gmt::direction d_from = direction_in(*this, s.from, e);
		direction d_to = direction_in(*this, s.to, e);

		if(d_from == ON || d_to == ON)
			return false;

		if(d_from == d_to)
			return false;

		d_from = direction_in(s, from, e);
		d_to = direction_in(s, to, e);

		if(d_from == ON || d_to == ON)
			return false;

		if(d_from == d_to)
			return false;

		return true;
	}

	bool intersect_improper(
		const segment<T, n_dimension>& s,
		const T& e = 0.0) const
	{
		direction d_from = direction_in(*this, s.from, e);
		direction d_to = direction_in(*this, s.to, e);

		if(d_from == ON && is_between<T, n_dimension>(to, from, s.from))
			return true;
		else if(d_to == ON && is_between<T, n_dimension>(to, from, s.from))
			return true;

		return false;

	}

	intersection::type intersect(
		const segment<T, n_dimension>& s,
		const T& e = 0.0) const
	{
		if(intersect_proper(s, e))
			return intersection::PROPER;
		else if(intersect_improper(s, e))
			return intersection::IMPROPER;
		
		return intersection::NONE;
	}

	point<T, n_dimension> from;
	point<T, n_dimension> to;
};

typedef segment<double, 2>	segment2d;
typedef segment<double, 3>	segment3d;
typedef segment<int, 2>		segment2i;
typedef segment<int, 3>		segment3i;


}
