// Copyright 2016 Paul Merrill

#include <utility>
#include <boost/log/trivial.hpp>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "filters/point-filters/ColorOverlay.h"

using std::move;

int main(int argc, char **argv) {
    BOOST_LOG_TRIVIAL(info) << "[main] Starting";

    directoryTransform("test-images", "output-images",
                       [](Image8 img) {
                           rgb red = {255, 0, 0};
                           PointFilter overlay = makeColorOverlayFilter(red,
                                                                        0.4);
                           ParallelismPolicy policy = PP_MULTI_THREADED;
                           return runPointFilter(move(img), overlay, policy);
                       });

    BOOST_LOG_TRIVIAL(info) << "[main] Finished";

    return 0;
}
