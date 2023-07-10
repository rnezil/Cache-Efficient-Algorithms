#define MAX_MULTIPLY_BLOCK_SIZE 64

#include <cstddef>

namespace ra::cache {

struct mult_helper{
	static std::size_t n_orig {0};
	static std::size_t p_orig {0};
	static bool valid {false};
}

template<class T>
void matrix_multiply( const T* a, const T* b, std::size_t m, std::size_t n, 
		std::size_t p, T* c ){
	bool last_pass {false};
	if( !mult_helper::valid ){
		// Zero out output matrix in
		// preparation for accumulation
		std::size_t c_size = m * p;
		for( std::size_t i = 0; i < c_size )
			*(c + i) = T(0);

		// Store original matrix dimensions
		mult_helper::n_orig = n;
		mult_helper::p_orig = p;
		mult_helper::valid = true;
		last_pass = true;
	}

	if( m * n * p > MAX_MULTIPLY_BLOCK_SIZE ){
		// If problem size not small enough, recurse
		if( n >= m && n >= p ){
			// Size of left chunk
			std::size_t n_prime = n - n/2;

			// Left branch
			matrix_multiply(a, b, m, n_prime, p, c);

			// Right branch
			matrix_multiply(a + n_prime, b + p*n_prime, m, n - n_prime, p, c);
		}else if( m >= n && m >= p ){
			// Size of left chunk
			std::size_t m_prime = m - m/2;

			// Left branch
			matrix_multiply(a, b, m_prime, n, p, c);

			// Right branch
			matrix_multiply(a + n*m_prime, b, m - m_prime, n, p, c + p*m_prime);
		}else{
			// Size of left chunk
			std::size_t p_prime = p - p/2;

			// Left branch
			matrix_multiply(a, b, m, n, p_prime, c);

			// Right branch
			matrix_multiply(a, b + p_prime, m, n, p - p_prime, c + p_prime);
		}
	}else{
		// Once problem size small enough, 
		// write buffer
		for( std::size_t i = 0; i < m; ++i ){
			for( std::size_t j = 0; j < p; ++j ){
				for( std::size_t k = 0; k < n; ++k ){
					*(c + j + i*mult_helper::p_orig) += 
						*(a + k + i*mult_helper::n_orig) *
						*(b + j + k*mult_helper::p_orig);
				}
			}
		}
	}

	// If matrix multiplication complete,
	// reset mult_helper for next go
	if( last_pass )
		mult_helper::valid = false;
}

template<class T>
void naive_matrix_multiply( const T* a, const T* b, std::size_t m, std::size_t n, 
		std::size_t p, T* c ){
	T sum(0);
	for( std::size_t i = 0; i < m; ++i ){
		for( std::size_t j = 0; j < p; ++j ){
			for( std::size_t k = 0; k < n; ++k ){
				sum += *(a + k + i*n) * *(b + j + k*p);
			}
			*c = sum;
			++c;
			sum = T(0);
		}
	}
}

}

