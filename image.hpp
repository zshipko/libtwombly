#ifndef TWOMBLY_IMAGE_HEADER
#define TWOMBLY_IMAGE_HEADER

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

#ifndef NO_OPENCV
#include <opencv2/opencv.hpp>
#endif

namespace tw {


#if defined(NO_OPENCV) || !defined(CV_8U)
struct Point {
    Point(double _x, double _y) : x(_x), y(_y) {}
    double x, y;
};

struct Rectangle {
    Rectangle(double _x, double _y, double _w, double _h) :
         x(_x), y(_y), width(_w), height(_h) {}
    double x, y, width, height;
};
#else // opencv is included
using namespace cv;
#endif

} //namespace tw

#endif //TWOMBLY_IMAGE_HEADER
