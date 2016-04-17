// Copyright 2016 Paul Merrill

#include <boost/log/trivial.hpp>

#include "Image8.h"

int main(int argc, char **argv) {
    BOOST_LOG_TRIVIAL(info) << "Starting";

    Image8 i(argv[1]);
    i.write("out.bmp");

    return 0;
}
