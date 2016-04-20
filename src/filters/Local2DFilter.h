// Copyright 2016 Paul Merrill

#ifndef SRC_FILTERS_LOCAL2DFILTER_H_
#define SRC_FILTERS_LOCAL2DFILTER_H_

#include "../Image8.h"
#include "../ParallelismPolicy.h"

// A square matrix.
struct Kernel {
    int dimension;
    float *values;

    float value(int x, int y) const {
        return values[y * dimension + x];
    }
};

enum EdgePolicy {
    EP_CLAMP,
    EP_WRAP,
    EP_CLIP
};

Image8 runLocal2DFilter(Image8 image, Kernel kernel,
                        ParallelismPolicy parallelismPolicy,
                        EdgePolicy edgePolicy);

#endif  // SRC_FILTERS_LOCAL2DFILTER_H_
