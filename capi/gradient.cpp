#include "draw_c.h"

#ifdef __cplusplus
using namespace tw;
#endif


void draw_fillPattern (drawing d, int64_t width, int64_t height, int channels, uint8_t *data){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->fillPattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->fillPattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->fillPattern<Color16>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->fillPattern<Color16>(width, height, channels, data);
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->fillPattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->fillPattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->fillPattern<Color16>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->fillPattern<Color16>(width, height, channels, data);
        break;
    }
}

void draw_strokePattern (drawing d, int64_t width, int64_t height, int channels, uint8_t *data){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->strokePattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->strokePattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->strokePattern<Color16>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->strokePattern<Color16>(width, height, channels, data);
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->strokePattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->strokePattern<Color>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->strokePattern<Color16>(width, height, channels, data);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->strokePattern<Color16>(width, height, channels, data);
        break;
    }
}

void draw_fillLinearGradientH(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix _mtx){
switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->fillLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->fillLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->fillLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->fillLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->fillLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->fillLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->fillLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->fillLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    }
}

void draw_fillLinearGradientV(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix _mtx){
switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->fillLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->fillLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->fillLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->fillLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->fillLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->fillLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->fillLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->fillLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    }
}

void draw_strokeLinearGradientH(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix _mtx){
switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->strokeLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->strokeLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->strokeLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->strokeLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->strokeLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->strokeLinearGradientH<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->strokeLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->strokeLinearGradientH<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    }
}

void draw_strokeLinearGradientV(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix _mtx){
switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->strokeLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->strokeLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->strokeLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->strokeLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->strokeLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->strokeLinearGradientV<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->strokeLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->strokeLinearGradientV<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    }
}

void draw_fillRadialGradient(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix _mtx){
switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->fillRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->fillRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->fillRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->fillRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->fillRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->fillRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->fillRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->fillRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    }
}

void draw_strokeRadialGradient(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix _mtx){
switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->strokeRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->strokeRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->strokeRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->strokeRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->strokeRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->strokeRadialGradient<Color>(
                Color(b[0], b[1], b[2], b[3]),
                Color(m[0], m[1], m[2], m[3]),
                Color(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->strokeRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->strokeRadialGradient<Color16>(
                Color16(b[0], b[1], b[2], b[3]),
                Color16(m[0], m[1], m[2], m[3]),
                Color16(e[0], e[1], e[2], e[3]),
                s, x, *((agg::trans_affine*)_mtx.handle));
        break;
    }
}

