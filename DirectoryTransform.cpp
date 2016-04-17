// Copyright 2016 Paul Merrill

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <string>

#include "DirectoryTransform.h"

using boost::filesystem::create_directories;
using boost::filesystem::directory_iterator;
using boost::filesystem::path;
using boost::filesystem::remove_all;
using std::string;

static bool isImageFileName(string path)
{
    size_t dot = path.rfind(".");
    if (dot == string::npos) {
        return false;
    }
    string extension = path.substr(dot);
    return extension == ".png" ||
           extension == ".jpg" ||
           extension == ".bmp";
}

static void clearDirectory(path path)
{
    remove_all(path);
    create_directories(path);
}

void directoryTransform(string sourceDir, string destinationDir,
                        ImageTransform operation)
{
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
        BOOST_LOG_TRIVIAL(info) << message;

        Image8 originalImage(inputPath.string());
        Image8 modifiedImage = operation(originalImage);

        if (!clearedDestinationDirectory) {
            clearDirectory(destinationDir);
            clearedDestinationDirectory = true;
        }

        string outputPath = destinationDir + relativePath;
        modifiedImage.write(outputPath);
    }
}
