#pragma once

#include <cstdlib>

#include <vector>
#include <memory>
#include <set>

#include "point.hpp"
#include "polygon.hpp"
#include "exception.hpp"

namespace gmt {

/** Doubly connected edge list
  * @tparam vertex_type the type holded by vertices
  * @tparam edge_type the type holded by edges
  * @tparam face_type the type holded by faces
  */
template<
	typename vertex_type,
	typename edge_type,
	typename face_type
>
class dcel{
public:
	class edge;
	class vertex;
	class face;
private:
	/*
	 * typedefs
	 */
	typedef std::unique_ptr<edge> edgeptr;
	typedef std::unique_ptr<vertex> vertexptr;
	typedef std::unique_ptr<face> faceptr;

	/*
	 * auxiliar
	 */

	/*
	 * returns a std::pair with first and
	 * second twins edges
	 */
	std::pair<edgeptr, edgeptr> make_twins(
		vertex* a,
		vertex* b)
	{
		std::pair<edgeptr, edgeptr> twins(
			edgeptr(new edge),
			edgeptr(new edge)
		);

		twins.first->twin = twins.second.get();
		twins.second->twin = twins.first.get();

		twins.first->next = twins.second.get();
		twins.second->next = twins.first.get();
		twins.first->prev = twins.second.get();
		twins.second->prev = twins.first.get();

		twins.first->origin = a;
		twins.first->destination = b;
		twins.second->origin = b;
		twins.second->destination = a;

		twins.first->incident_face = this->m_external_face.get();
		twins.second->incident_face = this->m_external_face.get();

		/*
		 * check whether external face points to no incident
		 * edge
		 */
		if(m_external_face->incident_edge == nullptr)
			m_external_face->incident_edge = twins.second.get();

		return twins;
	}

	/*
	 * return the mutual face between a, and b
	 */
	face* mutual_face(vertex* a, vertex* b)
	{
		std::set<face*> face_set;

		if(a->incident_edge == nullptr
			|| b->incident_edge == nullptr)
			return this->m_external_face.get();

		/*
		 * orbit the vertex a
		 * adding the faces in the set
		 */
		edge* e = a->incident_edge;
		do{
			face_set.insert(e->incident_face);
			e = e->next;
			e = e->twin;
		}while(e != a->incident_edge);

		/*
		 * orbit b and find the faces in a
		 */
		e = b->incident_edge;
		do{
			auto it = face_set.find(e->incident_face);
			if(it != face_set.end()
				&& (*it) != this->m_external_face.get())
				return *it;

			e = e->next;
			e = e->twin;
		}while(e != b->incident_edge);

		return this->m_external_face.get();
	}

	/*
	 * returns true whether the edge `e`
	 * closes a loop
	 */
	bool close_a_loop(edge* e)
	{
		edge* i = e;

		while(i->next != e){
			if(i->next == e->twin)
				return false;
			i = i->next;
		}
		
		return true;
	}

	/*
	 * check whether the new edge need a new face
	 */
	void face_fix(edge* e, face* f)
	{

		/*
		 * connect faces
		 */
		if(e->next == e->twin){
			e->incident_face = f;
			e->twin->incident_face = f;
		}else{
			e->incident_face = e->next->incident_face;
			e->twin->incident_face = e->twin->next->incident_face;
		}

		/*
		 * resolve if has a loop
		 */
		if(close_a_loop(e)){

			e->incident_face->incident_edge = e->twin;

			/*
			 * creates a new face
			 */
			faceptr f(new face);
			f->incident_edge = e;

			edge* i = e;
			do{
				i->incident_face = f.get();
				i = i->next;
			}while(i != e);

			faces.push_back(std::move(f));
		}
	}

	/*
	 * connect the orbit of the edge `e`
	 * and the vertex `v` witch `e` is the incident
	 * edge
	 */
	void connect_orbit(edge* e, vertex* v, face* f)
	{

		edge* e_aux = nullptr;

		if(v->incident_edge == nullptr){
			v->incident_edge = e;
		}else{
			edge* i = v->incident_edge;
			while(i->incident_face != f){
				i = i->next;
				i = i->twin;

				if(i == v->incident_edge){
					throw exception(
						"connect_orbit: "
						"cannot find the mutual face");
				}
			}

			e_aux = i;
		}

		if(e_aux != nullptr){
			e->next = e_aux->next;
			e->twin->prev = e_aux;
			e_aux->next->prev = e;
			e_aux->next = e->twin;
		}

	}

public:

	/** Represents a half-edge of the dcel
	  */
	class edge{
	public:
		edge()
		: twin(nullptr),
		  next(nullptr),
		  prev(nullptr),
		  origin(nullptr),
		  destination(nullptr),
		  incident_face(nullptr)
		{}

		edge(const edge_type& data)
		 : edge()
		{
			this->data = data;
		}

		~edge()
		{}

		edge* twin;
		edge* next;
		edge* prev;

		vertex* origin;
		vertex* destination;

		face* incident_face;

		edge_type data;
	};

	/** Represents a vertex of the dcel
	  */
	class vertex{
	public:
		vertex()
		 : incident_edge(nullptr)
		{}

		vertex(const vertex_type& data)
		 : vertex()
		{
			this->data = data;
		}

		~vertex()
		{}

		edge* incident_edge;
		vertex_type data;
	};

	/** Represents a face of the dcel
	  */
	class face{
	public:
		face()
		: incident_edge(nullptr)
		{}

		face(const face_type& data)
		: face()
		{
			this->data = data;
		}

		~face()
		{}

		edge* incident_edge;
		face_type data;
	};

	dcel()
	 : m_external_face(faceptr(new face))
	{}

	~dcel()
	{}

	vertex* add_vertex(const vertex_type& data)
	{
		vertices.push_back(vertexptr(new vertex(data)));
		return vertices[vertices.size() - 1].get();
	}

	vertex* add_vertex(
		const vertex_type& data,
		edge* e)
	{
		vertices.push_back(vertexptr(new vertex(data)));
		vertex* v = vertices[vertices.size() - 1].get();
		v->incident_edge = e;

		auto twins = make_twins(v, e->destination);

		twins.first->next = e->next;
		twins.first->prev = e;
		twins.second->next = e->twin;
		twins.second->prev = e->twin->prev;
		e->next->prev = twins.first.get();
		e->twin->prev->next = twins.second.get();

		twins.first->incident_face = e->incident_face;
		twins.second->incident_face = e->twin->incident_face;

		e->destination->incident_edge = twins.first.get();
		e->destination = v;
		e->twin->origin = v;
		e->next = twins.first.get();
		e->twin->prev = twins.second.get();

		edges.push_back(std::move(twins.first));
		edges.push_back(std::move(twins.second));
		return v;
	}

	edge* add_edge(
		vertex* a,
		vertex* b,
		face* face = nullptr)
	{
		if(face == nullptr)
			face = mutual_face(a, b);

		auto twins = make_twins(a, b);
		connect_orbit(twins.first.get(), b, face);
		connect_orbit(twins.second.get(), a, face);
		face_fix(twins.first.get(), face);

		edges.push_back(std::move(twins.first));
		edges.push_back(std::move(twins.second));
		return twins.first.get();
	}

	static size_t n_incident_edge(const face* f)
	{
		size_t n = 0;
		if(f->incident_edge != nullptr){
			edge* e = f->incident_edge;

			do{
				n++;
				e = e->next;
			}while(e != f->incident_edge);
		}

		return n;
	}

	static size_t n_incident_edge(const vertex* v)
	{
		size_t n = 0;
		if(v->incident_edge != nullptr){
			edge* e = v->incident_edge;

			do{
				n++;
				e = e->next;
				e = e->twin;
			}while(e != v->incident_edge);
		}

		return n;
	}

	edge* edge_at(size_t index)
	{
		return edges[index].get();
	}

	const edge* edge_at(size_t index) const
	{
		return edges[index].get();
	}

	vertex* vertex_at(size_t index)
	{
		return vertices[index].get();
	}

	const vertex* vertex_at(size_t index) const
	{
		return vertices[index].get();
	}

	face* face_at(size_t index)
	{
		return faces[index].get();
	}

	const face* face_at(size_t index) const
	{
		return faces[index].get();
	}


	size_t n_edge() const
	{
		return edges.size();
	}

	size_t n_vertex() const
	{
		return vertices.size();
	}

	size_t n_face() const
	{
		return faces.size();
	}

	face* external_face() const
	{
		return m_external_face.get();
	}

private:
	faceptr m_external_face;
	std::vector<edgeptr> edges;
	std::vector<vertexptr> vertices;
	std::vector<faceptr> faces;
};

typedef dcel<gmt::point2d, void*, void*> dcel2d;
typedef dcel<gmt::point3d, void*, void*> dcel3d;

};
