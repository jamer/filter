// Copyright 2016 Paul Merrill

#include <algorithm>
#include <cmath>
#include <cstdint>

#include "Sepia.h"

using std::max;
using std::min;

static double gammaAdjustment = 1.6;
static uint8_t gammaAdjustLookup[256];

static void makeGammaAdjustLookup() {
    for (int in = 0; in < 256; in++) {
        double out = in;
        out /= 255.0;
        out = pow(out, 1.0 / gammaAdjustment);
        out *= 255.0;
        out = min(out, 255.0);
        out = max(out, 0.0);
        gammaAdjustLookup[in] = static_cast<uint8_t>(out);
    }
}

PointFilter makeSepiaFilter() {
    makeGammaAdjustLookup();
    return [](rgb pixel) {
        float r = pixel.r;
        float g = pixel.g;
        float b = pixel.b;

        float gray = (pixel.r + pixel.g + pixel.b) / 3;

        // Reduce saturation.
        r = (r + gray) / 2;
        g = (g + gray) / 2;
        b = (b + gray) / 2;

        // Mix and match color channels to create the "sepia" coloring. (This
        // has the unintended side-effect of darkening the pixels as well.)
        r = (g * b) / 255;
        g = (b * r) / 255;
        b = (r * g) / 255;

        // To fix the darkening, multiply everything by 1.75. Using
        // multiplication instead of addition means that bright colors become
        // brighter, but dark colors stay dark.
        r *= 1.75;
        g *= 1.75;
        b *= 1.75;

        uint8_t r8 = static_cast<uint8_t>(min(r, 255.f));
        uint8_t g8 = static_cast<uint8_t>(min(g, 255.f));
        uint8_t b8 = static_cast<uint8_t>(min(b, 255.f));

        // Apply a gamma ramp to the colors.
        r8 = gammaAdjustLookup[r8];
        g8 = gammaAdjustLookup[g8];
        b8 = gammaAdjustLookup[b8];

        return rgb{r8, g8, b8};
    };
}
