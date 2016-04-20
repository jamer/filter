// Copyright 2016 Paul Merrill

#include "BoxBlur3.h"

static float values[] = {
    1.0f/9, 1.0f/9, 1.0f/9,
    1.0f/9, 1.0f/9, 1.0f/9,
    1.0f/9, 1.0f/9, 1.0f/9
};

Kernel boxBlur3{3, values};
