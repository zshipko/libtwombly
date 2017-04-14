#include "draw.h"

#ifdef __cplusplus
using namespace tw;
#endif

gradient draw_gradient_create(){
    gradient grad;
    grad.handle = (void*) new Gradient<Color>();
    return grad;
}

void draw_gradient_free(gradient *grad){
    if (grad->handle){
        delete ((Gradient<Color>*)grad->handle);
        grad->handle = nullptr;
    }
}

void draw_gradient_add_stop(gradient grad, Pixel color){
    ((Gradient<Color>*)grad.handle)->add_stop(Color(color[0], color[1], color[2], color[3]));
}

transform_matrix draw_gradient_get_matrix(gradient grad){
    transform_matrix mtx;
    mtx.handle = &(((Gradient<Color>*)grad.handle)->mtx);
    return mtx;
}

void draw_gradient_set_matrix(gradient grad, transform_matrix mtx){
    ((Gradient<Color>*)grad.handle)->mtx = *(agg::trans_affine*)mtx.handle;
}

gradient draw_gradient_create16(){
    gradient grad;
    grad.handle = (void*) new Gradient<Color16>();
    return grad;
}

void draw_gradient_add_stop16(gradient grad, Pixel color){
    ((Gradient<Color16>*)grad.handle)->add_stop(Color16(color[0], color[1], color[2], color[3]));
}

void draw_fill_gradient (drawing d, gradient grad, int s, int x, gradient_type grad_type){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgb24>*)d.handle)->fill_gradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgb48>*)d.handle)->fill_gradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    case 4:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgba32>*)d.handle)->fill_gradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgba64>*)d.handle)->fill_gradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    }
}

void draw_stroke_gradient (drawing d, gradient grad, int s, int x, gradient_type grad_type){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgb24>*)d.handle)->stroke_gradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgb48>*)d.handle)->stroke_gradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    case 4:
        if (d.bits_per_channel == 8 )
            ((Drawing<rgba32>*)d.handle)->stroke_gradient<Color>(
                *((Gradient<Color>*)grad.handle), s, x, grad_type
            );
        else if (d.bits_per_channel == 16)
            ((Drawing<rgba64>*)d.handle)->stroke_gradient<Color16>(
                *((Gradient<Color16>*)grad.handle), s, x, grad_type
            );
        break;
    }
}

