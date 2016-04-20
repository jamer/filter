// Copyright 2016 Paul Merrill

#ifndef SRC_PARALLELISMPOLICY_H_
#define SRC_PARALLELISMPOLICY_H_

enum ParallelismPolicy {
    PP_SINGLE_THREAD,
    PP_CXX_STD_THREAD,
    PP_OPEN_MP,
    PP_MPI
};

#endif  // SRC_PARALLELISMPOLICY_H_
