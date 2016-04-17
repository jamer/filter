#include <iostream>
#include "ImageI8.h"

using namespace std;

int main(int argc, char **argv) {
    cout << "Hello, World!" << endl;
    ImageI8 i(argv[1]);
    i.write("out.bmp");
    return 0;
}
