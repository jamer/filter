#include <cassert>

#include "ImageI8.h"
#include "stb_image.h"
#include "stb_image_write.h"

ImageI8::ImageI8(const char *filename)
{
    int n;
    data = stbi_load(filename, &w, &h, &n, 3);

    assert(data);
}

ImageI8::~ImageI8()
{
    stbi_image_free(data);
}

void ImageI8::write(const char *filename)
{
    int ok = stbi_write_bmp(filename, w, h, 3, data);
    assert(ok);
}
