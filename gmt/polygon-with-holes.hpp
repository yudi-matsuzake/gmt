#pragma once

#include <vector>
#include <gmt/polygon.hpp>

namespace gmt {

/**
  * class that represents a polygon with holes
  */
template<typename T, size_t n_dimension>
class polygon_with_holes : public polygon<T, n_dimension> {
protected:
	std::vector<polygon<T, n_dimension>> m_holes;
public:
	polygon_with_holes()
	{}

	~polygon_with_holes()
	{}

	void add_hole(const gmt::polygon<T, n_dimension>& p)
	{
		m_holes.push_back(p);
	}

	polygon<T, n_dimension>& hole(size_t i)
	{
		return m_holes.at(i);
	}

	const polygon<T, n_dimension>& hole(size_t i) const
	{
		return m_holes.at(i);
	}

	std::vector<polygon<T, n_dimension>>& holes()
	{
		return m_holes;
	}

	const std::vector<polygon<T, n_dimension>>& holes() const
	{
		return m_holes;
	}


};

typedef polygon_with_holes<double, 2>	polygon_with_holes2d;
typedef polygon_with_holes<double, 3>	polygon_with_holes3d;
typedef polygon_with_holes<int, 2>	polygon_with_holes2i;
typedef polygon_with_holes<int, 3>	polygon_with_holes3i;

}
