#pragma once

#include <vector>

#include "point.hpp"
#include "vec.hpp"
#include "pi.hpp"

namespace gmt {

/**
  * simple polygon
  */
template<typename T, std::size_t n_dimension>
class polygon : public std::vector<point<T, n_dimension>> {
public:
	polygon()
	{}

	polygon(const std::initializer_list<point<T, n_dimension>>& l)
	{
		for(auto& i : l)
			this->push_back(i);
	}

	~polygon()
	{}

	friend std::ostream& operator<<(std::ostream& o, const polygon& poly)
	{
		o << "[";
		if(poly.size()){
			for(size_t i=0; i<poly.size() - 1; i++)
				o << poly[i] << ", ";
			o << poly[poly.size() - 1];
		}
		o << "]";
		return o;
	}

};

typedef polygon<double, 2>	polygon2d;
typedef polygon<double, 3>	polygon3d;
typedef polygon<int, 2>		polygon2i;
typedef polygon<int, 3>		polygon3i;

}
