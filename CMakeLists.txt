cmake_minimum_required(VERSION 3.17)
project(NonEuclidean)
set(CMAKE_CXX_STANDARD 14)

file( GLOB SOURCES src/*.cpp )
file( GLOB HEADERS src/*.cpp )
add_executable(NonEuclidean ${SOURCES} ${HEADERS})
target_include_directories(NonEuclidean PRIVATE include)

