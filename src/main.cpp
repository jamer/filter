// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "Log.h"
#include "filters/point-filters/ColorOverlay.h"
#include "filters/point-filters/Saturation.h"

using std::move;

Image8 appleFrostedGlass(Image8 img) {
    ParallelismPolicy policy = PP_CXX_STD_THREAD;

    PointFilter desaturate = makeAlterSaturationFilter(0.5);
    Image8 desaturatedImage = runPointFilter(move(img), desaturate, policy);

    rgb nearWhite{245, 245, 245};
    PointFilter overlay = makeColorOverlayFilter(nearWhite, 0.66);
    return runPointFilter(move(desaturatedImage), overlay, policy);
}

int main(int argc, char **argv) {
    log("[main] Starting");

    directoryTransform("test-images", "output-images", appleFrostedGlass);

    log("[main] Finished");

    return 0;
}
