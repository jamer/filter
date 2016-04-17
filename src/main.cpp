// Copyright 2016 Paul Merrill

#include <boost/log/trivial.hpp>
#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "filters/Fill.h"

using std::move;

int main(int argc, char **argv) {
    BOOST_LOG_TRIVIAL(info) << "[main] Starting";

    directoryTransform("test-images", "output-images",
                       [](Image8 img) {
                           return fillImageWithSolidColor(move(img), 255, 0, 0);
                       });

    BOOST_LOG_TRIVIAL(info) << "[main] Finished";

    return 0;
}
