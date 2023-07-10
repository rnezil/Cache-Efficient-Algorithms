#define MAX_MULTIPLY_BLOCK_SIZE 64

#include <cstddef>
#include <cassert>

namespace ra::cache {

struct mult_helper{
	std::size_t n;
	std::size_t p;
	bool valid;
};

static mult_helper orig;

template<class T>
void matrix_multiply( const T* a, const T* b, std::size_t m, std::size_t n, 
		std::size_t p, T* c ){
	bool last_pass {false};
	if( !orig.valid ){
		// Zero out output matrix in
		// preparation for accumulation
		std::size_t c_size = m * p;
		for( std::size_t i = 0; i < c_size; ++i )
			*(c + i) = T(0);

		// Store original matrix dimensions
		orig.n = n;
		orig.p = p;
		orig.valid = true;
		last_pass = true;
	}

	if( m * n * p > MAX_MULTIPLY_BLOCK_SIZE ){
		// If problem size not small enough, recurse
		if( n >= m && n >= p ){
			// Variables for clarity
			std::size_t n_left = n - n/2;
			std::size_t n_right = n/2;
			assert( n_left + n_right == n );

			// Left branch:
			// left half of a times top half of b
			// write to c
			matrix_multiply(a, b, m, n_left, p, c);

			// Right branch:
			// right half of a times bottom half of b
			// write to c
			matrix_multiply(a + n_left, b + n_left*orig.p, m, n_right, p, c);

		}else if( m >= n && m >= p ){
			// Variables for clarity
			std::size_t m_left = m - m/2;
			std::size_t m_right = m/2;
			assert( m_left + m_right == m );

			// Left branch:
			// top half of a times b
			// write to top half of c
			matrix_multiply(a, b, m_left, n, p, c);

			// Right branch:
			// bottom half of a times b
			// write to bottom half of c
			matrix_multiply(a + m_left*orig.n, b, m_right, n, p, c + m_left*orig.p);
		}else{
			// Variables for clarity
			std::size_t p_left = p - p/2;
			std::size_t p_right = p/2;
			assert( p_left + p_right == p );

			// Left branch:
			// a times left half of b
			// write to left half of c
			matrix_multiply(a, b, m, n, p_left, c);

			// Right branch
			// a times right half of b
			// write to right half of c
			matrix_multiply(a, b + p_left, m, n, p_right, c + p_left);
		}
	}else{
		// Once problem size small enough, 
		// write buffer
		for( std::size_t i = 0; i < m; ++i ){
			for( std::size_t j = 0; j < p; ++j ){
				for( std::size_t k = 0; k < n; ++k ){
					*(c + j + i*orig.p) += 
						*(a + k + i*orig.n) *
						*(b + j + k*orig.p);
				}
			}
		}
	}

	// If matrix multiplication complete,
	// reset mult_helper for next go
	if( last_pass ){
		orig.valid = false;
	}
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

