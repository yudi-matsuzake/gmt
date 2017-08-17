#pragma once

#include <cstdlib>
#include <cstring>

#include "exception.hpp"

namespace gmt {

/**
  * Describes a euclidean point of arbitrary number of dimension.
  */
template<typename T = double, std::size_t n_dimension = 2>
class point
{
public:

	/**
	  * TODO: Constructor of point.
	  * TODO: Elaborated Description.
	  */
	point(void)
	{
		for(size_t i=0; i<n_dimension; i++)
			axis[i] = 0;
	}

	/** @brief constructs a point with facilitator, the number of
	  *  arguments is the same of the dimensions, e.g., point(x, y, z, etc..)
	  */
	point(const std::initializer_list<T>& l)
	{

		if(l.size() != n_dimension)
			throw axis_out_of_bounds(n_dimension, l.size());

		size_t count = 0;
		for(auto& i : l)
			this->axis[count++] = i;
	}

	/**
	  * TODO: Desctructor of point.
	  * TODO: Elaborated Description
	  */
	virtual ~point ()
	{}

	/** @brief Get the number of dimensions of this point
	  *
	  * @return the constant number of dimension of this vec
	 */
	std::size_t ndim() const
	{
		return n_dimension;
	}

	/** @brief get element in the axis `axis`
	  */
	T& at(std::size_t index)
	{
		if(index < n_dimension)
			return this->axis[index];
		else
			throw axis_out_of_bounds(n_dimension, index);
	}

	const T& at(std::size_t index) const
	{
		if(index < n_dimension)
			return this->axis[index];
		else
			throw axis_out_of_bounds(n_dimension, index);
	}

	/** @brief reference to x
	  */
	T& x(void) { return at(0); }
	const T& x(void) const { return at(0); }

	/** @brief reference to y
	  */
	T& y(void) { return at(1); }
	const T& y(void) const { return at(1); }

	/** @brief reference to z
	  */
	T& z(void) { return at(2); }
	const T& z(void) const { return at(2); }

	/**
	  * @brief subtracts the `p` of this point and return the result
	  */
	point<T, n_dimension> subtraction(const point<T, n_dimension>& p) const
	{
		point<T, n_dimension> r;

		for(size_t i=0; i<n_dimension; i++)
			r[i] = at(i) - p[i];

		return r;
	}

	/**
	  * @brief subtracts the `p` of this point in place
	  */
	void subtract(const point<T, n_dimension>& p)
	{
		for(size_t i=0; i<n_dimension; i++)
			(*this)[i] -= p[i];
	}

	/**
	  * @brief adds the `p` of this point and return the result
	  */
	point<T, n_dimension> addition(const point<T, n_dimension>& p) const
	{
		point<T, n_dimension> r;

		for(size_t i=0; i<n_dimension; i++)
			r[i] = at(i) + p[i];

		return r;
	}

	/**
	  * @brief adds the `p` in this point
	  */
	void add(const point<T, n_dimension>& p)
	{
		for(size_t i=0; i<n_dimension; i++)
			(*this)[i] += p[i];
	}

	/** Check if the point contains only zeros.
	  *
	  * @return true in case the point is zero or false otherwise
	  */
	bool is_zero (void) const
	{
		for(std::size_t i=0; i<n_dimension; i++)
			if(at(i) != 0)
				return false;

		return true;
	}

	/*
	 * Operators --------------------------------------
	 */
	T& operator[](std::size_t axis)
	{
		return at(axis);
	}

	const T& operator[](std::size_t index) const
	{
		return at(index);
	}

	point<T, n_dimension> operator+(const point<T, n_dimension>& p) const
	{
		return this->addition(p);
	}

	point<T, n_dimension> operator+=(const point<T, n_dimension>& p)
	{
		return this->add(p);
	}

	point<T, n_dimension> operator-(const point<T, n_dimension>& p) const
	{
		return this->subtraction(p);
	}

	point<T, n_dimension> operator-=(const point<T, n_dimension>& p)
	{
		return this->subtract(p);
	}

protected:
	T axis[n_dimension];

};

typedef point<double,	2> point2d;
typedef point<int,	2> point2i;
typedef point<float,	2> point2f;
typedef point<double,	3> point3d;
typedef point<int,	3> point3i;
typedef point<float,	3> point3f;

}
