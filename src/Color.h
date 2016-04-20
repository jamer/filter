// Copyright 2016 Paul Merrill

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

#include <cstdint>

struct hsv;

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    rgb operator*(float f) const;
    rgb operator+(const rgb& other) const;

    hsv toHSV();
};

struct hsv {
    uint8_t h;
    uint8_t s;
    uint8_t v;

    rgb toRGB();
};

#endif  // SRC_COLOR_H_
