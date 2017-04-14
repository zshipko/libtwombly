#include "draw.hpp"

namespace tw {

template class Drawing<rgba32>;
template class Drawing<rgb24>;
template class Drawing<rgba64>;
template class Drawing<rgb48>;
template class Drawing<bgra32>;
template class Drawing<bgr24>;
template class Gradient<Color>;
template class Gradient<Color16>;

inline Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return Color(r, g, b, a);
}

inline Color rgb(uint8_t r, uint8_t g, uint8_t b){
    return Color(r, g, b, 255);
}

inline Color16 rgba_16(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return Color16(r, g, b, a);
}

inline Color16 rgb_16(uint8_t r, uint8_t g, uint8_t b){
    return Color16(r, g, b, 255);
}

DrawingRGBA32 draw(int32_t w, int32_t h, uint8_t *data){
    return Drawing<rgba32>(w, h, 4, data);
}

DrawingRGBA64 draw(int32_t w, int32_t h, uint16_t *data){
    return Drawing<rgba64>(w, h, 4, data);
}

DrawingRGB24 draw_rgb(int32_t w, int32_t h, uint8_t *data){
    return Drawing<rgb24>(w, h, 3, data);
}

DrawingRGB48 draw_rgb(int32_t w, int32_t h, uint16_t *data){
    return Drawing<rgb48>(w, h, 3, data);
}

} //namespace tw

