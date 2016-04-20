// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "Log.h"

#include "filters/Local2DFilter.h"
#include "filters/local-2d-kernels/BoxBlur.h"
#include "filters/local-2d-kernels/GaussianBlur.h"
#include "filters/local-2d-kernels/UnsharpenMask3.h"
#include "filters/PointFilter.h"

#include "filters/point-filters/ColorOverlay.h"
#include "filters/point-filters/Fill.h"
#include "filters/point-filters/Grayscale.h"
#include "filters/point-filters/Saturation.h"
#include "filters/point-filters/Sepia.h"

using std::move;

//
// Point filters
//
Image8 colorOverlay(Image8 img) {
    ParallelismPolicy policy = PP_SINGLE_THREAD;
    rgb color{255, 127, 0};
    float alpha = 0.5;
    PointFilter filter = makeColorOverlayFilter(color, alpha);
    return runPointFilter(img, filter, policy);
}

Image8 fill(Image8 img) {
    ParallelismPolicy policy = PP_CXX_STD_THREAD;
    rgb color{127, 0, 127};
    PointFilter filter = makeFillFilter(color);
    return runPointFilter(img, filter, policy);
}

Image8 grayscale(Image8 img) {
    ParallelismPolicy policy = PP_OPEN_MP;
    PointFilter filter = makeGrayscaleFilter();
    return runPointFilter(img, filter, policy);
}

Image8 saturation(Image8 img) {
    ParallelismPolicy policy = PP_MPI;
    float multiple = 0.25;
    PointFilter filter = makeAlterSaturationFilter(multiple);
    return runPointFilter(img, filter, policy);
}

Image8 sepia(Image8 img) {
    ParallelismPolicy policy = PP_OPEN_MP;
    PointFilter filter = makeSepiaFilter();
    return runPointFilter(img, filter, policy);
}

//
// Local-2D filters
//
Image8 boxBlur(Image8 img) {
    ParallelismPolicy parallelismPolicy = PP_OPEN_MP;
    EdgePolicy edgePolicy = EP_CLAMP;

    Kernel blur = makeBoxBlur(4);
    return runLocal2DFilter(move(img), blur, parallelismPolicy, edgePolicy);
}

Image8 gaussianBlur(Image8 img) {
    ParallelismPolicy parallelismPolicy = PP_SINGLE_THREAD;
    EdgePolicy edgePolicy = EP_WRAP;

    Kernel blur = makeGaussianBlur(6, 2);
    return runLocal2DFilter(move(img), blur, parallelismPolicy, edgePolicy);
}

Image8 unsharpenMask(Image8 img) {
    ParallelismPolicy parallelismPolicy = PP_CXX_STD_THREAD;
    EdgePolicy edgePolicy = EP_CLAMP;

    Kernel sharpen = unsharpenMask3;
    return runLocal2DFilter(move(img), sharpen, parallelismPolicy, edgePolicy);
}

//
// Composite filters
//
Image8 appleFrostedGlass(Image8 img) {
    ParallelismPolicy parallelismPolicy = PP_CXX_STD_THREAD;
    EdgePolicy edgePolicy = EP_CLAMP;

    Kernel blur = makeGaussianBlur(12, 12);
    Image8 blurredImage = runLocal2DFilter(move(img), blur, parallelismPolicy,
                                           edgePolicy);

    rgb nearWhite{245, 245, 245};
    PointFilter overlay = makeColorOverlayFilter(nearWhite, 0.8);
    return runPointFilter(move(blurredImage), overlay, parallelismPolicy);
}

int main(int argc, char **argv) {
    log("[main] Starting");

    // Point filters
    directoryTransform("test-images", "output-color-overlay", colorOverlay);
    directoryTransform("test-images", "output-fill", fill);
    directoryTransform("test-images", "output-grayscale", grayscale);
    directoryTransform("test-images", "output-saturation", saturation);
    directoryTransform("test-images", "output-sepia", sepia);

    // Local-2D filters
    directoryTransform("test-images", "output-box-blur", boxBlur);
    directoryTransform("test-images", "output-gaussian-blur", gaussianBlur);
    directoryTransform("test-images", "output-unsharpen-mask", unsharpenMask);

    // Composite filters
    directoryTransform("test-images", "output-apple", appleFrostedGlass);

    log("[main] Finished");

    return 0;
}
