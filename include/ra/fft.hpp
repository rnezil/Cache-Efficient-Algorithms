#include <numbers>
#include <cstddef>
#include <cmath>
#include <utility>
#include <cassert>
#include "ra/matrix_transpose.hpp"

namespace ra::cache{

template <class T>
void forward_fft(T* x, std::size_t n){
	// Enforce n is power of 2
	//assert((n & (n - 1)) == 0);
	
	// Require that there be at least 1 sample in
	// the input signal
	assert(n);

	// Create variables to store matrix dimensions
	std::size_t n1 = static_cast<long unsigned>(std::sqrt(static_cast<long double>(n)));
	std::size_t n2 = n1;

	// Algorithm for finding n1 and n2 is n is not
	// a perfect square
	if( n1 * n2 != n ){
		++n1;
		n2 = n / n1;

		while( n1 * n2 != n ){
			--n1;
			n2 = n / n1;
		}

		// Ensure n1 as close to sqrt(n) as possible
		long double n1_diff = std::sqrt(n) - n1;
		if( n1_diff < 0 )
			n1_diff *= -1;

		long double n2_diff = std::sqrt(n) - n2;
		if( n2_diff < 0 )
			n2_diff *= -1;

		if( n1_diff > n2_diff )
			std::swap(n1, n2);
	}

	// Code is broken if this assertion fails
	assert(n1 * n2 == n);
}

}

