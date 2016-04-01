//
// Created by Paul Merrill on 3/31/16.
//

#include <cassert>

#include "Image.h"
#include "stb_image.h"

Image::Image(const char *filename)
{
    int n;
    unsigned char *data = stbi_load(filename, &w, &h, &n, 0);

    assert(data);
    assert(n == 3);
}

Image::~Image()
{
    stbi_image_free(data);
}
