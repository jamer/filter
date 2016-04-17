// Copyright 2016 Paul Merrill

#include <cstdint>

#include "Fill.h"

PointFilter makeFillFilter(rgb color) {
    return [color](rgb pixel) {
        return color;
    };
}
