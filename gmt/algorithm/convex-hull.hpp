#pragma once

#include <list>
#include <algorithm>
#include <set>

#include <gmt/polygon.hpp>
#include <gmt/algorithm/comparators.hpp>
#include <gmt/algorithm/direction.hpp>

namespace gmt {

using merge_hull_tangent = std::pair<
	std::list<gmt::point2d>::iterator,
	std::list<gmt::point2d>::iterator
>;

/*
 * cyclic next iterator walk
 */
std::list<gmt::point2d>::iterator merge_hull_next(
	std::list<gmt::point2d>& l,
	std::list<gmt::point2d>::iterator& i)
{
	if(std::next(i) == l.end()){
		return l.begin();
	}else{
		return std::next(i);
	}
}

/*
 * cyclic prev iterator walk
 */
std::list<gmt::point2d>::iterator merge_hull_prev(
	std::list<gmt::point2d>& l,
	std::list<gmt::point2d>::iterator& i)
{
	if(i == l.begin())
		return std::prev(l.end());
	else
		return std::prev(i);
}


/*
 * build a merge_hull tangent
 * if this function was called passing
 * (right_list, left_list, leftmost_from_right, rightmost_from_left)
 * the top tangent will be returned.
 */
merge_hull_tangent get_tangent(
	std::list<gmt::point2d>& first_list,
	std::list<gmt::point2d>& second_list,
	std::list<gmt::point2d>::iterator& first,
	std::list<gmt::point2d>::iterator& second)
{
	merge_hull_tangent tangent(first, second);
	merge_hull_tangent tangent_loop(first, second);

	bool first_t = false, second_t = false;
	std::list<gmt::point2d>::iterator candidate;
	gmt::direction d;

	while((first_t == false || second_t == false)){

		if(first_list.size() <= 1)
			first_t = true;
		if(second_list.size() <= 1)
			second_t = true;

		if(second_t == false){

			candidate = merge_hull_next(
				second_list,
				tangent.second
			);

			d = direction_in(
				*tangent.first,
				*tangent.second,
				*candidate
			);

			switch(d){
			case RIGHT:
				tangent.second = candidate;
				first_t = false;
				break;
			case LEFT:
				second_t = true;
				break;
			case ON:
				if(is_between(
					*tangent.first,
					*tangent.second,
					*candidate)){
					second_list.erase(candidate);
				}else{
					auto tmp = tangent.second;
					tangent.second = candidate;
					second_list.erase(tmp);
				}

				first_t = false;
				break;
			}

		}else if(first_t == false){

			candidate = merge_hull_prev(
				first_list,
				tangent.first
			);

			d = direction_in(
				*tangent.first,
				*tangent.second,
				*candidate
			);

			switch(d){
			case RIGHT:
				tangent.first = candidate;
				second_t = false;
				break;
			case LEFT:
				first_t = true;
				break;
			case ON:
				if(is_between(
					*tangent.first,
					*tangent.second,
					*candidate)){
					first_list.erase(candidate);
				}else{
					auto tmp = tangent.first;
					tangent.first = candidate;
					first_list.erase(tmp);
				}

				second_t = false;
				break;
			}

		}
	}

	return tangent;
}

/** Merge hull auxiliar function to merge the
  * two convex hull from left and right sides
  *
  * @param left list of left points
  * @param right list of right points
  *
  * @see merge_hull
  */
std::list<gmt::point2d> merge_hull_conquer(
	std::list<gmt::point2d>& left,
	std::list<gmt::point2d>& right)
{

	/*
	 * get the iterators that represents
	 * the leftmost point from the right side
	 * and the rightmost point from the left side.
	 *
	 * FIXME: it's possible to get the higher/lowest
	 * leftmost/rightmost in n iterations, this will make
	 * 2n
	 */
	auto lowest_leftmost_from_right = lowest_leftmost(right);
	auto lowest_rightmost_from_left = lowest_rightmost(left);

	/*
	 * get the top and bottom tangents
	 */
	auto bottom_tangent = get_tangent(
		left,
		right,
		lowest_rightmost_from_left,
		lowest_leftmost_from_right
	);

	auto highest_leftmost_from_right = higher_leftmost(right);
	auto highest_rightmost_from_left = higher_rightmost(left);
	auto top_tangent = get_tangent(
		right,
		left,
		highest_leftmost_from_right,
		highest_rightmost_from_left
	);

	/*
	 * treat when the left side or the right side
	 * has just two elements
	 */
	if(left.size() == 2){
		if(left.begin() == bottom_tangent.first &&
			std::next(left.begin()) == top_tangent.second){
			left.splice(left.begin(), left, std::next(left.begin()));
		}
	}

	if(right.size() == 2){
		if(right.begin() == top_tangent.first &&
			std::next(right.begin()) == bottom_tangent.second){
			right.splice(right.begin(), right, std::next(right.begin()));
		}
	}

	/*
	 * erase points in the left side list of points that will
	 * not be in the convex hull
	 * we will return the left list as the merged list
	 */
	for(auto i = merge_hull_next(left, bottom_tangent.first);
		 i != top_tangent.second;){
		auto cup = merge_hull_next(left, i);
		left.erase(i);

		i = cup;
	}

	/*
	 * add the points of the right list that will be in the
	 * convex hull
	 */
	auto i = bottom_tangent.second;
	auto inserted = bottom_tangent.first;
	auto last = merge_hull_next(right, top_tangent.first);
	do{
		inserted = left.insert(std::next(inserted), i, std::next(i));
		i = merge_hull_next(right, i);
	}while(i != last);

	return left;
}

/**
  * Merge hull auxiliar function.
  *
  * @see merge_hull
  */
std::list<gmt::point2d> merge_hull_divide(
	const std::vector<gmt::point2d>& sorted,
	size_t start,
	size_t end)
{
	std::list<gmt::point2d> l;

	/*
	 * treat the cases when the size is three, i.e., a triangle
	 * and when the size is two
	 */
	size_t siz = end - start;
	if(siz == 3){
		size_t a = start;
		size_t b = start+1;
		size_t c = start+2;
		switch(gmt::direction_in(
				sorted[a],
				sorted[b],
				sorted[c])){
			case LEFT:
				l.push_back(sorted[a]);
				l.push_back(sorted[b]);
				l.push_back(sorted[c]);
				break;
			case RIGHT:
				l.push_back(sorted[c]);
				l.push_back(sorted[b]);
				l.push_back(sorted[a]);
				break;
			case ON:

				/*
				 * if the three points are collinear,
				 * then, remove the point that are in
				 * the middle of the other two
				 */
				if(is_between(
					sorted[a],
					sorted[b],
					sorted[c])){
					l.push_back(sorted[a]);
					l.push_back(sorted[b]);
				}else if(is_between(
					sorted[a],
					sorted[c],
					sorted[b])){
					l.push_back(sorted[a]);
					l.push_back(sorted[c]);
				}else{
					l.push_back(sorted[b]);
					l.push_back(sorted[c]);
				}
				break;
		}

		return l;
	}else if(siz < 3){
		for(size_t i = start; i<end; i++)
			l.push_back(sorted[i]);
		return l;
	}

	size_t m = (start + end)/2;

	/*
	 * divide and two subproblems
	 */
	std::list<gmt::point2d> left = merge_hull_divide(sorted, start, m);
	std::list<gmt::point2d> right = merge_hull_divide(sorted, m, end);

	/*
	 * merge the responses from left and right
	 */
	return merge_hull_conquer(left, right);
}

/**
  * Merge hull is a algorithm to calculate the convex hull of a collection
  * of points. Merge hull is based on divide and conquer approach and it's
  * complexity is O(n lg n).
  *
  * @param points	container of points
  *
  * @return	a convex polygon that contains all the points in the `points`
  * 		collection
  *
  */
template<typename list_container>
gmt::polygon2d merge_hull(const list_container& points)
{
	/*
	 * create a set to sort and uniquely add a point
	 */
	std::set<point2d, axis_comparator> sorted_unique(
		points.begin(),
		points.end(),
		axis_comparator()
	);

	/*
	 * create a random access point vector because
	 * list_container can be std::list which hasn't
	 * random access to it's elements
	 */
	std::vector<gmt::point2d> sorted(
		sorted_unique.begin(),
		sorted_unique.end()
	);

	/*
	 * call merge_hull_divide which returns a list of points
	 * of the convex hull
	 */
	std::list<gmt::point2d> ch_list = merge_hull_divide(
		sorted,
		0,
		sorted.size()
	);

	polygon2d ch;
	ch.reserve(points.size());

	/*
	 * build the polygon
	 */
	for(auto& i : ch_list)
		ch.push_back(i);

	return ch;
}

}
