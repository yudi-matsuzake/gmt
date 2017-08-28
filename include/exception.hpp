#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace gmt {

class exception : public std::runtime_error {

public:
	exception(const std::string& msg)
		: std::runtime_error(msg)
	{}

	virtual ~exception()
	{}

};

class axis_out_of_bounds : public exception {
public:

	axis_out_of_bounds(int max_index, int outofbounds_index)
		: exception("Axis out of bounds"),
		 max_index(max_index),
		 outofbounds_index(outofbounds_index)
	{}

	~axis_out_of_bounds()
	{}

	int max_index, outofbounds_index;
};

class glfw_error : public exception {
public:
	glfw_error(int code, const char* msg)
		: exception(msg),
		  code(code)
	{}

	glfw_error(const char* msg)
		: exception(msg),
		  code(-1)
	{}

	int code;
};

class glfw_unknown_key_name : public glfw_error
{
public:
	glfw_unknown_key_name()
		: glfw_error("Unknown key name")
	{}
};

class poly_hasnt_enough_vertex : public exception {
public:
	size_t has, need;

	poly_hasnt_enough_vertex(size_t has, size_t need)
	 : exception(get_msg(has, need)), has(has), need(need)
	{}

	~poly_hasnt_enough_vertex()
	{}

	static std::string get_msg(size_t has, size_t need)
	{
		std::ostringstream s;
		s << "Poly has not enough vertices: has "
		  << has
		  << " and the method needed at least "
		  << need
		  << ".";

		return s.str();
	}
};

}
