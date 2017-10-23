#pragma once

#include <memory>
#include "polygon.hpp"
#include "dcel/dcelp.hpp"

namespace gmt {

template<typename polygon_type>
std::unique_ptr<dcel2d> from_polygon(const polygon_type& p)
{
	std::unique_ptr<dcel2d> d(new dcel2d);

	if(size_t n = p.size()){
		gmt::point2d p0 = p[0];
		gmt::point2d p1;
		gmt::dcel2d::vertex* first = d->add_vertex(p0);
		gmt::dcel2d::vertex* v0 = first;
		gmt::dcel2d::vertex* v1;

		for(size_t i=1; i< n; i++){
			p1 = p[i%n];
			v1 = d->add_vertex(p1);

			d->add_edge(v0, v1);

			p0 = p1;
			v0 = v1;
		}

		d->add_edge(v1, first);
	}

	return d;
}

void from_polygon(
	dcel2d& d,
	const polygon2d& p)
{

	if(size_t n = p.size()){
		gmt::point2d p0 = p[0];
		gmt::point2d p1;
		gmt::dcel2d::vertex* first = d.add_vertex(p0);
		gmt::dcel2d::vertex* v0 = first;
		gmt::dcel2d::vertex* v1;

		for(size_t i=1; i< n; i++){
			p1 = p[i%n];
			v1 = d.add_vertex(p1);

			d.add_edge(v0, v1);

			p0 = p1;
			v0 = v1;
		}

		d.add_edge(v1, first);
	}
}


};
