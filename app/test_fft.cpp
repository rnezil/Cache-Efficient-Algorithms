#define FFT_ORDER 4

#include "ra/fft.hpp"
#include <iostream>

using namespace ra::cache;

int main(){
	std::cout << "Begin fftesting.\n\n";
	for( int i = 0; i < FFT_ORDER ; ++i ){
		std::cout << (1<<i) << " point DFT.\n";
		std::complex<float> input[1<<i];
		for( int j = 0; j < 1<<i; ++j )
			input[j] = (float)j;
		for( int j = 0; j < 1<<i; ++j )
			std::cout << input[j] << ' ';
		std::cout << '\n';
		forward_fft(input, 1<<i);
		for( int j = 0; j < 1<<i; ++j )
			std::cout << input[j] << ' ';
		std::cout << '\n' << '\n';
	}
	return std::cout ? 0 : 1;
}
