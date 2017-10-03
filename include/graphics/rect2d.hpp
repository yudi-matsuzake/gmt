#pragma once
#include "point.hpp"

namespace gmt {

/*
 * simple representation of a rect
 */
typedef struct rect2d {
	gmt::point2d pos;
	double width, height;
} rect2d;

}
