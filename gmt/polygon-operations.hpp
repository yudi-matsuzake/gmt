#pragma once
#include <cstdlib>
#include <gmt/polygon.hpp>

namespace gmt {

polygon2d operator+(
	const polygon2d& a,
	const polygon2d& b)
{
	polygon2d r;
	r.resize(a.size());

	for(size_t i = 0; i<a.size() ; i++)
		r[i] = a[i] + b[i];

	return r;
}

polygon2d& operator+=(
	polygon2d& a,
	const polygon2d& b)
{
	for(size_t i = 0; i<a.size() ; i++)
		a[i] += b[i];

	return a;
}

polygon2d operator-(
	const polygon2d& a,
	const polygon2d& b)
{
	polygon2d r;
	r.resize(a.size());

	for(size_t i = 0; i<a.size() ; i++)
		r[i] = a[i] - b[i];

	return r;
}

polygon2d operator*(
	const polygon2d& a,
	const polygon2d& b)
{
	polygon2d r;
	r.resize(a.size());

	for(size_t i = 0; i<a.size() ; i++)
		for(size_t j = 0; j<a[i].ndim() ; j++)
			r[i][j] = a[i][j] * b[i][j];

	return r;
}

polygon2d operator*(
	const polygon2d& a,
	double b)
{
	polygon2d r;
	r.resize(a.size());

	for(size_t i = 0; i<a.size() ; i++)
		r[i] = a[i] * b;

	return r;
}


polygon2d operator/(
	const polygon2d& a,
	const polygon2d& b)
{
	polygon2d r;
	r.resize(a.size());

	for(size_t i = 0; i<a.size() ; i++)
		for(size_t j = 0; j<a[i].ndim(); j++)
			r[i][j] = a[i][j] / b[i][j];

	return r;
}

polygon2d operator/(
	const polygon2d& a,
	double b)
{
	polygon2d r;
	r.resize(a.size());

	for(size_t i = 0; i<a.size() ; i++)
		r[i] = a[i] / b;

	return r;
}

polygon2d& operator/=(
	polygon2d& a,
	double b)
{
	for(size_t i = 0; i<a.size() ; i++)
		a[i] /= b;

	return a;
}

polygon2d& operator*=(
	polygon2d& a,
	double b)
{
	for(size_t i = 0; i<a.size() ; i++)
		a[i] *= b;

	return a;
}

polygon2d operator-(const polygon2d& a)
{
	polygon2d r;
	r.resize(a.size());
	for(size_t i = 0; i<a.size() ; i++){
		r[i] = a[i]*(-1.0);
	}

	return r;
}

template<typename T, size_t n_dimension>
T polygon_norm(const gmt::polygon<T, n_dimension>& polygon)
{
	T sum = 0.0;
	for(size_t i = 0; i<polygon.size() ; i++)
		for(size_t j = 0; j<polygon[i].ndim() ; j++)
			sum += polygon[i][j]*polygon[i][j];

	return sqrt(sum);
}

double polygon_dot(
	const gmt::polygon2d& a,
	const gmt::polygon2d& b)
{
	double sum = 0.0;

	for(size_t i = 0; i<a.size() ; i++){
		for(size_t j = 0; j<a[i].ndim() ; j++){
			sum += a[i][j]*b[i][j];
		}
	}

	return sum;
}

};
