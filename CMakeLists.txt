cmake_minimum_required(VERSION 3.3)
project(filters)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -mtune=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp ImageI8.cpp ImageI8.h stb_image.h stb_image_impl.cpp stb_image_write_impl.cpp)
add_executable(filters ${SOURCE_FILES})
