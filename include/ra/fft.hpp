#include <numbers>
#include <cmath>
#include <cassert>
#include <complex>
#include "ra/matrix_transpose.hpp"

namespace ra::cache{

template <class T>
void forward_fft(T* x, std::size_t n){
	// Enforce n is power of 2
	assert((n & (n - 1)) == 0);

	if( n > 4 ){
		// Create variables to store matrix dimensions
		std::size_t n1 = static_cast<long unsigned>(std::sqrt(
					static_cast<long double>(n)));
		std::size_t n2 = n1;

		// Algorithm for finding n1 and n2 if n is not
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
	
		// Transpose input matrix in place
		matrix_transpose(x, n1, n2, x);	

		// Replace each row with its n1-point DFT
		for( std::size_t i = 0; i < n2; ++i )
			forward_fft(x + n1*i, n1);

		// Multiply by twiddle factors
		T j = std::complex<typename T::value_type>(0,2/n);
		T twiddle = std::pow(std::numbers::e_v<typename T::value_type>,
				j * std::numbers::pi_v<typename T::value_type>);

		// For matrix x with n2 rows and n1 columns
		for( std::size_t k = 0; k < n2; ++k )
			for( std::size_t j = 0; j < n1; ++j )
				x[j + k*n1] *= std::pow(twiddle, -1*k*j);

		// Transpose in place for elemental contiguousness
		matrix_transpose( x, n2, n1, x );

		// Replace each row with its n2-point DFT
		for( std::size_t i = 0; i < n1; ++i )
			forward_fft(x + n2*i, n2);

		// Tranpose in place to produce final result
		matrix_transpose( x, n1, n2, x );
	}else{
		// Compute easy DFT's once size is small enough
		if( n != 1 && n != 0 ){
			// Do nothing for a 1-point DFT
			if( n == 2){
				// 2-point DFT
				T tmp0 = x[0];
				T tmp1 = x[1];
				x[0] = tmp0 + tmp1;
				x[1] = tmp0 - tmp1;
			}else{
				// 4-point DFT
				T j = std::complex<typename T::value_type>(0,1);
				T tmp0 = x[0];
				T tmp1 = x[1];
				T tmp2 = x[2];
				T tmp3 = x[3];
				x[0] = tmp0 + tmp1 + tmp2 + tmp3;
				x[1] = tmp0 - tmp2 + j*(tmp3 - tmp1);
				x[2] = tmp0 - tmp1 + tmp2 - tmp3;
				x[3] = tmp0 - tmp2 + j*(tmp1 - tmp3);
			}
		}
	}
}

}

