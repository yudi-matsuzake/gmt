#pragma once

#include "point.hpp"
#include "color.hpp"

namespace gmt {

template<typename T, size_t n_dimention>
class vertex : public point<T, n_dimention> {
public:
	gmt::color color;
};

}
