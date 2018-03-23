#pragma once

#include <gmt/vec.hpp>
#include <gmt/point.hpp>

namespace gmt {

template<typename T, std::size_t n_dimension>
class line {
public:
	vec<T, n_dimension> sense;
	vec<T, n_dimension> shift;

	line()
	{}

	line(	const point<T, n_dimension>& p0,
		const point<T, n_dimension>& p1)
		: sense(p1 - p0), shift(p0)
	{}

	line(	const segment<T, n_dimension>& seg)
		: line(seg.from, seg.to)
	{}

	line(	const vec<T, n_dimension>& v)
		: line(point<T, n_dimension>(), v)
	{}

	virtual ~line()
	{}

	point<T, n_dimension> point_on_line_at(
		const T& scalar) const
	{
		return shift + sense*scalar;
	}

	std::ostream& print(
		std::ostream& o,
		const std::string& scalar = "t") const
	{
		o << shift << " + " << scalar << sense;

		return o;
	}

	friend std::ostream& operator<<(
		std::ostream& o,
		const line<T, n_dimension>& l)
	{
		return l.print(o);
	}


};

typedef line<double, 2>	line2d;
typedef line<double, 3>	line3d;
typedef line<float, 2>	line2f;
typedef line<float, 3>	line3f;
typedef line<int, 2>	line2i;
typedef line<int, 3>	line3i;

}
