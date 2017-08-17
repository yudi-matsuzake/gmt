#pragma once

#include <stdexcept>
#include <string>

namespace gmt {

class exception : public std::runtime_error {

public:
	exception(const std::string& msg)
		: std::runtime_error(msg)
	{
	}

	virtual ~exception()
	{
	}

};

class axis_out_of_bounds : public exception {
public:

	axis_out_of_bounds(int max_index, int outofbounds_index)
		: exception("Axis out of bounds"),
		 max_index(max_index),
		 outofbounds_index(outofbounds_index)
	{
	}

	~axis_out_of_bounds()
	{
	}

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

}
