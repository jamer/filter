// Copyright 2016 Paul Merrill

#ifndef DIRECTORYTRANSFORM_H_
#define DIRECTORYTRANSFORM_H_

#include <string>
#include <functional>
#include "Image8.h"

typedef std::function<Image8(Image8)> ImageTransform;

void directoryTransform(string sourceDir, string destinationDir,
                        ImageTransform operation);

#endif  // DIRECTORYTRANSFORM_H_
