// Copyright 2016 Paul Merrill

#ifndef SRC_IMAGE8_H_
#define SRC_IMAGE8_H_

#include <cstdint>
#include <memory>
#include <string>

using std::string;
using std::unique_ptr;

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    rgb operator*(float f) const {
        rgb pixel = *this;
        pixel.r *= f;
        pixel.g *= f;
        pixel.b *= f;
        return pixel;
    }

    rgb operator+(const rgb& other) const {
        rgb pixel = *this;
        pixel.r += other.r;
        pixel.g += other.g;
        pixel.b += other.b;
        return pixel;
    }
};

class Image8 {
 public:
    Image8();
    explicit Image8(string filename);
    Image8(Image8&& other);
    Image8(const Image8& other);
    ~Image8();

    Image8& operator=(Image8&& other);
    Image8& operator=(const Image8& other);

    void write(string filename);

    uint32_t w, h;
    rgb *pixels;
};

#endif  // SRC_IMAGE8_H_
