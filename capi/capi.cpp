#include "twombly.h"

#ifdef __cplusplus
using namespace tw;
#endif

#ifdef __cplusplus
static agg::path_storage *_get_path(drawing* d){
    return (agg::path_storage*)d->handle;
}
#endif

drawing* draw_empty(){
    drawing *d = (drawing*)malloc(sizeof(drawing));
    if (!d){
        return NULL;
    }
    d->handle = new Drawing<rgba32>();
    d->bits_per_channel = 0;
    d->channels = 0;
    return d;
}

drawing* draw_create(int32_t width, int32_t height, int channels, uint8_t *data){
    drawing* d = (drawing*)malloc(sizeof(drawing));
    if (!d){
        return NULL;
    }
    d->handle = NULL;
    d->user_data = NULL;
    d->is_bgr = false;

    if (channels == 1){
        d->handle = (new Drawing<gray8>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 8;
    } else if (channels == 3){
        d->handle = (new Drawing<rgb24>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 8;
    } else if (channels == 4) {
        d->handle = (new Drawing<rgba32>(width, height, channels, data));
        d->channels = 4;
        d->bits_per_channel = 8;
    }

    return d;
}

drawing* draw_create_bgr(int32_t width, int32_t height, int channels, uint8_t *data){
    drawing* d = (drawing*)malloc(sizeof(drawing));
    if (!d){
        return NULL;
    }
    d->handle = NULL;
    d->is_bgr = true;

    if (channels == 1){
        d->handle = (new Drawing<gray8>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 8;
    } else if (channels == 3){
        d->handle = (new Drawing<bgr24>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 8;
    } else if (channels == 4) {
        d->handle = (new Drawing<bgra32>(width, height, channels, data));
        d->channels = 4;
        d->bits_per_channel = 8;
    }

    return d;
}

drawing* draw_create16(int32_t width, int32_t height, int channels, uint16_t *data){
    drawing* d = (drawing*)malloc(sizeof(drawing));
    if (!d){
        return NULL;
    }

    d->handle = NULL;
    d->is_bgr = false;

    if (channels == 1){
        d->handle = (new Drawing<gray16>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 16;
    } else if (channels == 3){
        d->handle = (new Drawing<rgb48>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 16;
    } else if (channels == 4) {
        d->handle = (new Drawing<rgba64>(width, height, channels, data));
        d->channels = 4;
        d->bits_per_channel = 16;
    }

    return d;
}

drawing* draw_create16_bgr(int32_t width, int32_t height, int channels, uint16_t *data){
    drawing* d = (drawing*)malloc(sizeof(drawing));
    if (!d){
        return NULL;
    }

    d->handle = NULL;
    d->is_bgr = true;

    if (channels == 1){
        d->handle = (new Drawing<gray16>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 16;
    } else if (channels == 3){
        d->handle = (new Drawing<bgr48>(width, height, channels, data));
        d->channels = 3;
        d->bits_per_channel = 16;
    } else if (channels == 4) {
        d->handle = (new Drawing<bgra64>(width, height, channels, data));
        d->channels = 4;
        d->bits_per_channel = 16;
    }

    return d;
}

void draw_free(drawing *d){
    if (!d || !d->handle){
        return;
    }

    switch(d->channels){
    case 1:
        if (d->bits_per_channel == 8) delete (Drawing<gray8>*)d->handle;
        else if (d->bits_per_channel == 16) delete (Drawing<gray8>*)d->handle;
        break;
    case 3:
        if (d->bits_per_channel == 8) delete (Drawing<rgb24>*)d->handle;
        else if (d->bits_per_channel == 16) delete (Drawing<rgb48>*)d->handle;
        break;
    case 4:
        if (d->bits_per_channel == 8) delete (Drawing<rgba32>*)d->handle;
        else if (d->bits_per_channel == 16) delete (Drawing<rgba64>*)d->handle;
        break;
    default:
        return;
    }

    d->handle = NULL;
    free(d);
}

bool draw_get_preserve(drawing* d){
    DRAWING(d, preserve);

    // default
    return false;
}

void draw_set_preserve(drawing* d, bool p){
    DRAWING(d, preserve, p);
}

bool draw_get_antialias(drawing* d){
    DRAWING(d, antialias);

    // default
    return false;
}

void draw_set_antialias(drawing* d, bool a){
    DRAWING(d, antialias, a);
}

void draw_set_line_width(drawing* d, double w){
    DRAWING(d, line_width, w);
}

double draw_get_line_width(drawing* d){
    DRAWING(d, line_width);

    // default
    return -1;
}

void draw_set_miter_limit(drawing* d, double m){
    DRAWING(d, miter_limit, m);
}

double draw_get_miter_limit(drawing* d){
    DRAWING(d, miter_limit);

    // default
    return -1;
}

void draw_set_line_join(drawing* d, line_join_style lj){
    DRAWING(d, line_join, lj);
}

line_join_style draw_get_line_join(drawing* d){
    DRAWING(d, line_join);

    return miter_join;
}

void draw_set_line_cap(drawing* d, line_cap_style lc){
    DRAWING(d, line_cap, lc);
}

line_cap_style draw_get_line_cap(drawing* d){
    DRAWING(d, line_cap);

    return butt_cap;
}

void draw_set_active_path(drawing* d, unsigned int p){
    DRAWING(d, active_path, p);
}

unsigned int draw_get_active_path(drawing* d){
    DRAWING(d, active_path);

    return 0;
}

unsigned int draw_new_path(drawing* d){
    DRAWING(d, new_path);
    return 0;
}

void draw_rotate(drawing* d, double angle){
    DRAWING(d, rotate, angle);
}

void draw_translate(drawing* d, double x, double y){
    DRAWING(d, translate, x, y);
}

void draw_scale(drawing* d, double n){
    DRAWING(d, scale, n);
}

void draw_scale2(drawing* d, double x, double y){
    DRAWING(d, scale, x, y);
}

void draw_skew(drawing* d, double x, double y){
    DRAWING(d, skew, x, y);
}

void draw_clear_transforms(drawing* d){
    DRAWING(d, clear_transforms);
}

void draw_close_polygon(drawing* d){
    _get_path(d)->close_polygon();
}

void draw_end_polygon(drawing* d){
    _get_path(d)->end_poly();
}

void draw_invert_polygon(drawing* d){
    DRAWING(d, invert_polygon);
}

void draw_reset(drawing* d){
    DRAWING(d, reset);
}

void draw_clear(drawing* d, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    DRAWING(d, clear, r, g, b, a);
}

void draw_remove_all(drawing* d){
    _get_path(d)->remove_all();
}

void draw_ellipse(drawing* d, double a, double b, double c, double e){
    DRAWING(d, ellipse, a, b, c, e);
}

void draw_rect(drawing* d, double x, double y , double w, double h, double c){
    DRAWING(d, rect, x, y, w, h, c);
}

void draw_clip(drawing* d, double x, double y, double w, double h){
    DRAWING(d, clip, x, y, w, h);
}

void draw_reset_clip(drawing* d){
    DRAWING(d, reset_clip);
}

double draw_last_x(drawing* d){
    if (!d->handle){
        return -1;
    }

    return _get_path(d)->last_x();
}

double draw_last_y(drawing* d){
    if (!d->handle){
        return -1;
    }

    return _get_path(d)->last_y();
}

void draw_rel_to_abs(drawing* d, double *x, double *y){
    _get_path(d)->rel_to_abs(x, y);
}

void draw_move_to(drawing* d, double x, double y){
    _get_path(d)->move_to(x, y);
}

void draw_move_rel(drawing* d, double x, double y){
    _get_path(d)->move_rel(x, y);
}

void draw_line_to(drawing* d, double x, double y){
    _get_path(d)->line_to(x, y);
}

void draw_line_rel(drawing* d, double x, double y){
    _get_path(d)->line_rel(x, y);
}

void draw_hline_to(drawing* d, double a){
    _get_path(d)->hline_to(a);
}

void draw_hline_rel(drawing* d, double a){
    _get_path(d)->hline_rel(a);
}

void draw_vline_to(drawing* d, double a){
    _get_path(d)->vline_to(a);
}

void draw_vline_rel(drawing* d, double a){
    _get_path(d)->vline_rel(a);
}

void draw_curve_to2(drawing* d, double x, double y){
    DRAWING(d, curve_to, x, y);
}

void draw_curve_rel2(drawing* d, double x, double y){
    DRAWING(d, curve_rel, x, y);
}

void draw_curve_to4(drawing* d, double x, double y, double x1, double y1){
    DRAWING(d, curve_to, x, y, x1, y1);
}

void draw_curve_rel4(drawing* d, double x, double y, double x1, double y1){
    DRAWING(d, curve_rel, x, y, x1, y1);
}

void draw_curve_to6(drawing* d, double x, double y, double x1, double y1, double a, double b){
    DRAWING(d, curve_to, x, y, x1, y1, a, b);
}

void draw_curve_rel6(drawing* d, double x, double y, double x1, double y1, double a, double b){
    DRAWING(d, curve_rel, x, y, x1, y1, a, b);
}

void draw_arc_to(drawing* d, double x, double y, double x1, double y1, double a){
    DRAWING(d, arc_to, x, y, x1, y1, a, false, false);
}

void draw_arc_rel(drawing* d, double x, double y, double x1, double y1, double a){
    DRAWING(d, arc_rel, x, y, x1, y1, a, false, false);
}

double draw_text_simple(drawing* d, double x, double y, const char *str, int size, double width, const char *font){
    DRAWING(d, text_simple, x, y, str, size, width, font);

    return -1;
}

double draw_text(drawing* d, double x, double y, const char *str, const char * font, double w, double h){
#ifndef NO_FREETYPE
    DRAWING(d, text, x, y, str, font, w, h);
#endif
    return -1;
}

void draw_set_color(drawing* d, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    DRAWING(d, set_color, r, g, b, a);
}

void draw_fill_color(drawing* d, float* p){
    DRAWING(d, fill, Color(p[0], p[1], p[2], p[3]));
}

void draw_fill(drawing* d){
    DRAWING(d, fill);
}

void draw_stroke_color(drawing* d, float* p){
    DRAWING(d, stroke, Color(p[0], p[1], p[2], p[3]));
}

void draw_stroke(drawing* d){
    DRAWING(d, stroke);
}

void draw_dash(drawing* d, double a, double b){
    DRAWING(d, dash, a, b);
}

void draw_dash_color(drawing* d, float* p, double x, double y){
    DRAWING(d, dash, Color(p[0], p[1], p[2], p[3]), x, y);
}

void draw_paint(drawing* d){
    DRAWING(d, paint);
}

void draw_auto_close(drawing* d, bool c){
    DRAWING(d, auto_close, c);
}

bool draw_in_path(drawing* d, double x, double y){
    DRAWING(d, in_path, x, y);
    return false;
}

bool draw_is_drawn(drawing* d, double x, double y){
    DRAWING(d, is_drawn, x, y);
    return false;
}

unsigned int draw_get_vertex(drawing* d, unsigned int idx, double *x, double *y){
    DRAWING(d, vertex, idx, x, y);
    return 0;
}

unsigned int draw_next_vertex(drawing* d, double *x, double *y){
    DRAWING(d, vertex, x, y);
    return 0;
}

unsigned int draw_get_command(drawing* d, unsigned int idx){
    DRAWING(d, command, idx);
    return 0;
}

unsigned int draw_last_vertex(drawing* d, double *x, double *y){
    DRAWING(d, last_vertex, x, y);
    return 0;
}

unsigned int draw_prev_vertex(drawing* d, double *x, double *y){
    DRAWING(d, prev_vertex, x, y);
    return 0;
}

void draw_modify_vertex(drawing* d, unsigned int idx, double x, double y, unsigned int cmd){
    DRAWING(d, modify_vertex, idx, x, y, cmd);
}

unsigned int draw_total_vertices(drawing* d){
    DRAWING(d, total_vertices);
    return 0;
}

void draw_join(drawing* a, drawing* b){
    agg::path_storage *pth = _get_path(b);
    DRAWING(a, join_path, (*pth));
}

void draw_concat(drawing* a, drawing* b){
    agg::path_storage *pth = _get_path(b);
    DRAWING(a, concat_path, (*pth));
}

void draw_alpha_mask_init(drawing* a){
    DRAWING(a, alpha_mask_init);
}

void draw_alpha_mask_free(drawing* a){
    DRAWING(a, alpha_mask_free);
}

void draw_alpha_mask_fill(drawing* a, uint8_t v){
    DRAWING(a, alpha_mask_fill, v);
}

void draw_alpha_mask_set(drawing* a, int32_t x, int32_t y, uint8_t val){
    DRAWING(a, alpha_mask_set, x, y, val);
}

uint8_t draw_alpha_mask_get(drawing* a, int32_t x, int32_t y){
    DRAWING(a, alpha_mask_get, x, y);
    return 0;
}

uint8_t *draw_alpha_mask_ptr(drawing* a){
    DRAWING(a, alpha_mask_ptr);
    return nullptr;
}

uint8_t *draw_alpha_mask_ptr_offs(drawing* a, int32_t x, int32_t y){
    DRAWING(a, alpha_mask_ptr_offs, x, y);
    return nullptr;
}
