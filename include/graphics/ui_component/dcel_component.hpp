#pragma once

#include "dcel.hpp"
#include "graphics/ui_component/single_polygon_component.hpp"
#include "dcel/construct.hpp"
#include "algorithm/distance.hpp"
#include "ui_component.hpp"

namespace gmt {

class dcel_component : public single_polygon_component {
public:
	typedef enum mode {
		ADD_VERTEX,
		ADD_EDGE
	} mode;

protected:
	dcel2d& dcel;
	int button;
	int action;
	int finish_polygon_button;
	int clear_button;
	int toggle_mode_button;

	bool poly_finished;

	mode m;
	gmt::dcel2d::vertex* first_vertex;

	point2d mousepos;

	/*
	 * max distance to break an edge, i.e.,
	 * max distance to insert vertex in edge
	 */
	double max_distance_break_edge;

public:

	dcel_component(
		polygon2d& poly,
		dcel2d& dcel,
		int poly_button = GLFW_MOUSE_BUTTON_LEFT,
		int poly_action = GLFW_RELEASE,
		int button = GLFW_MOUSE_BUTTON_LEFT,
		int action = GLFW_RELEASE,
		int finish_polygon_button = GLFW_KEY_ENTER,
		int clear_button = GLFW_KEY_C,
		int toggle_mode_button = GLFW_KEY_M)
		: single_polygon_component(poly, poly_button, poly_action),
		  dcel(dcel),
		  button(button),
		  action(action),
		  finish_polygon_button(finish_polygon_button),
		  clear_button(clear_button),
		  toggle_mode_button(toggle_mode_button),
		  poly_finished(false),
		  m(ADD_VERTEX),
		  first_vertex(nullptr),
		  max_distance_break_edge(50.0)
	{}

	~dcel_component()
	{}

	void build_dcel()
	{
		dcel.clear();
		from_polygon(dcel, poly);
	}

	void finish_polygon()
	{
		if(!poly_finished){
			poly_finished = true;
			build_dcel();
		}
	}

	void clear()
	{
		poly.clear();
		poly_finished = false;
	}

	void toggle_mode()
	{
		if(m == ADD_VERTEX)
			m = ADD_EDGE;
		else
			m = ADD_VERTEX;
	}

	mode get_mode() const
	{
		return m;
	}

	point2d get_mouse_position() const
	{
		return mousepos;
	}

	/**
	  * return the nearest vertex if the distance is
	  * less or equal to `max_distance_choose_vertex`
	  * or nullptr otherwise
	  */
	dcel2d::vertex* get_nearest_vertex()
	{
		if(!dcel_ready() || dcel.n_vertex() == 0)
			return nullptr;

		auto* nearest = dcel.vertex_at(0);
		double nearest_distance = gmt::distance(
			mousepos,
			nearest->data.p
		);

		for(size_t i=1; i<dcel.n_vertex(); i++){
			auto* v = dcel.vertex_at(i);
			double distance = gmt::distance(
				mousepos,
				v->data.p
			);

			if(distance < nearest_distance){
				nearest = v;
				nearest_distance = distance;
			}
		}

		return nearest;
	}

	/**
	  * return the face that the mouse is on
	  */
	const dcel2d::face* get_mouse_face()
	{
		return dcel2d::dcel_find_face(dcel, mousepos);
	}

	bool dcel_ready() const
	{
		return poly_finished;
	}

	dcel2d::vertex* get_first_vertex()
	{
		return first_vertex;
	}

	/**
	  * return the nearest edge if the distance is
	  * less or equal to `max_distance_break_edge`
	  * or nullptr otherwise
	  *
	  * @param nearest_point reference to a point2d structure
	  * 	that will be overwrote with the nearest point
	  */
	dcel2d::edge* get_nearest_edge(point2d& nearest_point)
	{
		if(!dcel_ready() && dcel.n_edge())
			return nullptr;

		auto* nearest = dcel.edge_at(0);
		nearest_point = gmt::nearest_point(
			mousepos,
			segment2d{
				nearest->origin->data.p,
				nearest->destination->data.p
			}
		);

		double nearest_distance = gmt::distance(
			mousepos,
			nearest_point
		);

		for(size_t i=1; i<dcel.n_edge(); i++){
			auto* e = dcel.edge_at(i);
			point2d p = gmt::nearest_point(
				mousepos,
				segment2d{
					e->origin->data.p,
					e->destination->data.p,
				}
			);

			double distance = gmt::distance(
				mousepos,
				p
			);

			if(distance < nearest_distance){
				nearest = e;
				nearest_distance = distance;
				nearest_point = p;
			}
		}

		if(nearest_distance > max_distance_break_edge){
			nearest_point = get_mouse_position();
			return nullptr;
		}

		return nearest;
	}

	dcel2d::edge* get_nearest_edge()
	{
		point2d dumb;
		return get_nearest_edge(dumb);
	}

	virtual void on_mouse_button(	composable_ui& ui,
					int button,
					int action,
					int mods)
	{
		if(dcel_ready()){
			if(button == this->button && action == this->action){
				if(m == ADD_VERTEX){
					gmt::point2d p;
					dcel2d::edge* e = get_nearest_edge(p);
					if(e != nullptr)
						dcel.add_vertex(p, e);
					else
						dcel.add_vertex(p);
				}else{
					if(first_vertex){
						dcel.add_edge(
							first_vertex,
							get_nearest_vertex()
						);
						first_vertex = nullptr;
					}else{
						first_vertex =
							get_nearest_vertex();
					}
				}
			}
		}else{
			single_polygon_component::on_mouse_button(
				ui,
				button,
				action,
				mods
			);
		}
	}

	virtual void on_key (	composable_ui& ui,
				int key,
				int scancode,
				int action,
				int mods)
	{
		single_polygon_component::on_key(
			ui,
			key,
			scancode,
			action,
			mods
		);

		if(action == this->action){
			if(key == finish_polygon_button)
				finish_polygon();
			else if(key == clear_button)
				clear();
			else if(key == toggle_mode_button)
				toggle_mode();
		}

	}

	virtual void on_mouse_position(	composable_ui& ui,
					double x,
					double y)
	{
		single_polygon_component::on_mouse_position(ui, x, y);
		mousepos = ui.get_mouse_point(x, y);
	}

};

}
