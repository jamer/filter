// Copyright 2016 Paul Merrill

#ifndef SRC_IMAGE8_H_
#define SRC_IMAGE8_H_

#include <cstdint>
#include <string>

#include "Color.h"

using std::string;

class Image8 {
 public:
    Image8();
    Image8(uint32_t w, uint32_t h);
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
