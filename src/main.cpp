// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "MPIHelper.h"
#include "Log.h"
#include "filters/point-filters/Grayscale.h"

using std::move;

int main(int argc, char **argv) {
    log("[main] Starting");

    directoryTransform("test-images", "output-images",
                       [](Image8 img) {
                           PointFilter overlay = makeGrayscaleFilter();
                           ParallelismPolicy policy = PP_CXX_STD_THREAD;
                           return runPointFilter(move(img), overlay, policy);
                       });

    log("[main] Finished");

    return 0;
}
