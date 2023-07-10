#define MAX_BLOCK_SIZE 64

#include <cstddef>
#include <list>

namespace ra::cache {

// Compute tranpose of matrix with m rows, n columns,
// and data elements pointed to a. Write results to
// buffer pointed to by b.
//
// Algorithm should use cache-oblivious divide and 
// conquer strategy where matrices are divided until
// the product of m and n is equal to or less than
// sixty-four.
//
// Matrices are stored in row-major order.
template<class T>
void matrix_transpose( const T* a, std::size_t m, std::size_t n, T* b ){
	// Keeps track of vertical block spans
	std::list<std::size_t> vert_blocks {m};

	// Keeps track of horizontal block spans
	std::list<std::size_t> hori_blocks {n};

	// Iterators for the lists
	auto iter = vert_blocks.begin();

	// Temporary variable
	std::size_t split = 0;

	// Loop for getting block sizes
	while( *vert_blocks.begin() * *hori_blocks.begin() > MAX_BLOCK_SIZE ){
		// # cols > # rows
		if( *hori_blocks.begin() > *vert_blocks.begin() ){
			// Split vertically
			iter = hori_blocks.begin();
			for( std::size_t i = hori_blocks.size(); i; --i ){
				// Save 'full' block width
				split = *iter;

				// Divide in half and save value
				*iter /= 2;

				// Insert new value in front of saved
				// value; also account for case where
				// integer divison results in truncation;
				// also get iterator point to 2 before
				// next full interval
				iter = hori_blocks.insert( iter, split - *iter );
				++++iter;
			}	
		}else{
			// Split horizontally
			iter = vert_blocks.begin();
			for( std::size_t i = vert_blocks.size(); i != 0; --i ){
				// Save 'full' block width
				split = *iter;

				// Divide in half and save value
				*iter /= 2;

				// Insert new value in front of saved
				// value; also account for case where
				// integer divison results in truncation;
				// also get iterator point to 2 before
				// next full interval
				iter = vert_blocks.insert( iter, split - *iter );
				++++iter;
			}	
		}
	}

	// Tracking variables
	std::size_t vert_offset = 0;
	std::size_t hori_offset = 0;

	// Variable to avoid need for calculating
	// n * m in each iteration of the following
	// loop
	const std::size_t full_size = n * m;

	// Iterators
	auto vert = vert_blocks.begin();
	auto hori = hori_blocks.begin();

	// Write the buffer
	while( vert_offset * hori_offset < full_size ){
		// Write buffer
		for( std::size_t i = 0; i < *vert; ++i ){
			for( std::size_t j = 0; j < *hori; ++j ){
				*(b + i + j*m + vert_offset + m*hori_offset) = *(a + j + i*n + hori_offset + n*vert_offset);
			}
		}

		// Move to next block
		hori_offset += *hori;
		++hori;

		// If end of horizontal line reached and
		// there are still vertical lines left,
		// reset horizontal offset and update
		// vertical offset
		if( hori_offset == n ){
			// Move to next line of blocks
			vert_offset += *vert;
			++vert;

			// If there are more lines below,
			// reset horizontal offset
			if( vert_offset < m ){
				hori_offset = 0;
				hori = hori_blocks.begin();
			}
		}
	}
}

template<class T>
void naive_matrix_transpose( const T* a, std::size_t m, std::size_t n, T* b ){
	for( std::size_t i = 0; i < m; ++i )
		for( std::size_t j = 0; j < n; ++j )
			*(b + i + j*m) = *(a + j + i*n);
}

struct mult_helper{
	static std::size_t m_orig {0};
	static std::size_t n_orig {0};
	static std::size_t p_orig {0};
	static bool valid {false};
}

template<class T>
void matrix_multiply( const T* a, const T* b, std::size_t m, std::size_t n, 
		std::size_t p, T* c ){
	if( !mult_helper::valid ){
		mult_helper::m_orig = m;
		mult_helper::n_orig = n;
		mult_helper::p_orig = p;
		mult_helper::valid = true;
	}

	if( m * n * p > MAX_BLOCK_SIZE ){
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
				}
			}
		}
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

