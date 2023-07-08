#define MAX_BLOCK_SIZE 64

#include <cstddef>
#include <list>
#include <cassert>

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
			for( std::size_t i = hori_blocks.size(); i; --i ){
				// Save 'full' block width
				split = *iter;

				// Divide in half and save value
				*iter = split/2;

				// Compute other half
				split = split - *iter;

				// Insert new value in front of saved
				// value; also account for case where
				// integer divison results in truncation;
				// also get iterator point to 2 before
				// next full interval
				iter = hori_blocks.insert( iter, split );
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

	std::size_t accum;
	for( hori = hori_blocks.begin(); hori != hori_blocks.end(); ++hori )
		accum += *hori;
	//assert( accum == n );
	std::cout << "Number of cols:\t\t" << n << '\n';
	std::cout << "Sum hori blocks:\t" << accum << '\n';
	accum = 0;
	hori = hori_blocks.begin();
	for( vert = vert_blocks.begin(); vert != vert_blocks.end(); ++vert )
		accum += *vert;
	//assert( accum == m );
	std::cout << "Number of rows:\t\t" << m << '\n';
	std::cout << "Sum vert blocks:\t" << accum << '\n';
	vert = vert_blocks.begin();

	// Write the buffer
	while( vert_offset * hori_offset < full_size ){
		// Write buffer
		for( int i = 0; i < *vert; ++i ){
			for( int j = 0; j < *hori; ++j ){
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

}

