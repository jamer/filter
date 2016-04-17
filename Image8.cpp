// Copyright 2016 Paul Merrill

#include <cassert>
#include <string>

#include "./stb_image.h"
#include "./stb_image_write.h"

#include "Image8.h"

Image8::Image8(string filename)
{
    int n;
    data = stbi_load(filename.c_str(), &w, &h, &n, 3);

    assert(data);
}

Image8::~Image8()
{
    stbi_image_free(data);
}

void Image8::write(string filename)
{
    int ok = stbi_write_bmp(filename.c_str(), w, h, 3, data);
    assert(ok);
}
