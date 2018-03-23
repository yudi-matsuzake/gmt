#pragma once

#include <gmt/algorithm/point-in-polygon.hpp>
#include <gmt/algorithm/direction.hpp>
#include <gmt/dcel/dcel_structure.hpp>

namespace gmt {

template<
	typename value_type,
	typename point_type,
	size_t n_dimension
>
class dcelp_placeholder {
public:
	dcelp_placeholder()
	{}

	dcelp_placeholder(
		const gmt::point<point_type, n_dimension>& p,
		const value_type& value = 0)
		: value(value), p(p)
	{}
	~dcelp_placeholder()
	{}

	value_type value;
	gmt::point<point_type, n_dimension> p;
};

/** Doubly connected edge list with point vertices
  * @tparam vertex_type the type holded by vertices
  * @tparam edge_type the type holded by edges
  * @tparam face_type the type holded by faces
  */
template<
	typename point_type,
	size_t n_dimension,
	typename vertex_type,
	typename edge_type,
	typename face_type
>
class dcelp : public dcel<
		dcelp_placeholder<vertex_type, point_type, n_dimension>,
		edge_type,
		face_type
	> {
public:
	using dcelbase = dcel<
		dcelp_placeholder<vertex_type, point_type, n_dimension>,
		edge_type,
		face_type
	>;

	using edge = typename dcelbase::edge;
	using vertex = typename dcelbase::vertex;
	using face = typename dcelbase::face;

	dcelp()
	{}

	~dcelp()
	{}

	static bool dcel_point_in_face(const face* f, const point2d& p)
	{
		polygon2d polygon;

		if(edge* e = f->incident_edge){
			do{
				polygon.push_back(e->origin->data.p);
				e = e->next;
			}while(e != f->incident_edge);
		}

		return point_in_polygon(polygon, p);
	}

	/**
	  * calculates whether e1 has some point to the right based
	  * on e0
	  */
	static bool dcel_has_some_point_right(edge* e0, edge* e1)
	{
		return direction_in(
				e0->origin->data.p,
				e0->destination->data.p,
				e1->origin->data.p

			) == RIGHT || direction_in(
				e0->origin->data.p,
				e0->destination->data.p,
				e1->destination->data.p
			) == RIGHT;
	}

	/**
	  * Find the face in the dcel below the point `p`
	  */
	static face* dcel_find_face(dcelp& dcel, const point2d& p)
	{
		if(!dcel.n_face())
			return dcel.external_face();

		auto* f = dcel.face_at(0);
		edge* closest_right = nullptr;
		auto* e = f->incident_edge;

		bool visit_new_face = true;

		do{
			if(visit_new_face){
				visit_new_face = false;
				if(f == dcel.external_face()){
					if(!dcel_point_in_face(f, p))
						return dcel.external_face();
				}else{
					if(dcel_point_in_face(f, p))
						return f;
				}
			}

			point2d a = e->origin->data.p;
			point2d b = e->destination->data.p;

			direction d = direction_in(a, b, p);

			if(d == RIGHT){
				if(closest_right == nullptr ||
					dcel_has_some_point_right(closest_right, e)){
					closest_right = e;
					e = e->twin;
					f = e->incident_face;
					visit_new_face = true;
				}
			}else if(d == ON){
				if(is_between(a, b, p))
					return f;
			}

			if(f == dcel.external_face())
				e = e->prev;
			else
				e = e->next;

		}while(true);
	}

	/*
	 * add edge with point semantic
	 * TODO: add tratament when the edge is not a diagonal
	 */
	virtual edge* add_edge(
		vertex* a,
		vertex* b,
		face* face = nullptr)
	{

		if(face == nullptr){
			if(a->incident_edge == nullptr){
				face = dcel_find_face(*this, a->data.p);
			}else if(b->incident_edge == nullptr){
				face = dcel_find_face(*this, b->data.p);
			}else if(edge* e = b->incident_edge){

				edge* last_left = nullptr;

				do{
					auto d = gmt::direction_in(
						e->origin->data.p,
						e->destination->data.p,
						a->data.p
					);

					if(d == LEFT){
						last_left = e;
					}else if(d == RIGHT){
						if(last_left)
							break;
					}

					e = e->next;
					e = e->twin;
				}while(e != b->incident_edge);

				if(!last_left)
					last_left = e;
				face = last_left->incident_face;
			}
		}

		return dcelbase::add_edge(a, b, face);
	}

};

typedef dcelp<double, 2, void*, void*, void*> dcel2d;
typedef dcelp<double, 4, void*, void*, void*> dcel3d;

}
