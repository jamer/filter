// Copyright 2016 Paul Merrill

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "../Log.h"
#include "../MPIHelper.h"
#include "Local2DFilter.h"

using std::max;
using std::min;
using std::move;
using std::string;
using std::thread;
using std::transform;
using std::to_string;
using std::vector;

static rgb getPixel(uint32_t w, uint32_t h, rgb *source, EdgePolicy edgePolicy,
                    int32_t x, int32_t y) {
    int32_t inBoundsX = x;
    int32_t inBoundsY = y;
    inBoundsX = max(inBoundsX, 0);
    inBoundsY = max(inBoundsY, 0);
    inBoundsX = min(inBoundsX, static_cast<int32_t>(w) - 1);
    inBoundsY = min(inBoundsY, static_cast<int32_t>(h) - 1);

    if (x != inBoundsX || y != inBoundsY) {
        switch (edgePolicy) {
        case EP_CLAMP:
            return source[inBoundsY * w + inBoundsX];
        case EP_WRAP:
            // TODO(pdm): Unimplemented due to considerations with
            //            multiprocessing.
            return rgb{0, 0, 0};
        case EP_CLIP:
            return rgb{0, 0, 0};
        }
    }

    return source[y * w + x];
}

static rgb convolutePixel(uint32_t w, uint32_t h, rgb *source,
                          const Kernel& kernel, EdgePolicy edgePolicy,
                          uint32_t destX, uint32_t destY) {
    int kernelRadius = (kernel.dimension - 1) / 2;

    rgb destPixel{0, 0, 0};

    for (int32_t y = -kernelRadius; y <= kernelRadius; y++) {
        for (int32_t x = -kernelRadius; x <= kernelRadius; x++) {
            float coefficient = kernel.value(kernelRadius + x,
                                             kernelRadius + y);
            int32_t sourceX = destX + x;
            int32_t sourceY = destY + y;
            rgb sourcePixel = getPixel(w, h, source, edgePolicy,
                                       sourceX, sourceY);
            destPixel += sourcePixel * coefficient;
        }
    }

    return destPixel;
}

static Image8 convoluteImage(uint32_t w, uint32_t h, rgb *source,
                             const Kernel& kernel, EdgePolicy edgePolicy,
                             uint32_t initialY, uint32_t finalY) {
    Image8 destination(w, h);
    for (uint32_t y = initialY; y < finalY; y++) {
        for (uint32_t x = 0; x < w; x++) {
            destination.pixels[y * w + x] = convolutePixel(w, h, source, kernel,
                                                           edgePolicy, x, y);
        }
    }
    return move(destination);
}

Image8 runLocal2DFilterSingleThreaded(Image8 source, Kernel kernel,
                                      EdgePolicy edgePolicy) {
    log("[Local2DFilter] Executing single threaded");

    uint32_t w = source.w;
    uint32_t h = source.h;

    return convoluteImage(w, h, source.pixels, kernel, edgePolicy, 0, h);
}

Image8 runLocal2DFilterWithStdThread(Image8 image, Kernel kernel) {
    unsigned threadCount = thread::hardware_concurrency();

    log("[Local2DFilter] Parallelizing with C++ std::thread");
    log("[Local2DFilter] Starting " + to_string(threadCount) + " threads");

    uint32_t w = image.w;
    uint32_t h = image.h;
    rgb *pixels = image.pixels;

    size_t pixelCount = w * h;
    size_t pixelsPerThread = pixelCount / threadCount;

    vector<thread *> threads;

    for (int threadId = 0; threadId < threadCount; threadId++) {
        size_t offset = pixelsPerThread * threadId;
        size_t limit = pixelsPerThread;

        if (threadId == threadCount - 1) {
           limit = pixelCount - offset;
        }

        rgb *begin = pixels + offset;
        rgb *end = pixels + offset + limit;

        thread *t = new thread([begin, end, &image, kernel]() {
            //transform(begin, end, begin, kernel);
        });
        threads.push_back(t);
    }

    for (thread *t : threads) {
        t->join();
    }

    return move(image);
}

Image8 runLocal2DFilterWithOpenMP(Image8 image, Kernel kernel) {
    int threadCount = omp_get_max_threads();

    log("[Local2DFilter] Parallelizing with OpenMP");
    log("[Local2DFilter] Starting " + to_string(threadCount) + " threads");

    uint32_t w = image.w;
    uint32_t h = image.h;
    rgb *pixels = image.pixels;

    size_t pixelCount = w * h;
    size_t pixelsPerThread = pixelCount / threadCount;

    #pragma omp parallel for
    for (int threadId = 0; threadId < threadCount; threadId++) {
        size_t offset = pixelsPerThread * threadId;
        size_t limit = pixelsPerThread;

        if (threadId == threadCount - 1) {
            limit = pixelCount - offset;
        }

        rgb *begin = pixels + offset;
        rgb *end = pixels + offset + limit;

        //transform(begin, end, begin, kernel);
    }

    return move(image);
}

