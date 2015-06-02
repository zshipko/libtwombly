#include "draw_c.h"

#ifdef __cplusplus
using namespace tw;
#endif

#define DRAWING(d, fn, ...) do { switch(d.channels){ \
    case 1: \
        if (d.bits_per_channel == 8) return ((Drawing<gray8>*)d.handle)-> fn (__VA_ARGS__); \
        else if(d.bits_per_channel == 16) return ((Drawing<gray16>*)d.handle)-> fn (__VA_ARGS__); \
        break; \
    case 3: \
        if (d.bits_per_channel == 8) return ((Drawing<rgb24>*)d.handle)-> fn (__VA_ARGS__); \
        else if(d.bits_per_channel == 16) return ((Drawing<rgb48>*)d.handle)-> fn (__VA_ARGS__); \
        break; \
    case 4: \
        if (d.bits_per_channel == 8) return ((Drawing<rgba32>*)d.handle)-> fn (__VA_ARGS__); \
        else if(d.bits_per_channel == 16) return ((Drawing<rgba64>*)d.handle)-> fn (__VA_ARGS__); \
        break; \
    }\
    throw std::runtime_error("bad drawing"); } while(0)

#ifdef __cplusplus
static agg::path_storage _get_path(drawing d){
    switch(d.channels){
    case 1:
        if (d.bits_per_channel == 8) return (agg::path_storage)(*((Drawing<gray8>*)d.handle));
        else if(d.bits_per_channel == 16) return (agg::path_storage)(*((Drawing<gray16>*)d.handle));
        break;
    case 3:
        if (d.bits_per_channel == 8) return (agg::path_storage)(*((Drawing<rgb24>*)d.handle));
        else if(d.bits_per_channel == 16) return (agg::path_storage)(*((Drawing<rgb48>*)d.handle));
        break;
    case 4:
        if (d.bits_per_channel == 8) return (agg::path_storage)(*((Drawing<rgba32>*)d.handle));
        else if(d.bits_per_channel == 16) return (agg::path_storage)(*((Drawing<rgba64>*)d.handle));
        break;
    }

    throw std::runtime_error("bad drawing");
}
#endif

drawing draw_create(int64_t width, int64_t height, int channels, uint8_t *data){
    drawing d;
    d.handle = NULL;

    if (channels == 1){
        d.handle = (new Drawing<gray8>(width, height, channels, data));
        d.channels = 3;
        d.bits_per_channel = 8;
    } else if (channels == 3){
        d.handle = (new Drawing<rgb24>(width, height, channels, data));
        d.channels = 3;
        d.bits_per_channel = 8;
    } else if (channels == 4) {
        d.handle = (new Drawing<rgba32>(width, height, channels, data));
        d.channels = 4;
        d.bits_per_channel = 8;
    }

    return d;
}

drawing draw_create16(int64_t width, int64_t height, int channels, uint8_t *data){
    drawing d;
    d.handle = NULL;

    if (channels == 1){
        d.handle = (new Drawing<gray16>(width, height, channels, data));
        d.channels = 3;
        d.bits_per_channel = 16;
    } else if (channels == 3){
        d.handle = (new Drawing<rgb48>(width, height, channels, data));
        d.channels = 3;
        d.bits_per_channel = 16;
    } else if (channels == 4) {
        d.handle = (new Drawing<rgba64>(width, height, channels, data));
        d.channels = 4;
        d.bits_per_channel = 16;
    }

    return d;
}

void draw_free(drawing d){
    if (!d.handle){
        return;
    }
    switch(d.channels){
    case 1:
        if (d.bits_per_channel == 8) delete (Drawing<gray8>*)d.handle;
        else if (d.bits_per_channel == 16) delete (Drawing<gray8>*)d.handle;
        break;
    case 3:
        if (d.bits_per_channel == 8) delete (Drawing<rgb24>*)d.handle;
        else if (d.bits_per_channel == 16) delete (Drawing<rgb48>*)d.handle;
        break;
    case 4:
        if (d.bits_per_channel == 8) delete (Drawing<rgba32>*)d.handle;
        else if (d.bits_per_channel == 16) delete (Drawing<rgba64>*)d.handle;
        break;
    }
}

