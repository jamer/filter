// Copyright 2016 Paul Merrill

#include <utility>

#include "DirectoryTransform.h"
#include "Image8.h"
#include "MPIHelper.h"
#include "Log.h"
#include "filters/point-filters/Saturation.h"

using std::move;

int main(int argc, char **argv) {
    log("[main] Starting");

    directoryTransform("test-images", "output-images",
                       [](Image8 img) {
                           PointFilter filter = makeAlterSaturationFilter(0.25);
                           ParallelismPolicy policy = PP_CXX_STD_THREAD;
                           return runPointFilter(move(img), filter, policy);
                       });

    log("[main] Finished");

    return 0;
}
