// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "Log.h"
#include "filters/Local2DFilter.h"
#include "filters/local-2d-kernels/BoxBlur3.h"
#include "filters/PointFilter.h"
#include "filters/point-filters/ColorOverlay.h"
#include "filters/point-filters/Saturation.h"

using std::move;

Image8 appleFrostedGlass(Image8 img) {
    ParallelismPolicy policy = PP_CXX_STD_THREAD;

    /*
    PointFilter desaturate = makeAlterSaturationFilter(0.5);
    Image8 desaturatedImage = runPointFilter(move(img), desaturate, policy);
    */

    rgb nearWhite{245, 245, 245};
    PointFilter overlay = makeColorOverlayFilter(nearWhite, 0.8);
    return runPointFilter(move(img), overlay, policy);
}

Image8 boxBlur(Image8 img) {
    ParallelismPolicy parallelismPolicy = PP_SINGLE_THREAD;
    EdgePolicy edgePolicy = EP_CLAMP;

    return runLocal2DFilter(move(img), boxBlur3, parallelismPolicy, edgePolicy);
}

int main(int argc, char **argv) {
    log("[main] Starting");

    directoryTransform("test-images", "output-images", boxBlur);

    log("[main] Finished");

    return 0;
}
