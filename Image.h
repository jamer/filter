//
// Created by Paul Merrill on 3/31/16.
//

#ifndef FILTERS_IMAGE_H
#define FILTERS_IMAGE_H

class Image {
public:
    Image(const char *filename);
    ~Image();

private:
    int w, h;
    unsigned char *data;
};

#endif //FILTERS_IMAGE_H
