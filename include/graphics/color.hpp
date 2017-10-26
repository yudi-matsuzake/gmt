#pragma once

#include "point.hpp"

namespace gmt {

class color : public gmt::point4d {
public:

	color(const gmt::point4d& p)
	{
		at(0) = p[0];
		at(1) = p[1];
		at(2) = p[2];
		at(3) = p[3];
	}

	color(double r = 0.0, double g = 0.0, double b = 0.0, double a = 0.0)
	{
		at(0) = r;
		at(1) = g;
		at(2) = b;
		at(3) = a;
	}

	~color(){};

	inline double r() const
	{
		return at(0);
	}

	inline double g() const
	{
		return at(1);
	}

	inline double b() const
	{
		return at(2);
	}

	inline double a() const
	{
		return at(3);
	}


	inline double r()
	{
		return at(0);
	}

	inline double g()
	{
		return at(1);
	}

	inline double b()
	{
		return at(2);
	}

	inline double a()
	{
		return at(3);
	}

};

}
