// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "Log.h"
#include "filters/Local2DFilter.h"
#include "filters/local-2d-kernels/GaussianBlur.h"
#include "filters/PointFilter.h"
#include "filters/point-filters/ColorOverlay.h"
#include "filters/point-filters/Saturation.h"

using std::move;

Image8 appleFrostedGlass(Image8 image) {
    ParallelismPolicy parallelismPolicy = PP_CXX_STD_THREAD;
    EdgePolicy edgePolicy = EP_CLAMP;

    /*
    PointFilter desaturate = makeAlterSaturationFilter(0.5);
    Image8 desaturatedImage = runPointFilter(move(img), desaturate, policy);
    */

    Kernel blur = makeGaussianBlur(12, 12);
    Image8 blurredImage = runLocal2DFilter(move(image), blur, parallelismPolicy,
                                           edgePolicy);

    rgb nearWhite{245, 245, 245};
    PointFilter overlay = makeColorOverlayFilter(nearWhite, 0.8);
    return runPointFilter(move(blurredImage), overlay, parallelismPolicy);
}

Image8 gaussianBlur(Image8 img) {
    ParallelismPolicy parallelismPolicy = PP_SINGLE_THREAD;
    EdgePolicy edgePolicy = EP_CLAMP;

    Kernel blur = makeGaussianBlur(12, 5);
    return runLocal2DFilter(move(img), blur, parallelismPolicy, edgePolicy);
}

int main(int argc, char **argv) {
    log("[main] Starting");

    directoryTransform("test-images", "output-images", appleFrostedGlass);

    log("[main] Finished");

    return 0;
}
