#include "draw.hpp"

namespace tw {

template class Drawing<rgba32>;
template class Drawing<rgb24>;
template class Drawing<rgba64>;
template class Drawing<rgb48>;
template class Drawing<bgra32>;
template class Drawing<bgr24>;

inline Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return Color(r, g, b, a);
}

#ifndef NO_OPENCV

inline Color rgba(Scalar s){
    return Color(s[2], s[1], s[0], s[3]);
}

inline Color16 rgba_16(Scalar s){
    return Color16(s[2], s[1], s[0], s[3]);
}

#endif

inline Color rgb(uint8_t r, uint8_t g, uint8_t b){
    return Color(r, g, b, 255);
}

inline Color rgba(Pixel p){
    return Color(p[0], p[1], p[2], p[3]);
}

inline Color16 rgba_16(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return Color16(r, g, b, a);
}

inline Color16 rgb_16(uint8_t r, uint8_t g, uint8_t b){
    return Color16(r, g, b, 255);
}

inline Color16 rgba_16(Pixel p){
    return Color16(p[0], p[1], p[2], p[3]);
}

#ifndef NO_OPENCV
Drawing<bgra32> draw(Mat4b& im){
    return Drawing<bgra32>(im);
}

Drawing<bgr24>draw(Mat3b& im){
    return Drawing<bgr24>(im);
}

Drawing<bgra64> draw(Mat4w& im){
    return Drawing<bgra64>(im);
}

Drawing<bgr48> draw(Mat3w& im){
    return Drawing<bgr48>(im);
}
#endif


}
