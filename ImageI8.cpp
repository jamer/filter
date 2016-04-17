#include <cassert>

#include "ImageI8.h"
#include "stb_image.h"

ImageI8::ImageI8(const char *filename)
{
    int n;
    unsigned char *data = stbi_load(filename, &w, &h, &n, 3);

    assert(data);
}

ImageI8::~ImageI8()
{
    stbi_image_free(data);
}

void ImageI8::save(const char *filename)
{
}
