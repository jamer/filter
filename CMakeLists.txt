cmake_minimum_required(VERSION 3.3)
project(filters)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -mtune=native")

set(SOURCE_FILES main.cpp Image.cpp Image.h stb_image.h stb_image_impl.cpp)
add_executable(filters ${SOURCE_FILES})
