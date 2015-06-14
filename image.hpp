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
class Point {
public:
    double x, y;
};

class Rectangle {
public:
    double x, y, width, height;
};
#else // opencv is included
using namespace cv;
#endif

} //namespace tw

#endif //TWOMBLY_IMAGE_HEADER
