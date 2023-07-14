#define CATCH_CONFIG_MAIN

#include "ra/matrix_multiply.hpp"
#include <catch2/catch.hpp>
#include <cstddef>

using namespace ra::cache;

TEMPLATE_TEST_CASE("Test matrix multiply", "[functions]",
		int, unsigned, long int, long unsigned, double,
		long double, float, long long int, long long unsigned,
		int64_t, int32_t, uint64_t, uint32_t){
	TestType a[10000];
	TestType b[10000];
	TestType c[40000];
	TestType d[40000];

	for( int i = 0; i < 100; ++i ){
		a[i] = i;
		b[i] = i + 69;
	}

	matrix_multiply( a, b, 10, 10, 10, c );
	naive_matrix_multiply( a, b, 10, 10, 10, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 20, 5, 20, c );
	naive_matrix_multiply( a, b, 20, 5, 20, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 5, 20, 5, c );
	naive_matrix_multiply( a, b, 5, 20, 5, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 2, 50, 2, c );
	naive_matrix_multiply( a, b, 2, 50, 2, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 50, 2, 50, c );
	naive_matrix_multiply( a, b, 50, 2, 50, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 100, 1, 100, c );
	naive_matrix_multiply( a, b, 100, 1, 100, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 1, 100, 1, c );
	naive_matrix_multiply( a, b, 1, 100, 1, d );

	for( int i = 0; i < 100; ++i )
		CHECK( c[i] == d[i] );

	for( int i = 0; i < 10000; ++i ){
		a[i] = i + 1;
		b[i] = a[i] * 8;
	}

	matrix_multiply( a, b, 100, 100, 100, c );
	naive_matrix_multiply( a, b, 100, 100, 100, d );

	for( int i = 0; i < 10000; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 50, 200, 50, c );
	naive_matrix_multiply( a, b, 50, 200, 50, d );

	for( int i = 0; i < 10000; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 200, 50, 200, c );
	naive_matrix_multiply( a, b, 200, 50, 200, d );

	for( int i = 0; i < 10000; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 10, 1000, 10, c );
	naive_matrix_multiply( a, b, 10, 1000, 10, d );

	for( int i = 0; i < 10000; ++i )
		CHECK( c[i] == d[i] );

	matrix_multiply( a, b, 1, 10000, 1, c );
	naive_matrix_multiply( a, b, 1, 10000, 1, d );

	for( int i = 0; i < 10000; ++i )
		CHECK( c[i] == d[i] );
}

/*template<class T>
void print( const T* a, std::size_t a_rows, std::size_t a_cols,
		const T* b, std::size_t b_rows, std::size_t b_cols,
		const T* c ){
	std::cout << "\n\n";
	for( std::size_t i = 0; i < a_rows; ++i ){
		for( std::size_t j = 0; j < a_cols; ++j ){
			std::cout << *(a + j + i*a_cols) << '\t';
		}
		std::cout << "\n\n";
	}
	std::cout << "\n\t\t multiplied by \n\n\n";
	for( std::size_t i = 0; i < b_rows; ++i ){
		for( std::size_t j = 0; j < b_cols; ++j ){
			std::cout << *(b + j + i*b_cols) << '\t';
		}
		std::cout << "\n\n";
	}
	std::cout << "\n\t\t equals \n\n\n";
	for( std::size_t i = 0; i < a_rows; ++i ){
		for( std::size_t j = 0; j < b_cols; ++j ){
			std::cout << *(c + j + i*b_cols) << '\t';
		}
		std::cout << "\n\n";
	}
	std::cout << "\n\n\n";
}

int main(){
	int easy[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int peasy[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
	int lemonsqueezy[9];
	ra::cache::matrix_multiply( easy, peasy, 3, 3, 3, lemonsqueezy );
	print(easy, 3, 3, peasy, 3, 3, lemonsqueezy);

	int a[100];
	int b[100];
	int c[100];

	int tmp = 0;

	for( int i = 0; i < 100; ++i ){
		a[i] = i+1;
		if( i % 10 == tmp ){
			b[i] = 1;
			++tmp;
		}else{
			b[i] = 0;
		}
	}

	ra::cache::matrix_multiply( a, b, 10, 10, 10, c );
	print( a, 10, 10, b, 10, 10, c );

	//ra::cache::naive_matrix_multiply( a, b, 10, 10, 10, c );
	//print( a, 10, 10, b, 10, 10, c );

	return 0;
}*/

