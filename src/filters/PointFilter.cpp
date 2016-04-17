// Copyright 2016 Paul Merrill

#include <cstdint>

#include "PointFilter.h"

Image8 runPointFilter(Image8 image, PointFilter filter) {
    uint32_t w = image.w;
    uint32_t h = image.h;
    rgb *pixels = image.pixels;

    for (uint32_t y = 0; y < h; y++) {
        for (uint32_t x = 0; x < w; x++) {
            size_t offset = y * w + x;
            pixels[offset] = filter(pixels[offset]);
        }
    }

    return std::move(image);
}
