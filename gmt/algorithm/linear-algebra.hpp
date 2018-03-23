#pragma once

#include <cmath>
#include <assert.h>

#include <list>

#include <gmt/mat.hpp>
#include <gmt/exception.hpp>

namespace gmt {

template<typename T, std::size_t l, std::size_t c>
mat<T, l, 1> gaussian_elimination(const mat<T, l, c>& a, const mat<T, l, 1>& b)
{
	assert(a.rows() > 1 && a.columns() > 1);
	assert(b.rows() == a.rows() && b.columns() == 1);

	mat<T, l, c> A = a.copy();
	mat<T, l, 1> B = b.copy();
	mat<T, l, 1> x;
	std::list<std::pair<int, int>> swaped_row;

	for(std::size_t i=0; i<A.rows(); i++){

		/*
		 * search for the maximal value in this column
		 */
		T max = std::abs(A[i][i]);
		std::size_t max_row = i;

		for(std::size_t j=i+1; j< A.rows(); j++){
			T tmp = std::abs(A[j][i]);
			if(tmp > max){
				max_row = j;
				max = tmp;
			}
		}

		/*
		 * if A[max_row][i] was equal to zero, the system might
		 * not have a solution
		 */
		if(A[max_row][i] == 0){
			if(max_row <= (A.columns()-1))
				throw system_has_no_solution<T, l, c>(a, b);
			else
				break; // overdetermined
		}

		/*
		 * swap the maximum row with the current
		 */
		if(i != max_row){
			A.swap_rows(i, max_row);
			B.swap_rows(i, max_row);
			swaped_row.push_front(std::pair<int, int>(i, max_row));
		}

		/*
		 * Make all rows below this zero in the current column
		 */
		for(size_t j = i+1; j< A.rows(); j++){
			T coef = A[j][i]/A[i][i];

			for(size_t k=i; k<A.columns(); k++){
				if(i == k)
					A[j][k] = 0;
				else
					A[j][k] -= coef*A[i][k];
			}

			B[j][0] -= coef*B[i][0];
		}

	}

	/*
	 * resolve x with the diagonal
	 */
	size_t i = A.rows();
	do{
		i--;
		x[i][0] = B[i][0]/A[i][i];
		size_t j = i;

		if(j != 0){
			do{
				j--;
				B[j][0] -= A[j][i] * x[i][0];
			}while(j != 0);
		}
	}while(i != 0);

	return x;
}

/**
 * resolves the system `a`, no format A*x = b
 */
template<typename T, std::size_t l, std::size_t c>
mat<T, l, 1> resolve(const mat<T, l, c>& a, const mat<T, l, 1>& b)
{
	return gaussian_elimination(a, b);
}

};
