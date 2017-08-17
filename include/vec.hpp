#pragma once

#include <cmath>
#include "point.hpp"
#include "exception.hpp"

namespace gmt {

/** @brief Class that represents a `n_dimension` linear vector with type `T`.
  *
  *
  * @template T			type of all axis of the vector
  * @template n_dimension	type of all axis of the vector
  */
template<typename T, std::size_t n_dimension = 2>
class vec : public point<T, n_dimension> {
public:

	/** @brief Constructor of a `vec`
	  */
	vec()
	{}

	vec(const point<T, n_dimension>& p0, const point<T, n_dimension>& p1)
	{
		point<T, n_dimension> p = p1 - p0;

		for(size_t i=0; i<p.ndim(); i++)
			this->axis[i] = p[i];

	}

	/** @brief constructs a vector with facilitator, the number of
	  *  arguments is the same of the dimensions, e.g., vec(x, y, z, etc..)
	  *
	  */
	vec(const std::initializer_list<T>& l)
		: point<T, n_dimension>(l)
	{}

	virtual ~vec()
	{}

	template<typename S>
	vec<T, n_dimension> scalar_product(const S scalar) const
	{
		for(std::size_t i=0; i<n_dimension; i++)
			this->at(i) *= scalar;
	}

	double dot_product(const vec<T, n_dimension>& v) const
	{
		double sum = 0.0;

		for(std::size_t i=0; i<n_dimension; i++)
			sum += this->at(i)*v[i];

		return sum;
	}

	double norm() const
	{
		double sum = 0.0;

		for(std::size_t i=0; i<n_dimension; i++){
			T n = this->at(i);
			sum += n*n;
		}

		return std::sqrt(sum);
	}

	double angle(const vec<T, n_dimension>& v) const
	{
		if(this->is_zero())
			throw exception("The vector must not be zero to calculate the angle");

		return std::acos(this->dot_product(v)/(this->norm()*v.norm()));
	}

	/*
	 * Operators --------------------------------------
	 */
};

typedef vec<double,	2> vec2d;
typedef vec<int,	2> vec2i;
typedef vec<float,	2> vec2f;
typedef vec<double,	3> vec3d;
typedef vec<int,	3> vec3i;
typedef vec<float,	3> vec3f;

}