bool draw_getAntialias(drawing d){
    DRAWING(d, antialias);
}

void draw_setAntialias(drawing d, bool a){
    DRAWING(d, antialias, a);
}

void draw_setLineWidth(drawing d, double w){
    DRAWING(d, lineWidth, w);
}

double draw_getLineWidth(drawing d){
    DRAWING(d, lineWidth);
}

void draw_setMiterLimit(drawing d, double m){
    DRAWING(d, miterLimit, m);
}

double draw_getMiterLimit(drawing d){
    DRAWING(d, miterLimit);
}

void draw_setLineJoin(drawing d, line_join_style lj){
    DRAWING(d, lineJoin, lj);
}

line_join_style draw_getLineJoin(drawing d){
    DRAWING(d, lineJoin);
}

void draw_setLineCap(drawing d, line_cap_style lc){
    DRAWING(d, lineCap, lc);
}

line_cap_style draw_getLineCap(drawing d){
    DRAWING(d, lineCap);
}

void draw_setActivePath(drawing d, unsigned int p){
    DRAWING(d, activePath, p);
}

unsigned int draw_getActivePath(drawing d){
    DRAWING(d, activePath);
}

unsigned int draw_newPath(drawing d){
    DRAWING(d, newPath);
}

void draw_rotate(drawing d, double angle){
    DRAWING(d, rotate, angle);
}

void draw_translate(drawing d, double x, double y){
    DRAWING(d, translate, x, y);
}

void draw_scale(drawing d, double n){
    DRAWING(d, scale, n);
}

void draw_scale2(drawing d, double x, double y){
    DRAWING(d, scale, x, y);
}

void draw_skew(drawing d, double x, double y){
    DRAWING(d, skew, x, y);
}

void draw_clearTransforms(drawing d){
    DRAWING(d, clearTransforms);
}

void draw_closePolygon(drawing d){
    DRAWING(d, closePolygon);
}

void draw_endPolygon(drawing d){
    DRAWING(d, endPolygon);
}

void draw_invertPolygon(drawing d){
    DRAWING(d, invertPolygon);
}

void draw_reset(drawing d){
    DRAWING(d, reset);
}

void draw_clear(drawing d, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    DRAWING(d, clear, r, g, b, a);
}

void draw_removePaths(drawing d){
    DRAWING(d, removePaths);
}

void draw_ellipse(drawing d, double a, double b, double c, double e){
    DRAWING(d, ellipse, a, b, c, e);
}

void draw_rect(drawing d, double x, double y , double w, double h, double c){
    DRAWING(d, rect, x, y, w, h, c);
}

void draw_clip(drawing d, double x, double y, double w, double h){
    DRAWING(d, clip, x, y, w, h);
}

void draw_resetClip(drawing d){
    DRAWING(d, resetClip);
}

double draw_lastX(drawing d){
    DRAWING(d, lastX);
}

double draw_lastY(drawing d){
    DRAWING(d, lastY);
}

void draw_relToAbs(drawing d, double *x, double *y){
    DRAWING(d, relToAbs, x, y);
}

void draw_moveTo(drawing d, double x, double y){
    DRAWING(d, moveTo, x, y);
}

void draw_moveRel(drawing d, double x, double y){
    DRAWING(d, moveRel, x, y);
}

void draw_lineTo(drawing d, double x, double y){
    DRAWING(d, lineTo, x, y);
}

void draw_lineRel(drawing d, double x, double y){
    DRAWING(d, lineRel, x, y);
}

void draw_curveTo2(drawing d, double x, double y){
    DRAWING(d, curveTo, x, y);
}

void draw_curveRel2(drawing d, double x, double y){
    DRAWING(d, curveRel, x, y);
}

void draw_curveTo4(drawing d, double x, double y, double x1, double y1){
    DRAWING(d, curveTo, x, y, x1, y1);
}

