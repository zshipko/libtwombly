#ifndef TWOMBLY_IMAGE_HEADER
#define TWOMBLY_IMAGE_HEADER

#include "config.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

namespace tw {

struct Point {
    Point(double _x, double _y) : x(_x), y(_y) {}
    double x, y;
};

struct Rectangle {
    Rectangle(double _x, double _y, double _w, double _h) :
         x(_x), y(_y), width(_w), height(_h) {}
    double x, y, width, height;
};

} //namespace tw

#endif //TWOMBLY_IMAGE_HEADER
