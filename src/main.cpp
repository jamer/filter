// Copyright 2016 Paul Merrill

#include <utility>
#include <boost/log/trivial.hpp>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "filters/point-filters/Fill.h"

using std::move;

int main(int argc, char **argv) {
    BOOST_LOG_TRIVIAL(info) << "[main] Starting";

    directoryTransform("test-images", "output-images",
                       [](Image8 img) {
                           rgb red = {255, 0, 0};
                           PointFilter fill = makeFillFilter(red);
                           ParallelismPolicy policy = PP_MULTI_THREADED;
                           return runPointFilter(move(img), fill, policy);
                       });

    BOOST_LOG_TRIVIAL(info) << "[main] Finished";

    return 0;
}
