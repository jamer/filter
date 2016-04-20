// Copyright 2016 Paul Merrill

#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>

#include "Log.h"
#include "MPIHelper.h"

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::time_point;
using std::cout;
using std::endl;
using std::string;

static string to_string(time_point<high_resolution_clock> time_point_time) {
    time_t time_time_t;
    tm tm_time;
    char course_time_cstring[128];  // Date, hours, minutes, seconds
    int64_t milliseconds_since_epoch;
    int milliseconds_of_current_second;
    char time_cstring[128];  // Date, hours, minutes, seconds, milliseconds

    time_time_t = high_resolution_clock::to_time_t(time_point_time);
    localtime_r(&time_time_t, &tm_time);
    strftime(course_time_cstring, sizeof(course_time_cstring), "%F %T",
             &tm_time);

    auto time_since_epoch = time_point_time.time_since_epoch();
    milliseconds_since_epoch = duration_cast<milliseconds>(time_since_epoch)
            .count();
    milliseconds_of_current_second = static_cast<int>(
            milliseconds_since_epoch % 1000);

    snprintf(time_cstring, sizeof(time_cstring), "%s.%03d",
             course_time_cstring, milliseconds_of_current_second);

    return string(time_cstring);
}

void log(string message) {
    int processId = mpiHelper.myProcessId();

    time_point<high_resolution_clock> now = high_resolution_clock::now();

    string line = "[" + to_string(now) + "] [#" + std::to_string(processId)
        + "] " + message;
    cout << line << endl;
}
