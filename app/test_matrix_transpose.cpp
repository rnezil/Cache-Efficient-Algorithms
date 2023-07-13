#define CATCH_CONFIG_MAIN

#include <iostream>
#include "ra/matrix_transpose.hpp"
#include <cstddef>
#include <catch2/catch.hpp>

using namespace ra::cache;

TEMPLATE_TEST_CASE("Test matrix transpose", "[functions]",
		float, double, long double, int, unsigned,
		long int, long unsigned, long long int,
		long long unsigned, int64_t, int32_t, int16_t,
		int8_t, uint64_t, uint32_t, uint16_t, uint8_t){
	TestType a[10000];
	TestType b[10000];
	TestType c[10000];
	TestType d[10000];

	for( int i = 0; i < 100; ++i ){
		a[i] = i;
		b[i] = i;
		c[i] = i;
		d[i] = i;
	}

	matrix_transpose( a, 10, 10, b );
	matrix_transpose( a, 10, 10, a );
	naive_matrix_transpose( c, 10, 10, d );
	naive_matrix_transpose( c, 10, 10, c );

	for( int i = 0; i < 100; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 5, 20, b );
	matrix_transpose( a, 5, 20, a );
	naive_matrix_transpose( c, 5, 20, d );
	naive_matrix_transpose( c, 5, 20, c );

	for( int i = 0; i < 100; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 20, 5, b );
	matrix_transpose( a, 20, 5, a );
	naive_matrix_transpose( c, 20, 5, d );
	naive_matrix_transpose( c, 20, 5, c );

	for( int i = 0; i < 100; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}
	
	for( int i = 0; i < 10000; ++i ){
		a[i] = i;
		b[i] = i;
		c[i] = i;
		d[i] = i;
	}

	matrix_transpose( a, 100, 100, b );
	matrix_transpose( a, 100, 100, a );
	naive_matrix_transpose( c, 100, 100, d );
	naive_matrix_transpose( c, 100, 100, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 50, 200, b );
	matrix_transpose( a, 50, 200, a );
	naive_matrix_transpose( c, 50, 200, d );
	naive_matrix_transpose( c, 50, 200, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 200, 50, b );
	matrix_transpose( a, 200, 50, a );
	naive_matrix_transpose( c, 200, 50, d );
	naive_matrix_transpose( c, 200, 50, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 1000, 10, b );
	matrix_transpose( a, 1000, 10, a );
	naive_matrix_transpose( c, 1000, 10, d );
	naive_matrix_transpose( c, 1000, 10, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 10, 1000, b );
	matrix_transpose( a, 10, 1000, a );
	naive_matrix_transpose( c, 10, 1000, d );
	naive_matrix_transpose( c, 10, 1000, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 125, 80, b );
	matrix_transpose( a, 125, 80, a );
	naive_matrix_transpose( c, 125, 80, d );
	naive_matrix_transpose( c, 125, 80, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

	matrix_transpose( a, 80, 125, b );
	matrix_transpose( a, 80, 125, a );
	naive_matrix_transpose( c, 80, 125, d );
	naive_matrix_transpose( c, 80, 125, c );

	for( int i = 0; i < 10000; ++i ){
		CHECK( a[i] == b[i] );
		CHECK( a[i] == c[i] );
		CHECK( a[i] == d[i] );
	}

}

/*template<class T>
void printrix( const T* mat, std::size_t m, std::size_t n ){
	for( std::size_t i = 0; i < m; ++i ){
		for( std::size_t j = 0; j < n; ++j ){
			std::cout << mat[j + i*n];
			if( mat[j + i*n] < 10 )
				std::cout << "    ";
			else if( mat[j + i*n] < 100 )
				std::cout << "   ";
			else if( mat[j + i*n] < 1000 )
				std::cout << "  ";
			else
				std::cout << " ";
		}
		std::cout << '\n';
	}
}

int main(){
	int barry[100];
	for( int i = 0; i < 100; ++i )
		barry[i] = i;

	printrix<int>(barry, 10, 10);
	std::cout << "\n\n\n";

	int yrrab[100];
	ra::cache::matrix_transpose( barry, 10, 10, yrrab );

	printrix<int>(yrrab, 10, 10);
	std::cout << "\n\n\n";

	int foo[10000];
	int bar[10000];

	for( int i = 0; i < 10000; ++i )
		foo[i] = i;

	printrix<int>(foo, 100, 100);
	std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 100, 100, bar );
	printrix<int>(bar, 100, 100);
	std::cout << "\n\n\n";

	printrix<int>(foo, 50, 200);
	std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 50, 200, bar );
	printrix(bar, 200, 50 );
	std::cout << "\n\n\n";
	
	printrix<int>(foo, 200, 50);
	std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 200, 50, bar );
	printrix(bar, 50, 200 );
	std::cout << "\n\n\n";

	printrix<int>(foo, 1000, 10);
	std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 1000, 10, foo );
	printrix(foo, 10, 1000);
	std::cout << "\n\n\n";

	return 0;
}*/

