#include "draw.hpp"

namespace tw {

template class Drawing<rgba32>;
template class Drawing<rgb24>;
template class Drawing<rgba64>;
template class Drawing<rgb48>;
template class Drawing<bgra32>;
template class Drawing<bgr24>;

#ifdef USE_OPENCV
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

#endif // USE_OPENCV

}
