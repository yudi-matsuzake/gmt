#pragma once

#include <cstdlib>
#include <utility>

namespace gmt{

template<typename T, std::size_t l, std::size_t c>
class mat {
public:
	mat()
	{}

	virtual ~mat()
	{}

	std::size_t rows() const
	{
		return l;
	}

	std::size_t columns() const
	{
		return c;
	}

	void swap_rows(std::size_t i, std::size_t j)
	{
		std::swap(data[i], data[j]);
	}

	mat<T, l, c> copy() const
	{
		mat<T, l, c> m;

		for(size_t i=0; i<rows(); i++)
			for(size_t j=0; j<columns(); j++)
				m[i][j] = data[i][j];

		return m;
	}

	T* operator[](std::size_t i)
	{
		return data[i];
	}

	const T* operator[](std::size_t i) const
	{
		return data[i];
	}

	friend std::ostream& operator<<(std::ostream& o, const mat<T, l, c>& m)
	{
		for(std::size_t i=0; i<l; i++){
			o << '|';
			for(std::size_t j=0; j<c; j++)
				o << m[i][j] << '\t';
			o << '|' << std::endl;
		}

		return o;
	}

private:

	T data[l][c];
};

}
