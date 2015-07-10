#include "draw.hpp"

#ifndef NO_FREETYPE
#include "agg_font_freetype.h"
#endif


#ifndef NO_FREETYPE
template <typename T>
double tw::Drawing<T>::text(double x, double y, const char *txt, const char *font, double width, double height){
    agg::glyph_rendering font_ren = agg::glyph_ren_outline;
    agg::font_engine_freetype_int32 font_engine;
    agg::font_cache_manager< agg::font_engine_freetype_int32> font_manager(font_engine);
    const char *ptr = txt;
    double start_x = x;

    // load font
    if (font_engine.load_font(font, 0, font_ren)){

        // apply settings
        font_engine.width(width);
        font_engine.height(height);
        font_engine.flip_y(true);
        font_engine.hinting(true);

        // draw each character
        while(*ptr){
            const agg::glyph_cache* glyph = font_manager.glyph( (unsigned int)*ptr );
            if (glyph != nullptr){
                font_manager.add_kerning(&x, &y);
                font_manager.init_embedded_adaptors(glyph, x, y);
                auto a = font_manager.path_adaptor();
                agg::conv_curve<agg::font_cache_manager< agg::font_engine_freetype_int32>::path_adaptor_type> curves(a);
                concat_path(curves);
            }
            x += glyph->advance_x;
            y += glyph->advance_y;
            ptr++;
        }
    }
    return x-start_x;
}
#endif // NO_FREETYPE

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

inline Color16 rgba_16(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return Color16(r, g, b, a);
}

inline Color16 rgb_16(uint8_t r, uint8_t g, uint8_t b){
    return Color16(r, g, b, 255);
}

#ifndef NO_OPENCV
Drawing<bgra32> draw(Mat4b& im, uint8_t *alpha_buf){
    return Drawing<bgra32>(im, alpha_buf);
}

Drawing<bgr24>draw(Mat3b& im, uint8_t *alpha_buf){
    return Drawing<bgr24>(im, alpha_buf);
}

Drawing<bgra64> draw(Mat4w& im, uint8_t *alpha_buf){
    return Drawing<bgra64>(im, alpha_buf);
}

Drawing<bgr48> draw(Mat3w& im, uint8_t *alpha_buf){
    return Drawing<bgr48>(im, alpha_buf);
}
#endif


}
