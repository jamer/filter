// Copyright 2016 Paul Merrill

#include "BoxBlur.h"

Kernel makeBoxBlur(int radius) {
    int dimension = radius * 2 + 1;
    int numElements = dimension * dimension;
    float coefficient = 1.0 / numElements;
    float *values = new float[numElements];
    for (int i = 0; i < numElements; i++) {
        values[i] = coefficient;
    }
    return Kernel{dimension, values};
}
