// Copyright 2016 Paul Merrill

#include <cmath>

#include "GaussianBlur.h"

static void normalize(Kernel kernel) {
    int numElements = kernel.dimension * kernel.dimension;

    float sum = 0.f;
    for (int i = 0; i < numElements; i++) {
        sum += kernel.values[i];
    }

    for (int i = 0; i < numElements; i++) {
        kernel.values[i] /= sum;
    }
}

static float gauss(float x, float y, float sigma) {
    float exponent = -(x * x + y * y) / (2 * sigma * sigma);
    return exp(exponent);
}

Kernel makeGaussianBlur(int radius, float sigma) {
    int dimension = radius * 2 + 1;
    int numElements = dimension * dimension;

    float *values = new float[numElements];
    for (int i = 0; i < numElements; i++) {
        int x = i % dimension - radius;
        int y = i / dimension - radius;
        values[i] = gauss(x, y, sigma);
    }

    Kernel kernel{dimension, values};
    normalize(kernel);
    return kernel;
}
