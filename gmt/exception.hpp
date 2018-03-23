#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

#include <gmt/mat.hpp>
/* #include <gmt/line.hpp> */

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

/* template<typename T, size_t n_dimension> */
class line_does_not_intersect_segment : public exception {
public:
	/* line<T, n_dimension> l; */
	/* segment<T, n_dimension> s; */

	line_does_not_intersect_segment()
		/* const line<T, n_dimension>& l, */
		/* const segment<T, n_dimension>& s) */
		/* : exception(msg(l, s)), l(l), s(s) */
		:exception("The line does not intersect the segment")
	{}

	~line_does_not_intersect_segment()
	{}

	/* std::string msg() */
	/* 	/1* const line<T, n_dimension>& l, *1/ */
	/* 	/1* const segment<T, n_dimension>& s) *1/ */
	/* { */
	/* 	std::stringstream stream; */
	/* 	stream	<< "Line " << l << " does not intersect " */
	/* 		<< "segment " << s << "."; */
	/* 	return stream.str(); */
	/* } */

};

template<typename T, std::size_t l, std::size_t c>
class system_has_no_solution : public exception {

public:
	system_has_no_solution(const mat<T, l, c>& A, const mat<T, l, 1> B)
		: exception("Linear algebra error: System has no solution"),
		  A(A), B(B)
	{}

	virtual ~system_has_no_solution()
	{}

	mat<T, l, c> A;
	mat<T, l, 1> B;

};

}
