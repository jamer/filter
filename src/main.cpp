// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "Log.h"
#include "filters/point-filters/ColorOverlay.h"

using std::move;

int main(int argc, char **argv) {
    log("[main] Starting");

    directoryTransform("test-images", "output-images",
                       [](Image8 img) {
                           rgb red = {255, 0, 0};
                           PointFilter overlay = makeColorOverlayFilter(red,
                                                                        0.4);
                           ParallelismPolicy policy = PP_OPEN_MP;
                           return runPointFilter(move(img), overlay, policy);
                       });

    log("[main] Finished");

    return 0;
}