static Image8 runLocal2DFilterWithMPIMaster(Image8 image, Kernel kernel) {
    int processCount = mpiHelper.processCount();

    log("[Local2DFilter] Parallelizing with MPIHelper");
    log("[Local2DFilter] Using " + to_string(processCount) + " processes");

    uint32_t w = image.w;
    uint32_t h = image.h;
    rgb *pixels = image.pixels;

    size_t pixelCount = w * h;
    size_t pixelsPerThread = pixelCount / processCount;

    for (int slaveId = 1; slaveId < processCount; slaveId++) {
        size_t offset = pixelsPerThread * slaveId;
        size_t limit = pixelsPerThread;

        if (slaveId == processCount - 1) {
            limit = pixelCount - offset;
        }

        rgb *slaveBegin = pixels + offset;
        // rgb *slaveEnd = pixels + offset + limit;

        mpiHelper.sendToSlave(slaveId, w);
        mpiHelper.sendToSlave(slaveId, h);
        mpiHelper.sendToSlave(slaveId, slaveBegin, limit);
    }

    size_t masterOffset = pixelsPerThread * 0;
    size_t masterLimit = pixelsPerThread;

    rgb *masterBegin = pixels + masterOffset;
    rgb *masterEnd = pixels + masterOffset + masterLimit;

    //transform(masterBegin, masterEnd, masterBegin, kernel);

    for (int slaveId = 1; slaveId < processCount; slaveId++) {
        size_t offset = pixelsPerThread * slaveId;
        size_t limit = pixelsPerThread;

        if (slaveId == processCount - 1) {
            limit = pixelCount - offset;
        }

        rgb *slaveBegin = pixels + offset;
        // rgb *slaveEnd = pixels + offset + limit;

        mpiHelper.receiveFromSlave(slaveId, slaveBegin, limit);
    }

    return move(image);
}

static void runLocal2DFilterWithMPISlave(Kernel kernel) {
    int slaveId = mpiHelper.myProcessId();
    int processCount = mpiHelper.processCount();

    uint32_t w;
    uint32_t h;

    mpiHelper.receiveFromMaster(&w);
    mpiHelper.receiveFromMaster(&h);

    size_t pixelCount = w * h;
    size_t pixelsPerThread = pixelCount / processCount;
    rgb *pixels = new rgb[w * h];

    size_t offset = pixelsPerThread * slaveId;
    size_t limit = pixelsPerThread;

    if (slaveId == processCount - 1) {
        limit = pixelCount - offset;
    }

    rgb *slaveBegin = pixels + offset;
    rgb *slaveEnd = pixels + offset + limit;

    mpiHelper.receiveFromMaster(slaveBegin, limit);

    //transform(slaveBegin, slaveEnd, slaveBegin, kernel);

    mpiHelper.sendToMaster(slaveBegin, limit);

    delete pixels;
}

Image8 runLocal2DFilterWithMPI(Image8 image, Kernel kernel) {
    int processId = mpiHelper.myProcessId();
    if (processId == MPI_MASTER_PROCESS_ID) {
        return runLocal2DFilterWithMPIMaster(move(image), kernel);
    } else {
        runLocal2DFilterWithMPISlave(kernel);
        return move(image);
    }
}

Image8 runLocal2DFilter(Image8 image, Kernel kernel,
                        ParallelismPolicy parallelismPolicy,
                        EdgePolicy edgePolicy) {
    if (parallelismPolicy != PP_MPI) {
        // MPI spawns multiple processes for us even if we're not going to use
        // them in this filter. Only let one process tackle filtering this
        // image.
        int processId = mpiHelper.myProcessId();
        if (processId != MPI_MASTER_PROCESS_ID) {
            return move(image);
        }
    }

    assert((kernel.dimension & 1) == 1);

    switch (parallelismPolicy) {
    case PP_SINGLE_THREAD:
        return runLocal2DFilterSingleThreaded(move(image), kernel, edgePolicy);
    case PP_CXX_STD_THREAD:
        return runLocal2DFilterWithStdThread(move(image), kernel);
    case PP_OPEN_MP:
        return runLocal2DFilterWithOpenMP(move(image), kernel);
    case PP_MPI:
        return runLocal2DFilterWithMPI(move(image), kernel);
    default:
        throw "Unknown parallelism policy";
    }
}
