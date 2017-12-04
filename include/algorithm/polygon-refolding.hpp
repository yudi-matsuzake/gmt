#pragma once
#include <fstream>
#include <vector>

#include "algorithm/distance.hpp"
#include "polygon.hpp"
#include "polygon-operations.hpp"

#define BIAS_BOUND	0.6
#define BIAS_MULTIPLIER	1.02
#define BIAS_INITIAL	0.02

namespace gmt {

/** Refolds the polygon `a` to polygon `b` only based in a
  * distance function
  */
std::vector<polygon2d> polygon_refolding_dist(
	const gmt::polygon2d& a,
	const gmt::polygon2d& b,
	double step = 1.0)
{
	std::vector<polygon2d> interpolation;

	gmt::polygon2d distance = b - a;

	double norm;
	auto tmp = a;
	while((norm = polygon_norm(distance)) > step){
		distance *= step/norm;
		tmp += distance;
		interpolation.push_back(tmp);
		distance = b - tmp;
	}

	interpolation.push_back(tmp + distance);

	return interpolation;
}

/**
  * calculate the energy gradient of a polygon `polygon`
  */
gmt::polygon2d energy_gradient(
	const gmt::polygon2d& polygon)
{
	gmt::polygon2d g;
	g.resize(polygon.size());

	for(size_t i = 0; i<polygon.size(); i++){
		for(size_t j = 0; j<polygon.size(); j++){
			size_t j_next = (j + 1)%polygon.size();
			if(i != j && i != j_next){
				auto nearest = gmt::nearest_point(
					polygon[i],
					gmt::segment2d{
						polygon[j],
						polygon[j_next]
					}
				);

				gmt::vec2d v(polygon[i], nearest);
				double sqxy = v.x()*v.x() + v.y()*v.y();
				double sqsqxy = sqxy*sqxy;

				double g_x = -(2.0*v.x())/sqsqxy;
				double g_y = -(2.0*v.y())/sqsqxy;

				g[i].x() += g_x;
				g[i].y() += g_y;
				/* double n = v.norm(); */
				/* g[i] += v*(-4.0/(n*n*n)); */
			}
		}
	}

	return g;
}

/**
  * calculate the energy of the polygon `polygon`
  */
double compute_energy(const gmt::polygon2d& polygon)
{
	double e = 0.0;

	for(size_t i = 0; i<polygon.size(); i++){
		for(size_t j = 0; j<polygon.size(); j++){
			size_t j_next = (j + 1)%polygon.size();
			if(i != j && i != j_next){
				auto nearest = gmt::nearest_point(
					polygon[i],
					gmt::segment2d{
						polygon[j],
						polygon[j_next]
					}
				);
				gmt::vec2d v(polygon[i], nearest);
				e += 1.0/v.norm_squared();
			}
		}
	}

	return e;
}

/**
  * refolds `a` to `b` based on the paper
  * http://graphics.berkeley.edu/papers/Iben-RPP-2009-04/
  * avoiding self intersection
  */
std::vector<polygon2d> polygon_refolding(
	const gmt::polygon2d& a,
	const gmt::polygon2d& b,
	double step = 0.03,
	size_t max_iterations = 0,
	const std::string& csv_filename = "polygon-refolding.csv")
{
	std::vector<polygon2d> a_interpolation;
	std::vector<polygon2d> b_interpolation;

	auto tmp_a = a;
	auto tmp_b = b;
	auto diff  = b-a;

	size_t count_iterations = 0;
	bool has_max_iterations = max_iterations > 0;
	bool iterations_over = false;
	double norm;

	std::ofstream csv(csv_filename);

	csv << "iteration,difference,energy_a,energy_b" << std::endl;

	while(!iterations_over && (norm = polygon_norm(diff)) > step){
		double energy_a = compute_energy(tmp_a);
		double energy_b = compute_energy(tmp_b);
		csv << count_iterations	<< ','
			  << norm		<< ','
			  << energy_a		<< ','
			  << energy_b		<< std::endl;

		/*
		 * assumes that `a` is the higher energy polygon
		 */
		gmt::polygon2d* h = &tmp_a;
		gmt::polygon2d* l = &tmp_b;
		double higher_energy = energy_a;
		std::vector<polygon2d>* forward_interpolation = &a_interpolation;

		if(energy_a < energy_b){
			h = &tmp_b;
			l = &tmp_a;
			higher_energy = energy_b;
			forward_interpolation = &b_interpolation;
		}

		/*
		 * normalized distance projection and
		 * normalized energy gradient projection
		 */
		auto d = *l - *h;
		d /= polygon_norm(d);
		auto g = energy_gradient(*h);
		g /= polygon_norm(g);
		/* auto g_normalized = g * step/polygon_norm(g); */

		/*
		 * if dot(d, g) > 0.0 the direction vector must be
		 * projected so that it's perpendicular with g
		 */
		double dot = polygon_dot(d, g);
		if(dot > 0.0)
			d = d - (g*dot);

		d *= step;
		auto candidate = *h + d;
		auto energy_candidate = compute_energy(candidate);

		if(energy_candidate > higher_energy){
			/*
			 * realize bounded search
			 */
			double bias = BIAS_INITIAL;
			bool found = false;

			while(!found && bias < BIAS_BOUND){
				auto d_biased = d + g*bias;

				candidate = *h + d_biased;
				energy_candidate = compute_energy(candidate);

				if(energy_candidate < higher_energy)
					found = true;
				bias *= BIAS_MULTIPLIER;
			}

			if(!found)
				candidate = *h + (g*step);
		}

		*h = candidate;
		forward_interpolation->push_back(*h);

		diff = tmp_b - tmp_a;

		count_iterations++;
		if(has_max_iterations)
			if(count_iterations >= max_iterations)
				iterations_over = true;
	}

	csv.close();

	if(b_interpolation.size()){
		for(size_t i = 0; i<b_interpolation.size() ; i++){
			size_t index = b_interpolation.size() - i - 1;
			a_interpolation.push_back(b_interpolation[index]);
		}
	}

	return a_interpolation;
}

};
