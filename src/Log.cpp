// Copyright 2016 Paul Merrill

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

#include "Log.h"

using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::cout;
using std::endl;
using std::string;

static string to_string(time_point<high_resolution_clock> point) {
    time_t point_time;
    char point_cstring[128];
    string point_string;

    point_time = high_resolution_clock::to_time_t(point);
    ctime_r(&point_time, point_cstring);

    point_string = point_cstring;
    point_string.erase(point_string.find('\n'));

    return point_string;
}

void log(string message) {
    time_point<high_resolution_clock> now = high_resolution_clock::now();

    string line = "[" + to_string(now) + "] " + message;
    cout << line << endl;
}
