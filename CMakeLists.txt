cmake_minimum_required(VERSION 3.3)
project(filters)

# Set to your location for mpic++
#set(CMAKE_CXX_COMPILER "mpic++")

set(CXX_VERSION_FLAGS "-std=c++1z")
set(OPENMP_FLAGS "-fopenmp")
set(NATIVE_OPTIMIZATIONS_FLAGS "-march=native -mtune=native")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_VERSION_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENMP_FLAGS}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${NATIVE_OPTIMIZATIONS_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OPENMP_FLAGS}")

set(SOURCE_FILES
  src/Color.cpp
  src/DirectoryTransform.cpp
  src/filters/Local2DFilter.cpp
  src/filters/local-2d-kernels/BoxBlur.cpp
  src/filters/local-2d-kernels/GaussianBlur.cpp
  src/filters/local-2d-kernels/UnsharpenMask3.cpp
  src/filters/PointFilter.cpp
  src/filters/point-filters/ColorOverlay.cpp
  src/filters/point-filters/Fill.cpp
  src/filters/point-filters/Grayscale.cpp
  src/filters/point-filters/Saturation.cpp
  src/filters/point-filters/Sepia.cpp
  src/Image8.cpp
  src/Log.cpp
  src/main.cpp
  src/MPIHelper.cpp
  src/stb_image/stb_image_impl.cpp
  src/stb_image/stb_image_write_impl.cpp
)
add_executable(filters ${SOURCE_FILES})

target_link_libraries(filters "boost_filesystem")
target_link_libraries(filters "boost_system")
