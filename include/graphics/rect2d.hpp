#pragma once

#include "point.hpp"

namespace gmt {

/*
 * simple representation of a rect
 */
typedef struct rect2d {
	gmt::point2d pos;
	double width, height;

	friend std::ostream& operator<<(std::ostream& o, const gmt::rect2d& r)
	{
		o << "(pos: " << r.pos
		  << ", width: " << r.width
		  << ", height: " << r.height
		  << ")";
		return o;
	}
} rect2d;

}
