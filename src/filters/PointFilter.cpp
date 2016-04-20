// Copyright 2016 Paul Merrill

#include <algorithm>
#include <cstdint>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "../Log.h"
#include "../MPIHelper.h"
#include "PointFilter.h"

using std::move;
using std::string;
using std::thread;
using std::transform;
using std::to_string;
using std::vector;

Image8 runPointFilterSingleThreaded(Image8 image, PointFilter filter) {
    log("[PointFilter] Executing single threaded");

    uint32_t w = image.w;
    uint32_t h = image.h;
    rgb *pixels = image.pixels;

    size_t pixelCount = w * h;

    rgb *begin = pixels;
    rgb *end = pixels + pixelCount;

    transform(begin, end, begin, filter);

    return move(image);
}

Image8 runPointFilterWithStdThread(Image8 image, PointFilter filter) {
    unsigned threadCount = thread::hardware_concurrency();

    log("[PointFilter] Parallelizing with C++ std::thread");
    log("[PointFilter] Starting " + to_string(threadCount) + " threads");

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

        thread *t = new thread([begin, end, &image, filter]() {
            transform(begin, end, begin, filter);
        });
        threads.push_back(t);
    }

    for (thread *t : threads) {
        t->join();
    }

    return move(image);
}

Image8 runPointFilterWithOpenMP(Image8 image, PointFilter filter) {
    int threadCount = omp_get_max_threads();

    log("[PointFilter] Parallelizing with OpenMP");
    log("[PointFilter] Starting " + to_string(threadCount) + " threads");

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

        thread *t = new thread([begin, end, &image, filter]() {
            transform(begin, end, begin, filter);
        });
    }

    return move(image);
}

static Image8 runPointFilterWithMPIMaster(Image8 image, PointFilter filter) {
    int processCount = mpiHelper.processCount();

    log("[PointFilter] Parallelizing with MPIHelper");
    log("[PointFilter] Using " + to_string(processCount) + " processes");

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

    transform(masterBegin, masterEnd, masterBegin, filter);

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

static void runPointFilterWithMPISlave(PointFilter filter) {
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

    transform(slaveBegin, slaveEnd, slaveBegin, filter);

    mpiHelper.sendToMaster(slaveBegin, limit);

    delete pixels;
}

Image8 runPointFilterWithMPI(Image8 image, PointFilter filter) {
    int processId = mpiHelper.myProcessId();
    if (processId == MPI_MASTER_PROCESS_ID) {
        return runPointFilterWithMPIMaster(move(image), filter);
    } else {
        runPointFilterWithMPISlave(filter);
        return move(image);
    }
}

Image8 runPointFilter(Image8 image, PointFilter filter,
                      ParallelismPolicy policy) {
    if (policy != PP_MPI) {
        // MPI spawns multiple processes for us even if we're not going to use
        // them in this filter. Only let one process tackle filtering this
        // image.
        int processId = mpiHelper.myProcessId();
        if (processId != MPI_MASTER_PROCESS_ID) {
            return move(image);
        }
    }

    switch (policy) {
    case PP_SINGLE_THREAD:
        return runPointFilterSingleThreaded(move(image), filter);
    case PP_CXX_STD_THREAD:
        return runPointFilterWithStdThread(move(image), filter);
    case PP_OPEN_MP:
        return runPointFilterWithOpenMP(move(image), filter);
    case PP_MPI:
        return runPointFilterWithMPI(move(image), filter);
    default:
        throw "Unknown parallelism policy";
    }
}
