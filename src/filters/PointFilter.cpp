// Copyright 2016 Paul Merrill

#include <algorithm>
#include <cstdint>
#include <omp.h>
#include <string>
#include <thread>
#include <vector>

#include "../Log.h"
#include "PointFilter.h"

using std::string;
using std::thread;
using std::transform;
using std::to_string;
using std::vector;

Image8 runPointFilterSingleThreaded(Image8 image, PointFilter filter) {
    uint32_t w = image.w;
    uint32_t h = image.h;
    rgb *pixels = image.pixels;

    size_t pixelCount = w * h;

    rgb *begin = pixels;
    rgb *end = pixels + pixelCount;

    transform(begin, end, begin, filter);

    return std::move(image);
}

Image8 runPointFilterMultiThreadedWithSharedMemory(Image8 image,
                                                   PointFilter filter) {
    unsigned threadCount = thread::hardware_concurrency();

    string message = "[PointFilter] Starting " + to_string(threadCount) +
                     " threads";
    log(message);

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

    return std::move(image);
}

Image8 runPointFilterWithOpenMP(Image8 image, PointFilter filter) {
    int threadCount = omp_get_max_threads();

    string message = "[PointFilter] Starting " + to_string(threadCount) +
                     " threads";
    log(message);

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

        transform(begin, end, begin, filter);
    }

    return std::move(image);
}

Image8 runPointFilter(Image8 image, PointFilter filter,
                      ParallelismPolicy policy) {
    switch (policy) {
    case PP_SINGLE_THREADED:
        return runPointFilterSingleThreaded(std::move(image), filter);
    case PP_MULTI_THREADED:
        return runPointFilterMultiThreadedWithSharedMemory(std::move(image),
                                                           filter);
    case PP_OPEN_MP:
        return runPointFilterWithOpenMP(std::move(image), filter);
    default:
        throw "Unknown parallelism policy";
    }
}
