// Copyright 2016 Paul Merrill

#include "Color.h"

rgb rgb::operator*(float f) const {
    rgb pixel = *this;
    pixel.r *= f;
    pixel.g *= f;
    pixel.b *= f;
    return pixel;
}

rgb rgb::operator+(const rgb& other) const {
    rgb pixel = *this;
    pixel.r += other.r;
    pixel.g += other.g;
    pixel.b += other.b;
    return pixel;
}

// From https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both#6930407
hsv rgb::toHSV()
{
    hsv hsvPixel;
    uint8_t rgbMin, rgbMax;

    rgbMin = r < g ? (r < b ? r : b) : (g < b ? g : b);
    rgbMax = r > g ? (r > b ? r : b) : (g > b ? g : b);

    hsvPixel.v = rgbMax;
    if (hsvPixel.v == 0) {
        hsvPixel.h = 0;
        hsvPixel.s = 0;
        return hsvPixel;
    }

    hsvPixel.s = 255 * uint32_t(rgbMax - rgbMin) / hsvPixel.v;
    if (hsvPixel.s == 0) {
        hsvPixel.h = 0;
        return hsvPixel;
    }

    if (rgbMax == r) {
        hsvPixel.h = 0 + 43 * (g - b) / (rgbMax - rgbMin);
    } else if (rgbMax == g) {
        hsvPixel.h = 85 + 43 * (b - r) / (rgbMax - rgbMin);
    } else {
        hsvPixel.h = 171 + 43 * (r - g) / (rgbMax - rgbMin);
    }

    return hsvPixel;
}

// From https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both#6930407
rgb hsv::toRGB()
{
    rgb rgbPixel;
    uint8_t region, remainder, p, q, t;

    if (s == 0) {
        rgbPixel.r = v;
        rgbPixel.g = v;
        rgbPixel.b = v;
        return rgbPixel;
    }

    region = h / 43;
    remainder = (h - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
    case 0:
        rgbPixel.r = v;
        rgbPixel.g = t;
        rgbPixel.b = p;
        break;
    case 1:
        rgbPixel.r = q;
        rgbPixel.g = v;
        rgbPixel.b = p;
        break;
    case 2:
        rgbPixel.r = p;
        rgbPixel.g = v;
        rgbPixel.b = t;
        break;
    case 3:
        rgbPixel.r = p;
        rgbPixel.g = q;
        rgbPixel.b = v;
        break;
    case 4:
        rgbPixel.r = t;
        rgbPixel.g = p;
        rgbPixel.b = v;
        break;
    default:
        rgbPixel.r = v;
        rgbPixel.g = p;
        rgbPixel.b = q;
        break;
    }

    return rgbPixel;
}
