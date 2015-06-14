#include "draw_c.h"

#ifdef __cplusplus
using namespace tw;
#endif

gradient draw_gradientCreate(){
    gradient grad;
    grad.handle = (void*) new Gradient<Color>();
    return grad;
}

void draw_gradientFree(gradient *grad){
    if (grad->handle){
        delete ((Gradient<Color>*)grad->handle);
        grad->handle = nullptr;
    }
}

void draw_gradientAddStop(gradient grad, Pixel color){
    ((Gradient<Color>*)grad.handle)->addStop(Color(color[0], color[1], color[2], color[3]));
}

transform_matrix draw_gradientGetMatrix(gradient grad){
    transform_matrix mtx;
    mtx.handle = &(((Gradient<Color>*)grad.handle)->mtx);
    return mtx;
}

void draw_gradientSetMatrix(gradient grad, transform_matrix mtx){
    ((Gradient<Color>*)grad.handle)->mtx = *(agg::trans_affine*)mtx.handle;
}

gradient draw_gradientCreate16(){
    gradient grad;
    grad.handle = (void*) new Gradient<Color16>();
    return grad;
}

void draw_gradientAddStop16(gradient grad, Pixel color){
    ((Gradient<Color16>*)grad.handle)->addStop(Color16(color[0], color[1], color[2], color[3]));
}

void draw_fillGradient (drawing d, gradient grad, int s, int x, gradient_type grad_type){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgb24>*)d.handle)->fillGradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgb48>*)d.handle)->fillGradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    case 4:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgba32>*)d.handle)->fillGradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgba64>*)d.handle)->fillGradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    }
}

void draw_strokeGradient (drawing d, gradient grad, int s, int x, gradient_type grad_type){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgb24>*)d.handle)->strokeGradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgb48>*)d.handle)->strokeGradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    case 4:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgba32>*)d.handle)->strokeGradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgba64>*)d.handle)->strokeGradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    }
}

void draw_fillPattern (drawing d, drawing e){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->fillPattern<Color>(*(Drawing<rgb24>*)e.handle);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->fillPattern<Color>(*(Drawing<bgr24>*)e.handle);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->fillPattern<Color16>(*(Drawing<rgb48>*)e.handle);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->fillPattern<Color16>(*(Drawing<bgr48>*)e.handle);
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->fillPattern<Color>(*(Drawing<rgba32>*)e.handle);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->fillPattern<Color>(*(Drawing<bgra32>*)e.handle);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->fillPattern<Color16>(*(Drawing<rgba64>*)e.handle);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->fillPattern<Color16>(*(Drawing<bgra64>*)e.handle);
        break;
    }
}

void draw_strokePattern (drawing d, drawing e){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgb24>*)d.handle)->strokePattern<Color>(*(Drawing<rgb24>*)e.handle);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgr24>*)d.handle)->strokePattern<Color>(*(Drawing<bgr24>*)e.handle);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgb48>*)d.handle)->strokePattern<Color16>(*(Drawing<rgb48>*)e.handle);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgr48>*)d.handle)->strokePattern<Color16>(*(Drawing<bgr48>*)e.handle);
        break;
    case 4:
        if (d.bits_per_channel == 8 && !d.is_bgr) ((Drawing<rgba32>*)d.handle)->strokePattern<Color>(*(Drawing<rgba32>*)e.handle);
        else if (d.bits_per_channel == 8 && d.is_bgr) ((Drawing<bgra32>*)d.handle)->strokePattern<Color>(*(Drawing<bgra32>*)e.handle);
        else if (d.bits_per_channel == 16 && !d.is_bgr) ((Drawing<rgba64>*)d.handle)->strokePattern<Color16>(*(Drawing<rgba64>*)e.handle);
        else if (d.bits_per_channel == 16 && d.is_bgr) ((Drawing<bgra64>*)d.handle)->strokePattern<Color16>(*(Drawing<bgra64>*)e.handle);
        break;
    }
}

