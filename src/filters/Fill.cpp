// Copyright 2016 Paul Merrill

#include <cstdint>

#include "Fill.h"

Image8 fillImageWithSolidColor(Image8 image, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t w = image.w;
    uint32_t h = image.h;
    uint8_t *data = image.data;

    for (uint32_t y = 0; y < h; y++) {
        for (uint32_t x = 0; x < w; x++) {
            size_t offset = y * w * 3 + x * 3;
            data[offset + 0] = r;
            data[offset + 1] = g;
            data[offset + 2] = b;
        }
    }

    return std::move(image);
}
