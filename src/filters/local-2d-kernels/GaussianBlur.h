// Copyright 2016 Paul Merrill

#ifndef SRC_FILTERS_LOCAL_2D_KERNELS_GAUSSIANBLUR_H_
#define SRC_FILTERS_LOCAL_2D_KERNELS_GAUSSIANBLUR_H_

#include "../Local2DFilter.h"

Kernel makeGaussianBlur(int radius, float sigma);

#endif  // SRC_FILTERS_LOCAL_2D_KERNELS_GAUSSIANBLUR_H_
