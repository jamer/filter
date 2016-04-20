// Copyright 2016 Paul Merrill

#ifndef SRC_FILTERS_POINTFILTER_H_
#define SRC_FILTERS_POINTFILTER_H_

#include <cstdint>
#include <functional>

#include "../Image8.h"

enum ParallelismPolicy {
    PP_SINGLE_THREADED,
    PP_MULTI_THREADED,
    PP_OPEN_MP
};

typedef std::function<rgb(rgb)> PointFilter;

Image8 runPointFilter(Image8 image, PointFilter filter,
                      ParallelismPolicy policy);

#endif  // SRC_FILTERS_POINTFILTER_H_
