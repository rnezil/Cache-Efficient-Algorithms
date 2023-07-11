#include "ra/fft.hpp"
#include <iostream>

int main(){
	int bargain[69];

	for( unsigned i = 1; i < 0x1<<31; ++i ){
		std::cout << "for " << i << '\n';
		ra::cache::forward_fft(bargain, i);
	}

	return std::cout ? 0 : 1;
}
