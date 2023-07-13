#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(cache LANGUAGES CXX)

#Find Catch2 package
find_package(Catch2 REQUIRED)

#Include Catch header
include(Catch)

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add executable target
add_executable(test_matrix_transpose ${CMAKE_CURRENT_SOURCE_DIR}/app/test_matrix_transpose.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/matrix_transpose.hpp)
add_executable(test_matrix_multiply ${CMAKE_CURRENT_SOURCE_DIR}/app/test_matrix_multiply.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/matrix_multiply.hpp)
add_executable(test_fft ${CMAKE_CURRENT_SOURCE_DIR}/app/test_fft.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/matrix_transpose.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/ra/fft.hpp)

#Force C++20 for FFT otherwise std::numbers is unusable
target_compile_options(test_fft PUBLIC -std=c++20)

