#pragma once
#include "algorithm/direction.hpp"
#include "dcel_structure.hpp"

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
			if(edge* e = b->incident_edge){
				edge* direction_most = e;
				gmt::direction following_direction = gmt::direction_in(
					e->origin->data.p,
					e->destination->data.p,
					a->data.p
				);


				do{
					gmt::direction d = gmt::direction_in(
						e->origin->data.p,
						e->destination->data.p,
						a->data.p
					);

					if(d == following_direction){
						if(gmt::direction_in(
							direction_most->origin->data.p,
							direction_most->destination->data.p,
							e->origin->data.p
							) == following_direction){

							direction_most = e;
						}
					}

					e = e->twin;
					e = e->prev;
				}while(e != b->incident_edge);

				if(following_direction == gmt::RIGHT)
					face = direction_most->twin->incident_face;
				else
					face = direction_most->incident_face;
			}else{
				face = this->external_face();
			}
		}

		return dcelbase::add_edge(a, b, face);
	}

};

typedef dcelp<double, 2, void*, void*, void*> dcel2d;
typedef dcelp<double, 4, void*, void*, void*> dcel3d;

}
