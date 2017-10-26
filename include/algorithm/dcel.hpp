#pragma once

#include "dcel/dcelp.hpp"
#include "algorithm/point-in-polygon.hpp"

namespace gmt {

bool dcel_point_in_face(const dcel2d::face* f, const point2d& p)
{
	polygon2d polygon;

	if(dcel2d::edge* e = f->incident_edge){
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
bool dcel_has_some_point_right(dcel2d::edge* e0, dcel2d::edge* e1)
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
const dcel2d::face* dcel_find_face(const dcel2d& dcel, const point2d& p)
{
	if(!dcel.n_face())
		return dcel.external_face();

	auto* f = dcel.face_at(0);
	dcel2d::edge* closest_right = nullptr;
	auto* e = f->incident_edge;

	do{
		if(f == dcel.external_face()){
			if(!dcel_point_in_face(f, p))
				return dcel.external_face();
		}else{
			if(dcel_point_in_face(f, p))
				return f;
		}

		point2d a = e->origin->data.p;
		point2d b = e->destination->data.p;

		direction d = direction_in(a, b, p);

		switch(d){
		case LEFT:
			if(f == dcel.external_face())
				e = e->prev;
			else
				e = e->next;
			break;
		case RIGHT:
			if(closest_right == nullptr ||
				dcel_has_some_point_right(closest_right, e)){
				closest_right = e;
				e = e->twin;
				f = e->incident_face;
			}

			if(f == dcel.external_face())
				e = e->prev;
			else
				e = e->next;

			break;
		case ON:
			return f;
			break;
		}
	}while(true);
}

};
