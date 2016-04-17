// Copyright 2016 Paul Merrill

#include <cstdint>

#include "Fill.h"

PointFilter makeFillFilter(rgb color) {
    return [](rgb pixel) {
        return color;
    };
}

