// Copyright 2016 Paul Merrill

#include <boost/log/trivial.hpp>

#include <cassert>
#include <string>

#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "Image8.h"

Image8::Image8() {
    w = 0;
    h = 0;
    pixels = nullptr;
}

Image8::Image8(string filename) {
    int width, height, components;
    stbi_uc *data;

    data = stbi_load(filename.c_str(), &width, &height, &components, 3);

    if (components != 3) {
        BOOST_LOG_TRIVIAL(info) << "[Image8] Number of image components not 3";
    }

    assert(width >= 0 && height >= 0 && data);

    w = static_cast<uint32_t>(width);
    h = static_cast<uint32_t>(height);
    pixels = reinterpret_cast<rgb *>(data);
}

Image8::Image8(Image8&& other) {
    *this = std::move(other);
}

Image8::Image8(const Image8& other) {
    *this = other;
}

Image8& Image8::operator=(Image8&& other) {
    w = other.w;
    h = other.h;
    pixels = other.pixels;

    other.w = 0;
    other.h = 0;
    other.pixels = nullptr;

    return *this;
}

Image8& Image8::operator=(const Image8& other) {
    BOOST_LOG_TRIVIAL(warning) << "[Image8] Copying an image";

    w = other.w;
    h = other.h;

    size_t dataLength = w * h * sizeof(rgb);
    pixels = reinterpret_cast<rgb *>(malloc(dataLength));
    memcpy(pixels, other.pixels, dataLength);

    return *this;
}

Image8::~Image8() {
    if (pixels) {
        stbi_uc *data = reinterpret_cast<stbi_uc *>(pixels);
        stbi_image_free(data);
        pixels = nullptr;
    }
}

void Image8::write(string filename) {
    assert(pixels);

    stbi_uc *data = reinterpret_cast<stbi_uc *>(pixels);
    int ok = stbi_write_bmp(filename.c_str(), w, h, 3, data);
    assert(ok);
}
