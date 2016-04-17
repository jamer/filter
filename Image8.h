// Copyright 2016 Paul Merrill

#ifndef IMAGE8_H_
#define IMAGE8_H_

#include <string>

using std::string;

class Image8 {
 public:
    explicit Image8(string filename);
    ~Image8();

    void write(string filename);

 private:
    int w, h;
    unsigned char *data;
};

#endif  // IMAGE8_H_
