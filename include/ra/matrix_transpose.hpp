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
	std::size_t split;
	
	// Loop for getting block sizes
	while( *vert_blocks.begin() * *hori_blocks.begin() > MAX_BLOCK_SIZE ){
		// # cols > # rows
		if( *hori_blocks.begin() > *vert_blocks.begin() ){
			// Split vertically
			iter = hori_blocks.begin();
			for( std::size_t i = hori_blocks.size(); i != 0; --i ){
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
	register std::size_t vert_offset = 0;
	register std::size_t hori_offset = 0;

	// Variable to avoid need for calculating
	// n * m in each iteration of the following
	// loop
	register const std::size_t full_size = n * m;

	// Iterators
	auto vert = vert_blocks.begin();
	auto hori = hori_blocks.begin();

	// Create local variables to improve loop
	// performance
	register std::size_t vert_span;
	register std::size_t hori_span;

	// Write the buffer
	while( vert_offset * hori_offset < full_size ){
		vert_span = *vert;
		hori_span = *hori;
		for( int i = 0; i < vert_span; ++i ){
			for( int j = 0; j < hori_span; ++j ){
				*(b + i + j*m + vert_offset + m*hori_offset) = *(a + j + i*n + hori_offset + n*vert_offset);
			}
		}
		hori_accum += *hori;
		vert_accum += *vert;
		++hori;
		++vert
	}
}

}

