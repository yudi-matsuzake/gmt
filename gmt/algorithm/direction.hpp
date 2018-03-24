#pragma once

#include <gmt/polygon.hpp>
#include <gmt/vec.hpp>
#include <gmt/algorithm/misc.hpp>
#include <gmt/pi.hpp>

namespace gmt {

typedef enum direction {
	LEFT,
	RIGHT,
	ON
} direction;

typedef enum vertex_type {
	REGULAR,
	START,
	END,
	SPLIT,
	MERGE
} vertex_type;

/** Calculates the direction of point `p2` in reference
  * of segment of `p0` and `p1`
  *
  */
template<typename T>
direction direction_in(
	const vec<T, 2>& v0,
	const vec<T, 2>& v1,
	double e = 0.0)
{
	double tmp = signed_parallelogram_area(v0, v1);

	if(is_equal(tmp, 0.0, e))
		return ON;
	else if(tmp < 0.0)
		return RIGHT;
	else
		return LEFT;
}

template<typename T>
direction direction_in(
	const point<T, 2>& p0,
	const point<T, 2>& p1,
	const point<T, 2>& p2,
	double e = 0.0)
{
	return direction_in(vec<T, 2>(p0, p1), vec<T, 2>(p0, p2), e);
}

template<typename T>
direction direction_in(
	const segment<T, 2>& s,
	const point<T, 2>& p,
	const T& e = 0.0)
{
	return direction_in(s.from, s.to, p, e);
}

/**
  * calculates the angle of two vectors going to left
  */
template<typename T, std::size_t n_dimension>
double angle_to_left(
	const vec<T, n_dimension>& v0,
	const vec<T, n_dimension>& v1)
{
	double minor_angle = v0.angle(v1);

	if(direction_in(v0, v1) == RIGHT)
		return 2*pi - minor_angle;

	return minor_angle;
}

/**
  * check if an vetex `i` of the polygon `poly` is
  * reflex
  * TODO: this function consider the polygon was created
  * in anti-clockwise; need to check that
  */
template<typename T, std::size_t n_dimension>
bool is_reflex(const polygon<T, n_dimension>& poly, std::size_t i)
{
	if(poly.size() < 3)
		return false;

	size_t next_index = (i + 1) % poly.size();
	size_t prev_index = (i == 0)
				? poly.size() - 1
				: i - 1;

	direction d = direction_in(poly[i], poly[prev_index], poly[next_index]);

	return d == LEFT;
}

/** Checks whether the vertex `index` of the polygon `poly` is
  * an ear
  *
  */
template<typename T, std::size_t n_dimension>
bool is_ear(const polygon<T, n_dimension>& poly, std::size_t index)
{
	if(poly.size() < 4)
		return false;

	if(is_reflex(poly, index))
		return false;

	size_t next_index = (index + 1) % poly.size();
	size_t prev_index = (index == 0)
				? poly.size() - 1
				: index - 1;

	for(	std::size_t i = (next_index + 1)%poly.size();
		i != prev_index;
		i = (i+1)%poly.size())
	{
		if(	direction_in(poly[prev_index], poly[index], poly[i])
				== LEFT
			&& direction_in(poly[index], poly[next_index], poly[i])
				== LEFT
			&& direction_in(poly[next_index], poly[prev_index], poly[i])
				== LEFT){
			return false;
		}
	}

	return true;
}

/** Checks whether the vertex `index` of the polygon `poly` is
  * a mouth
  *
  */
template<typename T, std::size_t n_dimension>
bool is_mouth(const polygon<T, n_dimension>& poly, std::size_t index)
{
	if(poly.size() < 4)
		return false;

	if(!is_reflex(poly, index))
		return false;

	size_t next_index = (index + 1) % poly.size();
	size_t prev_index = (index == 0)
				? poly.size() - 1
				: index - 1;

	for(	std::size_t i = (next_index + 1)%poly.size();
		i != prev_index;
		i = (i+1)%poly.size())
	{
		if(	direction_in(poly[prev_index], poly[index], poly[i])
				== RIGHT
			&& direction_in(poly[index], poly[next_index], poly[i])
				== RIGHT
			&& direction_in(poly[next_index], poly[prev_index], poly[i])
				== RIGHT){
			return false;
		}
	}

	return true;
}

/**
  * calculate if p0 is below p1
  */
template<typename T, std::size_t n_dimension>
inline bool is_below(
	const point<T, n_dimension>& p0,
	const point<T, n_dimension>& p1)
{
	return p0.y() < p1.y() || (p0.y() == p1.y()
			&& p0.x() > p1.x());
}

/**
  * calculate if p0 is above p1
  */
template<typename T, std::size_t n_dimension>
inline bool is_above(
	const point<T, n_dimension>& p0,
	const point<T, n_dimension>& p1)
{
	return !is_below(p0, p1);
}

/**
  * calculate if p0 is on p1's left side
  */
template<typename T, std::size_t n_dimension>
inline bool is_left(
	const point<T, n_dimension>& p0,
	const point<T, n_dimension>& p1)
{
	return p0.x() <= p1.x();
}

/**
  * calculate if p0 is on p1's right side
  */
template<typename T, std::size_t n_dimension>
inline bool is_right(
	const point<T, n_dimension>& p0,
	const point<T, n_dimension>& p1)
{
	return p0.x() > p1.x();
}

/*
 * classify the vertice i of the polygon
 */
template<typename T, std::size_t n_dimension>
vertex_type classify(
	const polygon<T, n_dimension>& poly,
	size_t index)
{
	size_t next_index = (index+1) % poly.size();
	size_t prev_index = (index == 0)
				? poly.size() - 1
				: index - 1;

	bool prev_is_below = is_below(poly[prev_index], poly[index]);
	bool next_is_below = is_below(poly[next_index], poly[index]);

	if(prev_is_below != next_is_below)
		return REGULAR;

	direction d = direction_in(
		poly[prev_index],
		poly[index],
		poly[next_index]
	);

	if(prev_is_below){
		// start or split

		switch(d){
		case LEFT:
			return START;
		case RIGHT:
			return SPLIT;
		case ON:
			return REGULAR;
		}
	}else{
		// end or merge
		switch(d){
		case LEFT:
			return END;
		case RIGHT:
			return MERGE;
		case ON:
			return REGULAR;
		}
	}

	return REGULAR;
}

/** Calculate the leftmost point in the container.
  *
  * @return an iterator to the leftmost
  */
template<typename list_container>
typename list_container::iterator leftmost(list_container& l)
{
	typename list_container::iterator leftmost = l.begin();

	for(auto i = std::next(l.begin()); i != l.end(); i++){
		if(is_left(*i, *leftmost))
			leftmost = i;
	}

	return leftmost;
}

/** Calculate the rightmost point in the container.
  *
  * @return an iterator to the rightmost
  */
template<typename list_container>
typename list_container::iterator rightmost(list_container& l)
{
	typename list_container::iterator rightmost = l.begin();

	for(auto i = std::next(l.begin()); i != l.end(); i++){
		if(is_right(*i, *rightmost))
			rightmost = i;
	}

	return rightmost;
}

}
