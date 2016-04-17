// Copyright 2016 Paul Merrill

#include <cstdint>

#include "ColorOverlay.h"

PointFilter makeColorOverlayFilter(rgb color, float alpha) {
    float alphaCompliment = 1 - alpha;
    return [color, alpha, alphaCompliment](rgb pixel) {
        return pixel * alphaCompliment + color * alpha;
    };
}
