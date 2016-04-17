#ifndef FILTERS_IMAGEI8_H
#define FILTERS_IMAGEI8_H

class ImageI8 {
public:
    ImageI8(const char *filename);
    ~ImageI8();

    void write(const char *filename);

private:
    int w, h;
    unsigned char *data;
};

#endif //FILTERS_IMAGEI8_H
