#pragma once

#include "point.hpp"

namespace gmt{

template <typename T, std::size_t n_dimension = 2>
class segment {
public:
	segment( const point<T, n_dimension>& from,
		 const point<T, n_dimension>& to)
		: from(from), to(to)
	{}

	virtual ~segment()
	{}


	point<T, n_dimension> from;
	point<T, n_dimension> to;
};

typedef segment<double, 2>	segment2d;
typedef segment<double, 3>	segment3d;
typedef segment<int, 2>		segment2i;
typedef segment<int, 3>		segment3i;


}
