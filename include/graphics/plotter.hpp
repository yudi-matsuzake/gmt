#pragma once

#include <list>
#include <vector>

#include "render.hpp"
#include "segment.hpp"
#include "polygon.hpp"
#include "line.hpp"
#include "dcel/dcelp.hpp"
#include "algorithm/intersection.hpp"

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

class plotter : public ui_render {
private:
	/*
	 * auxiliar functions
	 */
	void plot_dcel_vertices(const gmt::dcel2d& d, GLenum mode) const
	{
		begin(mode);
		for(size_t i=0; i<d.n_vertex(); i++)
			plot(d.vertex_at(i));
		end();
	}

	void plot_dcel_edges(const gmt::dcel2d& d, GLenum mode) const
	{
		for(size_t i=0; i<d.n_edge(); i++){
			begin(mode);
			plot(d.edge_at(i));
			plot(d.edge_at(i));
			end();
		}
	}

	void plot_dcel_faces(const gmt::dcel2d& d, GLenum mode) const
	{
		for(size_t i=0; i<d.n_face(); i++){
			begin(mode);
			plot(d.face_at(i));
			plot(d.face_at(i));
			end();
		}
	}

public:

	typedef enum {
		VERTICES,
		EDGES,
		FACES
	} component;

	plotter( const std::string& title = "Plotter",
		int width = 800,
		int height = 600,
		GLFWmonitor* monitor = nullptr,
		GLFWwindow* share = nullptr)
		: ui_render(title, width, height, monitor, share)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
	}

	virtual ~plotter()
	{}

	void clear(GLbitfield mask = GL_COLOR_BUFFER_BIT) const
	{
		glClear(mask);
	}

	void clear_color(const color& c) const
	{
		clear_color(c.r(), c.g(), c.b(), c.a());
	}

	void clear_color(double r, double g, double b, double a = 1.0) const
	{
		glClearColor(r, g, b, a);
	}

	void color(const color& c) const
	{
		color(c.r(), c.g(), c.b(), c.a());
	}

	void color(double r, double g, double b, double a = 1.0) const
	{
		glColor4d(r, g, b, a);
	}

	void begin(GLenum mode) const
	{
		glBegin(mode);
	}

	void end() const
	{
		glEnd();
	}

	template<typename G>
	void plot(const G& p, GLenum mode) const
	{
		begin(mode);
		plot(p);
		end();
	}

	template<typename G>
	void plot(const std::list<G>& l) const
	{
		for( const auto& p : l )
			plot(p);
	}

	template<typename G>
	void plot(const std::list<G>& l, GLenum mode) const
	{
		glBegin(mode);
		plot(l);
		glEnd();
	}

	template<typename G>
	void plot(const std::vector<G>& l) const
	{
		for( const auto& p : l )
			plot(p);
	}

	template<typename G>
	void plot(const std::vector<G>& l, GLenum mode) const
	{
		glBegin(mode);
		plot(l);
		glEnd();
	}

	void plot(const point<double, 2>& p) const
	{
		glVertex2d(p.x(), p.y());
	}

	void plot(const segment<double, 2>& s) const
	{
		plot(s.from);
		plot(s.to);
	}

	void plot(const polygon<double, 2>& poly) const
	{
		for(const auto& p : poly)
			plot(p);
	}

	void plot(const std::vector<polygon2d>& l, GLenum mode) const
	{
		glBegin(mode);
		for(const auto& p : l)
			plot(p, mode);
		glEnd();
	}

	void plot(const std::list<polygon2d>& l, GLenum mode) const
	{
		glBegin(mode);
		for(const auto& p : l)
			plot(p, mode);
		glEnd();
	}

	/*
	 * TODO: get values from opengl viewport
	 */
	void plot(const line2d& line) const
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

	template <typename T>
	void plot(const dcelp_placeholder<T, double, 2>& placeholder) const
	{
		plot(placeholder.p);
	}

	/*
	 * dcel plots
	 */
	void plot(const typename gmt::dcel2d::vertex* v) const
	{
		plot(v->data.p);
	}

	void plot(const typename gmt::dcel2d::edge* e) const
	{
		plot(e->origin);
		plot(e->destination);
	}

	void plot(const typename gmt::dcel2d::face* f) const
	{
		if(f){
			typename gmt::dcel2d::edge* e = f->incident_edge;
			if(e){
				do{
					plot(e->origin);
					e = e->next;
				}while(e != f->incident_edge);
			}
		}
	}

	void plot(const typename gmt::dcel2d::face* f, GLenum mode) const
	{
		begin(mode);
		plot(f);
		end();
	}

	/*
	 * plot the dcel structure choosing a component
	 */
	void plot(
		const gmt::dcel2d& dcel,
		component c,
		GLenum mode) const
	{
		switch(c){
		case VERTICES:
			plot_dcel_vertices(dcel, mode);
			break;
		case EDGES:
			plot_dcel_edges(dcel, mode);
			break;
		case FACES:
			plot_dcel_faces(dcel, mode);
			break;
		}
	}

	virtual void reshape()
	{

		gmt::window_size s = get_window_size();

		glViewport(0, 0, s.width, s.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0, s.width, 0.0, s.height, -1.f, 1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}

	void renderize()
	{
		reshape();
		draw();
	}

	/*
	 * get mouse point based on gmt::point2d,
	 * limited by the window size
	 */
	virtual gmt::point2d get_mouse_point() const
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