void draw_curveRel4(drawing d, double x, double y, double x1, double y1){
    DRAWING(d, curveRel, x, y, x1, y1);
}

void draw_curveTo6(drawing d, double x, double y, double x1, double y1, double a, double b){
    DRAWING(d, curveTo, x, y, x1, y1, a, b);
}

void draw_curveRel6(drawing d, double x, double y, double x1, double y1, double a, double b){
    DRAWING(d, curveRel, x, y, x1, y1, a, b);
}

void draw_arcTo(drawing d, double x, double y, double x1, double y1, double a){
    DRAWING(d, arcTo, x, y, x1, y1, a, false, false);
}

void draw_arcRel(drawing d, double x, double y, double x1, double y1, double a){
    DRAWING(d, arcRel, x, y, x1, y1, a, false, false);
}

double draw_putTextSimple(drawing d, double x, double y, const char *str, int size, double width, const char *font){
    DRAWING(d, putTextSimple, x, y, str, size, width, font);
}

#ifndef NO_FREETYPE
double draw_putText(drawing d, double x, double y, const char *str, const char * font, double w, double h){
    DRAWING(d, putText, x, y, str, font, w, h);
}
#endif

void draw_setColor(drawing d, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    DRAWING(d, setColor, r, g, b, a);
}

void draw_fill(drawing d){
    DRAWING(d, fill);
}

void draw_stroke(drawing d){
    DRAWING(d, stroke);
}

void draw_dash(drawing d, double a, double b){
    DRAWING(d, dash, a, b);
}

void draw_paint(drawing d){
    DRAWING(d, paint);
}

void draw_autoClose(drawing d, bool c){
    DRAWING(d, autoClose, c);
}

bool draw_inPath(drawing d, double x, double y){
    DRAWING(d, inPath, x, y);
}

unsigned int draw_getVertex(drawing d, unsigned int idx, double *x, double *y){
    DRAWING(d, getVertex, idx, x, y);
}

unsigned int draw_nextVertex(drawing d, double *x, double *y){
    DRAWING(d, getVertex, x, y);
}

unsigned int draw_getCommand(drawing d, unsigned int idx){
    DRAWING(d, getCommand, idx);
}

unsigned int draw_lastVertex(drawing d, double *x, double *y){
    DRAWING(d, lastVertex, x, y);
}

unsigned int draw_prevVertex(drawing d, double *x, double *y){
    DRAWING(d, prevVertex, x, y);
}

void draw_modifyVertex(drawing d, unsigned int idx, double x, double y, unsigned int cmd){
    DRAWING(d, modifyVertex, idx, x, y, cmd);
}

unsigned int draw_totalVertices(drawing d){
    DRAWING(d, totalVertices);
}

void draw_join(drawing a, drawing b){
    agg::path_storage pth = _get_path(b);
    DRAWING(a, joinPath, pth);
}

void draw_concat(drawing a, drawing b){
    agg::path_storage pth = _get_path(b);
    DRAWING(a, concatPath, pth);
}

// NOTE: fill/stroke pattern only implemented for rgb8 type drawings
void draw_fillPattern (drawing d, int64_t width, int64_t height, int channels, uint8_t *data){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8) return ((Drawing<rgb24>*)d.handle)->fillPattern<Color>(width, height, channels, data);
        break;
    case 4:
        if (d.bits_per_channel == 8) return ((Drawing<rgba32>*)d.handle)->fillPattern<Color>(width, height, channels, data);
        break;
    }
}

void draw_strokePattern (drawing d, int64_t width, int64_t height, int channels, uint8_t *data){
    switch(d.channels){
    case 3:
        if (d.bits_per_channel == 8) return ((Drawing<rgb24>*)d.handle)->strokePattern<Color>(width, height, channels, data);
        break;
    case 4:
        if (d.bits_per_channel == 8) return ((Drawing<rgba32>*)d.handle)->strokePattern<Color>(width, height, channels, data);
        break;
    }
}

