#pragma once

#include <gmt/point.hpp>

namespace gmt{

template <typename T, std::size_t n_dimension = 2>
class segment {
public:
	point<T, n_dimension> from;
	point<T, n_dimension> to;

	segment( const point<T, n_dimension>& from,
		 const point<T, n_dimension>& to)
		: from(from), to(to)
	{}

	virtual ~segment()
	{}

	friend std::ostream& operator<<(
		std::ostream& o,
		const segment<T, n_dimension>& seg)
	{
		o << seg.from << " -> " << seg.to;

		return o;
	}

};

typedef segment<double, 2>	segment2d;
typedef segment<double, 3>	segment3d;
typedef segment<int, 2>		segment2i;
typedef segment<int, 3>		segment3i;


}
