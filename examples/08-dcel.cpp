#include <iostream>
#include <cmath>

#include <gmt/graphics/ui_component/exit_component.hpp>
#include <gmt/graphics/ui_component/dcel_component.hpp>
#include <gmt/graphics/ui_component/zoom_component.hpp>
#include <gmt/graphics/ui_component/pan_component.hpp>

class dcel_component_tester : public gmt::composable_ui {
	gmt::polygon2d polygon;
	gmt::dcel2d dcel;

	gmt::dcel_component* dcel_component;
public:
	dcel_component_tester()
	  : gmt::composable_ui("DCEL component tester"),
	    dcel_component(new gmt::dcel_component(polygon, dcel))
	{
		clear_color(black);
		add_component(new gmt::exit_component);
		add_component(new gmt::zoom_component);
		add_component(new gmt::pan_component);
		add_component(dcel_component);
	}

	~dcel_component_tester()
	{}

	static size_t n_edge_in_orbit(gmt::dcel2d::vertex* v)
	{
		size_t n = 0;

		if(v->incident_edge != nullptr){
			gmt::dcel2d::edge* e = v->incident_edge;

			do{
				n++;
				e = e->twin->prev;
			}while(e != v->incident_edge);
		}

		return n;
	}


	static gmt::dcel2d::edge* get_edge_in_orbit(
			gmt::dcel2d::vertex* v,
			size_t index)
	{
		size_t n = 0;

		if(v->incident_edge != nullptr){
			gmt::dcel2d::edge* e = v->incident_edge;

			do{
				if(n == index)
					return e;
				n++;
				e = e->twin->prev;
			}while(e != v->incident_edge);
		}

		return nullptr;
	}

	static size_t n_edge_in_face(const gmt::dcel2d::face* f)
	{
		size_t n = 0;

		if(gmt::dcel2d::edge* e = f->incident_edge){
			do{
				e = e->next;
				n++;
			}while(e != f->incident_edge);
		}

		return n;
	}

	static gmt::dcel2d::edge* get_edge_in_face(
			const gmt::dcel2d::face* f,
			size_t index)
	{
		size_t n = 0;

		if(gmt::dcel2d::edge* e = f->incident_edge){
			do{
				if(n == index)
					return e;
				e = e->next;
				n++;
			}while(e != f->incident_edge);
		}

		return nullptr;
	}


	void draw_vertex_mode_current_edge()
	{
		glPushAttrib(GL_ENABLE_BIT); 
		glLineWidth(2.5);
		glPointSize(5);
		color(green);

		const double round_time = .2;

		gmt::dcel2d::vertex* v = dcel_component->get_nearest_vertex();
		plot(v->data.p, GL_POINTS);

		size_t orbit_size = n_edge_in_orbit(v);

		double t = get_time();
		size_t index = fmod(t/round_time, (double)orbit_size);
		if(gmt::dcel2d::edge* e = get_edge_in_orbit(v, index))
			plot(e, GL_LINES);

		glPopAttrib();
	}

	void draw_vertex_mode()
	{
		gmt::point2d p;
		dcel_component->get_nearest_edge(p);

		color(red);
		plot(p, GL_POINTS);
		draw_vertex_mode_current_edge();
	}
	
	void draw_edge_mode_current_edge()
	{
		const double round_time = .2;

		const gmt::dcel2d::face* f = dcel_component->get_mouse_face();
		size_t n_edge = n_edge_in_face(f);
		double t = get_time();
		size_t index = fmod(t/round_time, (double)n_edge);
		gmt::dcel2d::edge* e = get_edge_in_face(f, index);

		color(green);
		plot(e, GL_LINES);
	}

	void draw_edge_mode()
	{
		gmt::dcel2d::vertex* v0 =
			dcel_component->get_first_vertex();

		if(v0){
			glPushAttrib(GL_ENABLE_BIT); 
			glLineStipple(4, 0xAAAA); 
			glEnable(GL_LINE_STIPPLE);

			color(gray);
			begin(GL_LINES);
			plot(v0->data.p);
			plot(dcel_component
				->get_nearest_vertex()
				->data.p);
			end();

			glPopAttrib();

			begin(GL_POINTS);
			color(red);
			plot(v0->data.p);
			plot(dcel_component
				->get_nearest_vertex()
				->data.p);
			end();
		}else{
			color(red);
			plot(dcel_component
				->get_nearest_vertex()->data.p,
				GL_POINTS
			);
		}

		draw_edge_mode_current_edge();
	}

	void draw()
	{
		clear();
		glPointSize(5);

		color(white);
		if(dcel_component->dcel_ready()){
			plot(dcel, EDGES, GL_LINES);
			plot(dcel, VERTICES, GL_POINTS);

			if(dcel_component->get_mode()
				== gmt::dcel_component::ADD_VERTEX){
				draw_vertex_mode();
			}else{
				draw_edge_mode();
			}
		}else{
			plot(polygon, GL_LINE_LOOP);
		}
	}
};

int main()
{

	dcel_component_tester app;
	app.run();

	return 0;
}
