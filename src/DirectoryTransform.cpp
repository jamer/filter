// Copyright 2016 Paul Merrill

#include <chrono>
#include <string>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>

#include "DirectoryTransform.h"
#include "Log.h"
#include "MPIHelper.h"

using boost::filesystem::create_directories;
using boost::filesystem::directory_iterator;
using boost::filesystem::path;
using boost::filesystem::remove_all;
using std::chrono::duration;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::string;
using std::to_string;

class OperationTimer {
 public:
    OperationTimer() {
        begin = high_resolution_clock::now();
    }

    ~OperationTimer() {
        time_point<high_resolution_clock> finish = high_resolution_clock::now();
        duration<double> elapsedSeconds = finish - begin;

        string message = "[DirectoryTransform] Operation took " +
                to_string(elapsedSeconds.count()) + " seconds";
        log(message);
    }

 private:
    time_point<high_resolution_clock> begin;
};

static bool isImageFileName(string path) {
    size_t dot = path.rfind(".");
    if (dot == string::npos) {
        return false;
    }
    string extension = path.substr(dot);
    return extension == ".png" ||
           extension == ".jpg" ||
           extension == ".bmp";
}

static string changeFileExtention(string filename, string newExtension) {
    size_t dot = filename.rfind(".");
    if (dot == string::npos) {
        return filename + "." + newExtension;
    } else {
        return filename.substr(0, dot + 1) + newExtension;
    }
}

static void clearDirectory(string path) {
    remove_all(path);
    create_directories(path);
}

static void directoryTransformMaster(string sourceDir, string destinationDir,
                                     ImageTransform operation) {
    if (sourceDir[sourceDir.size() - 1] != '/') {
        sourceDir += '/';
    }
    if (destinationDir[destinationDir.size() - 1] != '/') {
        destinationDir += '/';
    }

    bool clearedDestinationDirectory = false;

    directory_iterator itr(sourceDir);
    directory_iterator end_itr;

    for (; itr != end_itr; ++itr) {
        path inputPath = itr->path();
        string relativePath = boost::replace_first_copy(inputPath.string(),
                                                        sourceDir, "");
        if (!isImageFileName(relativePath)) {
            continue;
        }

        string message = "[DirectoryTransform] Processing " + relativePath;
        log(message);

        Image8 originalImage(inputPath.string());
        Image8 modifiedImage;

        {
            OperationTimer timer;
            modifiedImage = operation(std::move(originalImage));
        }

        if (!clearedDestinationDirectory) {
            clearDirectory(destinationDir);
            clearedDestinationDirectory = true;
        }

        string outputPath = destinationDir +
                changeFileExtention(relativePath, "bmp");
        modifiedImage.write(outputPath);
    }
}

static void directoryTransformSlave(string sourceDir,
                                    ImageTransform operation) {
    directory_iterator itr(sourceDir);
    directory_iterator end_itr;

    for (; itr != end_itr; ++itr) {
        path inputPath = itr->path();
        if (!isImageFileName(inputPath.string())) {
            continue;
        }

        operation(Image8());
    }
}

void directoryTransform(string sourceDir, string destinationDir,
                        ImageTransform operation) {
    assert(sourceDir.size() && destinationDir.size());

    int processId = mpiHelper.myProcessId();

    if (processId == MPI_MASTER_PROCESS_ID) {
        directoryTransformMaster(sourceDir, destinationDir, operation);
    } else {
        directoryTransformSlave(sourceDir, operation);
    }
}
