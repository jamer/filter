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

void log(string message) {
    time_point<high_resolution_clock> now;
    time_t now_time;
    char now_string[128];

    now = high_resolution_clock::now();
    now_time = high_resolution_clock::to_time_t(now);
    ctime_r(&now_time, now_string);

    string line = string("[") + now_string + "] " + message + "\n";
    cout << message << endl;
}
