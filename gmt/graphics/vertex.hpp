#pragma once

#include <gmt/point.hpp>
#include <gmt/graphics/color.hpp>

namespace gmt {

template<typename T, size_t n_dimension>
class vertex : public point<T, n_dimension> {
public:
	gmt::color color;


	/** @brief Constructor of a `vertex`
	  */
	vertex()
	{}

	vertex(const point<T, n_dimension>& p)
		: point<T, n_dimension>(p)
	{}

	/** @brief constructs a vector with facilitator, the number of
	  *  arguments is the same of the dimensions, e.g., vertex(x, y, z, etc..)
	  *
	  */
	vertex(const std::initializer_list<T>& l)
		: point<T, n_dimension>(l)
	{}

	virtual ~vertex()
	{}

};

typedef vertex<double, 2> vertex2d;
typedef vertex<double, 3> vertex3d;
typedef vertex<int, 2> vertex2i;
typedef vertex<int, 3> vertex3i;
typedef vertex<float, 2> vertex2f;
typedef vertex<float, 3> vertex3f;

}
