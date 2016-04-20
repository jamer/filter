// Copyright 2016 Paul Merrill

#include <cstdint>

#include "Grayscale.h"

PointFilter makeGrayscaleFilter() {
    return [](rgb pixel) {
        uint8_t gray = static_cast<uint8_t>(
                pixel.r * 0.299 + pixel.g * 0.587 + pixel.b * 0.114);
        return rgb{gray, gray, gray};
    };
}
