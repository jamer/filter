cmake_minimum_required(VERSION 3.3)
project(filters)

set(CMAKE_BUILD_TYPE Debug)

set(CXX_VERSION_FLAGS "-std=c++1z")
set(DYNAMICALLY_LINK_BOOST_LOG "-DBOOST_LOG_DYN_LINK")
set(NATIVE_OPTIMIZATIONS_FLAGS "-march=native -mtune=native")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_VERSION_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DYNAMICALLY_LINK_BOOST_LOG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${NATIVE_OPTIMIZATIONS_FLAGS}")

set(SOURCE_FILES
  src/DirectoryTransform.cpp
  src/filters/Fill.cpp
  src/Image8.cpp
  src/main.cpp
  src/stb_image/stb_image_impl.cpp
  src/stb_image/stb_image_write_impl.cpp
)
add_executable(filters ${SOURCE_FILES})

target_link_libraries(filters "boost_filesystem")
target_link_libraries(filters "boost_log-mt")
target_link_libraries(filters "boost_system")
