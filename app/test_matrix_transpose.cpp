#include <iostream>
#include "ra/matrix_transpose.hpp"
#include <cstddef>

template<class T>
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

	//printrix<int>(foo, 100, 100);
	//std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 100, 100, bar );
	//printrix<int>(bar, 100, 100);
	std::cout << "\n\n\n";

	//printrix<int>(foo, 50, 200);
	//std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 50, 200, bar );
	//printrix(bar, 50, 200 );
	std::cout << "\n\n\n";
	
	//printrix<int>(foo, 200, 50);
	//std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 200, 50, bar );
	//printrix(bar, 200, 50 );
	std::cout << "\n\n\n";

	//printrix<int>(foo, 1000, 10);
	//std::cout << "\n\n\n";
	ra::cache::matrix_transpose( foo, 1000, 10, bar );
	//printrix(bar, 1000, 10);
	std::cout << "\n\n\n";

	return 0;
}


