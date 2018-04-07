#pragma once

#include <gmt/polygon.hpp>
#include <gmt/polygon-with-holes.hpp>
#include <gmt/algorithm/direction.hpp>
#include <gmt/algorithm/intersection.hpp>
#include <gmt/algorithm/misc.hpp>

namespace gmt {

typedef enum {
	INSIDE,
	OUTSIDE,
	ON_BONDARY
} side;

template<typename T>
side side_of(const T& poly, const point2d& p);

template<>
side side_of<polygon2d>(const polygon2d& poly, const point2d& p)
{
	int wn = 0;

	switch(poly.size()){
	case 0:
		return OUTSIDE;
		break;
	case 1:
		if(poly[0] == p)
			return INSIDE;
		return OUTSIDE;
		break;
	}

	for(size_t i=0; i<poly.size(); i++){
		point2d a = poly[i];
		point2d b = poly[(i+1)%poly.size()];

		if(p == a || p == b)
			return ON_BONDARY;

		if(is_collinear(a, b, p) && is_between(a, b, p))
			return ON_BONDARY;

		if(a.y() <= p.y()){
			if(b.y() > p.y()){
				gmt::direction d = direction_in(a, b, p);
				if(d == gmt::LEFT)
					wn++;
			}
		}else{
			if(b.y() <= p.y()){
				gmt::direction d = direction_in(a, b, p);
				if(d == gmt::RIGHT)
					wn--;
			}
				
		}

	}

	if(wn != 0)
		return INSIDE;

	return OUTSIDE;
}

/*
 * FIXME: (maybe this is reasonable?) this algorithm works with the
 * assumption that the holes are inside the boundary
 */
template<>
side side_of<polygon_with_holes2d>(const polygon_with_holes2d& poly, const point2d& p)
{
	side s = side_of<polygon2d>(poly, p);
	if(s != INSIDE)
		return s;

	for(const auto& hole : poly.holes()){
		s = side_of(hole, p);
		if(s == ON_BONDARY)
			return s;
		else if(s == INSIDE)
			return OUTSIDE;
	}

	return INSIDE;
}

template<typename T>
bool point_in_polygon(const T& poly, const point2d& p)
{
	return side_of(poly, p) != OUTSIDE;
}

}
