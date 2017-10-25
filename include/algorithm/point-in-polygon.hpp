#pragma once

#include "polygon.hpp"
#include "direction.hpp"
#include "algorithm/intersection.hpp"

namespace gmt {

bool is_upward_edge(
	const point2d& p,
	const point2d& a,
	const point2d& b,
	direction d)
{
	if(b.y() < p.y() || a.y() > p.y())
		return false;

	return d == LEFT;
}

bool is_downward_edge(
	const point2d& p,
	const point2d& a,
	const point2d& b,
	direction d)
{
	if(a.y() < p.y() || b.y() > p.y())
		return false;

	return d == RIGHT;
}


bool point_in_polygon(const polygon2d& poly, const point2d& p)
{
	int wn = 0;
	for(size_t i=0; i<poly.size(); i++){
		point2d a = poly[i];
		point2d b = poly[(i+1)%poly.size()];

		if(is_collinear(a, b, p) || p == a)
			return true;

		if(a.x() >= p.x() || b.x() >= p.x()){
			if(p == a || p == b){
				wn++;
			}else{
				direction d = direction_in(a, b, p);

				if(is_upward_edge(p, a, b, d))
					wn++;
				else if(is_downward_edge(p, a, b, d))
					wn--;
			}
		}

	}

	return wn != 0;
}

}
