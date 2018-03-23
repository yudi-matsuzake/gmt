#pragma once

#include <gmt/polygon.hpp>
#include <gmt/algorithm/direction.hpp>
#include <gmt/algorithm/intersection.hpp>
#include <gmt/algorithm/misc.hpp>

namespace gmt {

bool point_in_polygon(const polygon2d& poly, const point2d& p)
{
	int wn = 0;

	switch(poly.size()){
	case 0:
		return false;
		break;
	case 1:
		if(poly[0] == p)
			return true;
		return false;
		break;
	}

	for(size_t i=0; i<poly.size(); i++){
		point2d a = poly[i];
		point2d b = poly[(i+1)%poly.size()];

		if(p == a || p == b)
			return true;

		if(is_collinear(a, b, p) && is_between(a, b, p))
			return true;

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

	return wn != 0;
}

}
