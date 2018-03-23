#pragma once

#include <cmath>
#include <gmt/point.hpp>
#include <gmt/segment.hpp>
#include <gmt/exception.hpp>

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

	vec(const point<T, n_dimension>& p)
		: point<T, n_dimension>(p)
	{}

	vec(const point<T, n_dimension>& p0, const point<T, n_dimension>& p1)
	{

		for(size_t i=0; i<n_dimension; i++)
			this->axis[i] = p1[i] - p0[i];

	}

	vec(const segment<T, n_dimension>& seg)
		: vec(seg.from, seg.to)
	{}


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

	double norm_squared() const
	{
		double sum = 0.0;

		for(std::size_t i=0; i<n_dimension; i++){
			T n = this->at(i);
			sum += n*n;
		}

		return sum;
	}

	double norm() const
	{
		return std::sqrt(norm_squared());
	}

	double angle(const vec<T, n_dimension>& v) const
	{
		if(this->is_zero())
			throw exception("The vector must not be zero to calculate the angle");

		return std::acos(this->dot_product(v)/(this->norm()*v.norm()));
	}

	double oriented_angle(void) const
	{
		vec<T, n_dimension> v;
		v.x() = 1.0;

		double m = 1.0;
		if(this->y() < 0.0)
			m = -1.0;
			
		return m*angle(v);
	}

	vec<T, n_dimension> escalation(const T& scale) const
	{
		return multiplication(scale);
	}

	vec<T, n_dimension>& scale(const T& scale) const
	{
		multiply(scale);
		return *this;
	}

	vec<T, n_dimension> versor() const
	{
		return this->division(this->norm());
	}

	vec<T, n_dimension> rotation(double theta) const
	{
		if(n_dimension != 2)
			throw std::runtime_error(
				std::string("rotate: Not implemented for ")
				+ std::to_string(n_dimension)
				+ " dimensions!");

		T new_x = cos(theta)*this->x() - sin(theta)*this->y();
		T new_y = sin(theta)*this->x() + cos(theta)*this->y();

		return vec<T, n_dimension>{ new_x, new_y };
	}

	void rotate(double theta)
	{
		*(this) = rotation(theta);
	}


};

typedef vec<double,	2> vec2d;
typedef vec<int,	2> vec2i;
typedef vec<float,	2> vec2f;
typedef vec<double,	3> vec3d;
typedef vec<int,	3> vec3i;
typedef vec<float,	3> vec3f;

}
