#pragma once

#include <list>
#include <vector>

#include "render.hpp"
#include "segment.hpp"

namespace gmt {

class color {
public:
	color(double r = 0.0, double g = 0.0, double b = 0.0, double a = 0.0)
		: r(r), g(g), b(b), a(a)
	{}

	~color(){};

	double r, g, b, a;
};

class plotter : public ui_render {
public:

	plotter( const std::string& title = "Plotter",
		int width = 800,
		int height = 600,
		GLFWmonitor* monitor = nullptr,
		GLFWwindow* share = nullptr)
		: ui_render(title, width, height, monitor, share)
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	~plotter()
	{}

	void clear(GLbitfield mask = GL_COLOR_BUFFER_BIT)
	{
		glClear(mask);
	}

	void clear_color(const color& c)
	{
		clear_color(c.r, c.g, c.b, c.a);
	}

	void clear_color(double r, double g, double b, double a = 1.0)
	{
		glClearColor(r, g, b, a);
	}

	void color(const color& c)
	{
		color(c.r, c.g, c.b, c.a);
	}

	void color(double r, double g, double b, double a = 1.0)
	{
		glColor4d(r, g, b, a);
	}

	void begin(GLenum mode)
	{
		glBegin(mode);
	}

	void end()
	{
		glEnd();
	}

	void plot(const point<double, 2>& p)
	{
		glVertex2d(p.x(), p.y());
	}

	void plot(const point<double, 2>& p, GLenum mode)
	{
		begin(mode);
		plot(p);
		end();
	}

	void plot(const segment<double, 2>& s)
	{
		plot(s.from);
		plot(s.to);
	}

	void plot(const segment<double, 2>& s, GLenum mode)
	{
		begin(mode);
		plot(s.from);
		plot(s.to);
		end();
	}

	template<typename G>
	void plot(const std::list<G>& l)
	{
		for( const auto& p : l )
			plot(p);
	}

	template<typename G>
	void plot(const std::list<G>& l, GLenum mode)
	{
		glBegin(mode);
		plot(l);
		glEnd();
	}

	void renderize()
	{
		gmt::window_size s = get_window_size();

		glViewport(0, 0, s.width, s.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, s.width, 0.0, s.height, -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw();
	}

	virtual void draw() = 0;

	static const class color white;
	static const class color black;
	static const class color red;
	static const class color green;
	static const class color blue;
	static const class color yellow;

};

const color plotter::white(1.0, 1.0, 1.0, 1.0);
const color plotter::black(0.0, 0.0, 0.0, 1.0);
const color plotter::red(1.0, 0.0, 0.0, 1.0);
const color plotter::green(0.0, 1.0, 0.0, 1.0);
const color plotter::blue(0.0, 0.0, 1.0, 1.0);
const color plotter::yellow(1.0, 1.0, 0.0, 1.0);


}
