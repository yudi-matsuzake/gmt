#pragma once

#include <list>
#include <vector>

#include "render.hpp"
#include "segment.hpp"
#include "polygon.hpp"
#include "line.hpp"
#include "algorithm/intersection.hpp"

namespace gmt {

class color {
public:
	color(double r = 0.0, double g = 0.0, double b = 0.0, double a = 0.0)
		: r(r), g(g), b(b), a(a)
	{}

	~color(){};

	inline color operator*(double s) const
	{
		return color(r*s, g*s, b*s, a);
	}

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

	template<typename G>
	void plot(const G& p, GLenum mode)
	{
		begin(mode);
		plot(p);
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

	template<typename G>
	void plot(const std::vector<G>& l)
	{
		for( const auto& p : l )
			plot(p);
	}

	template<typename G>
	void plot(const std::vector<G>& l, GLenum mode)
	{
		glBegin(mode);
		plot(l);
		glEnd();
	}

	void plot(const point<double, 2>& p)
	{
		glVertex2d(p.x(), p.y());
	}

	void plot(const segment<double, 2>& s)
	{
		plot(s.from);
		plot(s.to);
	}

	void plot(const polygon<double, 2>& poly)
	{
		for(const auto& p : poly)
			plot(p);
	}

	void plot(const std::vector<polygon2d>& l, GLenum mode)
	{
		glBegin(mode);
		for(const auto& p : l)
			plot(p, mode);
		glEnd();
	}

	void plot(const std::list<polygon2d>& l, GLenum mode)
	{
		glBegin(mode);
		for(const auto& p : l)
			plot(p, mode);
		glEnd();
	}

	/*
	 * TODO: get values from opengl viewport
	 */
	void plot(const line2d& line)
	{
		window_size winsiz = get_window_size();

		/*
		 * create top-left (tf), top-right (tr), bottom-left (bl)
		 * and bottom-right (br) points
		 */
		point2d tl = { 0.0, 0.0 };
		point2d tr = { static_cast<double>(winsiz.width), 0.0 };
		point2d bl = { 0.0, static_cast<double>(winsiz.height) };
		point2d br = {
			static_cast<double>(winsiz.width),
			static_cast<double>(winsiz.height)
		};

		/*
		 * top, bottom, left and right lines
		 */
		segment2d viewport[4] = {
			segment2d(tl, tr),
			segment2d(bl, br),
			segment2d(tl, bl),
			segment2d(tr, br)
		};

		point2d intersection;
		point2d last_intersection;

		glPointSize(5);
		size_t intersection_count = 0;
		try{
			intersection = point_of_intersection(line, viewport[0]);
			intersection_count++;
		}catch(line_does_not_intersect_segment& e){}

		for(int i=1; i<4; i++){
			last_intersection = intersection;

			try{
				intersection = point_of_intersection(
						line,
						viewport[i]);

				intersection_count++;
			}catch(line_does_not_intersect_segment& e){}

			if(intersection_count == 2
				&& last_intersection != intersection){
				break;
			}
		}

		if(intersection_count == 2){
			segment2d s(intersection, last_intersection);
			plot(s);
		}
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

	/*
	 * get mouse point based on gmt::point2d,
	 * limited by the window size
	 */
	gmt::point2d get_mouse_point()
	{
		gmt::mouse_position m = get_mouse_position();
		gmt::window_size w = get_window_size();

		m.y = w.height - m.y;

		m.x = (m.x < 0.0) ? 0.0 : m.x;
		m.y = (m.y < 0.0) ? 0.0 : m.y;

		m.x = (m.x > w.width) ? w.width : m.x;
		m.y = (m.y > w.height) ? w.height : m.y;

		return gmt::point2d{ m.x, m.y };
	}

	virtual void draw() = 0;

	static const class color white;
	static const class color black;
	static const class color red;
	static const class color green;
	static const class color blue;
	static const class color yellow;
	static const class color gray;
	static const class color grey;

};

const color plotter::white(1.0, 1.0, 1.0, 1.0);
const color plotter::black(0.0, 0.0, 0.0, 1.0);
const color plotter::red(1.0, 0.0, 0.0, 1.0);
const color plotter::green(0.0, 1.0, 0.0, 1.0);
const color plotter::blue(0.0, 0.0, 1.0, 1.0);
const color plotter::yellow(1.0, 1.0, 0.0, 1.0);
const color plotter::gray(0.5, 0.5, 0.5, 1.0);
const color plotter::grey(0.5, 0.5, 0.5, 1.0);


}
