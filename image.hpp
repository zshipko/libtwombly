#ifndef TWOMBLY_IMAGE_HEADER
#define TWOMBLY_IMAGE_HEADER
#include <bitset>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

#include <opencv2/opencv.hpp>

namespace tw {

using namespace cv;

typedef float Pixel __attribute__ ((vector_size (16)));

enum {
    BLUE,
    GREEN,
    RED,
    ALPHA
} COLOR_INDEX;

// simple content based hash
std::bitset<64> hash(Mat &&im);

} //namespace tw

#endif //TWOMBLY_IMAGE_HEADER
