#define MAX_TRANSPOSE_BLOCK_SIZE 512
// A max block size of 512 was found to yield the
// best average performance on the Intel i7-9700k
// processor.

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

struct trans_helper{
	std::size_t m;
	std::size_t n;
	bool valid;
};

static trans_helper orig;

template<class T>
void matrix_transpose( const T* a, std::size_t m, std::size_t n, T* b ){
	if( a == b ){
		T* c = new T[m*n];
		matrix_transpose( a, m, n, c );
		std::size_t full_size = m*n;
		for( std::size_t i = 0; i < full_size; ++i )
			*(b + i) = *(c + i);
		delete[] c;
	}else{
	bool last_pass {false};
	if( !orig.valid ){
		// Store original matrix dimensions
		orig.n = n;
		orig.m = m;
		orig.valid = true;
		last_pass = true;
	}

	if( m * n > MAX_TRANSPOSE_BLOCK_SIZE ){
		// Split the matrix
		if( m > n ){
			// Split horizontally
			std::size_t split = m/2;

			// Top half
			matrix_transpose( a, split, n, b );

			// Bottom half
			matrix_transpose( a + split*orig.n, m - split, n, b + split );
		}else{
			// Split vertically
			std::size_t split = n/2;

			// Left half
			matrix_transpose( a, m, split, b );

			// Bottom Half
			matrix_transpose( a + split, m, n - split, b + split*orig.m );
		}
	}else{
		// Compute the transposition and write the buffer
		for( std::size_t i = 0; i < m; ++i )
			for( std::size_t j = 0; j < n; ++j )
				*(b + i + j*orig.m) = *(a + j + i*orig.n);
	}

	if( last_pass )
		orig.valid = false;
	}
}

template<class T>
void naive_matrix_transpose( const T* a, std::size_t m, std::size_t n, T* b ){
	if( a == b ){
		T* c = new T[m*n];

		for( std::size_t i = 0; i < m; ++i )
			for( std::size_t j = 0; j < n; ++j )
				*(c + i + j*m) = *(a + j + i*n);

		for( std::size_t i = 0; i < n*m; ++i )
			*(b + i) = *(c + i);

		delete[] c;
	}else{
		for( std::size_t i = 0; i < m; ++i )
			for( std::size_t j = 0; j < n; ++j )
				*(b + i + j*m) = *(a + j + i*n);
	}
}

}

/*
 * Original version of matrix_transpose below.
 * This version was found to perform worse than
 * the naive version for ALL values of
 * MAX_TRANSPOSE_BLOCK_SIZE, and thus was not
 * deemed acceptable. The version at the top of
 * this file was written to replace it.
 */


/*
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
	while( *vert_blocks.begin() * *hori_blocks.begin() > MAX_TRANSPOSE_BLOCK_SIZE ){
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

	if( a == b ){
		// Allocate auxiliary buffer
		T* c = new T[full_size];
		
		// Write auxiliary buffer
		while( vert_offset * hori_offset < full_size ){
			for( std::size_t i = 0; i < *vert; ++i ){
				for( std::size_t j = 0; j < *hori; ++j ){
					*(c + i + j*m + vert_offset + m*hori_offset) = *(a + j + i*n + hori_offset + n*vert_offset);
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

		// Write the actual buffer
		for( std::size_t i = 0; i < full_size; ++i )
			*(b + i) = *(c + i);

		// De-allocate auxiliary buffer
		delete[] c;
	}else{
		// Write the buffer
		while( vert_offset * hori_offset < full_size ){
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
}
*/

