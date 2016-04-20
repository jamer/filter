// Copyright 2016 Paul Merrill

#include "UnsharpenMask3.h"

static int dimension = 3;
static float values[] = {
         0.f, -1.f,  0.f,
        -1.f,  5.f, -1.f,
         0.f, -1.f,  0.f
};
const Kernel unsharpenMask3{dimension, &values[0]};
