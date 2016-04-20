// Copyright 2016 Paul Merrill

#include "Saturation.h"

PointFilter makeAlterSaturationFilter(float multiple) {
    return [multiple](rgb rgbPixel) {
        hsv hsvPixel = rgbPixel.toHSV();
        hsvPixel.s *= multiple;
        return hsvPixel.toRGB();
    };
}
