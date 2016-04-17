// Copyright 2016 Paul Merrill

#include <boost/log/trivial.hpp>

#include "DirectoryTransform.h"
#include "Image8.h"

int main(int argc, char **argv) {
    BOOST_LOG_TRIVIAL(info) << "[main] Starting";

    directoryTransform("test-images", "output-images",
                       [](Image8&& img) { return std::move(img); });

    BOOST_LOG_TRIVIAL(info) << "[main] Finished";

    return 0;
}
