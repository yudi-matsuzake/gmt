#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

namespace gmt {
template<typename T>

class matrix {
protected:
	T* raw_data;
public:

	class exception : public std::runtime_error {
	public:
		exception(const std::string& msg)
			: std::runtime_error(msg)
		{}

		~exception()
		{}
	};

	static matrix<T> identity(size_t n)
	{
		matrix<T> m(n, n);

		for(size_t i = 0; i<n ; i++){
			for(size_t j = 0; j<n ; j++){
				if(i == j)
					m[i][j] = 1;
				else
					m[i][j] = 0;
			}
		}

		return m;
	}

	const size_t n_row, n_col;

	explicit matrix(size_t n_row, size_t n_col)
		: n_row(n_row), n_col(n_col)
	{
		raw_data = new T[n_col*n_row];

		std::memset(raw_data, 0, n_col*n_row*sizeof(T));
	}

	virtual ~matrix()
	{
		delete[] raw_data;
	}

	matrix<T> transpose() const
	{
		matrix<T> m(n_col, n_row);

		for(size_t i = 0; i<m.n_row; i++)
			for(size_t j = 0; j<m.n_col ; j++)
				m[i][j] = (*this)[j][i];

		return m;
	}

	T* operator[](size_t index)
	{
		return &(raw_data[index*n_col]);
	}

	const T* operator[](size_t index) const
	{
		return &(raw_data[index*n_col]);
	}

	matrix<T> operator+(const matrix<T>& other) const
	{

		if(other.n_row != n_row || other.n_col != n_col)
			throw exception("operator+: the matrix must have the "
					"same size to perform this operation");

		matrix<T> m(n_row, n_col);
		for(size_t i=0; i<n_row; i++)
			for(size_t j=0; j<n_col; j++)
				m[i][j] = (*this)[i][j] + other[i][j];

		return m;
	}

	matrix<T> operator-(const matrix<T>& other) const
	{

		if(other.n_row != n_row || other.n_col != n_col)
			throw exception("operator+: the matrix must have the "
					"same size to perform this operation");

		matrix<T> m(n_row, n_col);
		for(size_t i=0; i<n_row; i++)
			for(size_t j=0; j<n_col; j++)
				m[i][j] = (*this)[i][j] - other[i][j];

		return m;
	}

	matrix<T> operator*(const matrix<T>& other) const
	{
		if(n_col != other.n_row)
			throw exception("operator*: the matrices must have the "
					"same number of columns and rows");
		matrix<T> m(n_row, other.n_col);

		for(size_t i = 0; i<n_row ; i++)
			for(size_t j = 0; j<other.n_col ; j++)
				for(size_t k = 0; k<n_col ; k++)
					m[i][j] += (*this)[i][k]*other[k][j];

		return m;
	}

	friend std::ostream& operator<<(std::ostream& o, const matrix<T>& m)
	{
		for(size_t i = 0; i<m.n_row ; i++){
			for(size_t j = 0; j<m.n_col ; j++){
				o << m[i][j];
				o << " ";
			}
			o << std::endl;
		}

		return o;
	}

};

}
