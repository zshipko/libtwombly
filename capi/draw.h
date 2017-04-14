#ifndef TWOMBLY_C_HEADER
#define TWOMBLY_C_HEADER
#include <stdint.h>
#include <stdbool.h>

#include "draw.hpp"

#ifndef TWOMBLY_SET_LOG
#define TWOMBLY_SET_LOG 1
#endif

#ifdef __cplusplus
using namespace tw;
extern "C" {
#endif

// C interface
#ifndef __cplusplus
typedef enum line_cap_style line_cap_style;
typedef enum line_join_style line_join_style;
typedef enum filling_rule filling_rule;
typedef enum path_commands path_commands;
typedef enum gradient_type gradient_type;

typedef struct Point {
    double x, y;
} Point;

typedef struct Rectangle {
    double x, y, width, height;
} Rectangle;
#endif

struct drawing {
    void *handle;
    int channels;
    int bits_per_channel;
    bool is_bgr;
    void *user_data;
};
typedef struct drawing drawing;

#define TWOMBLY_LOG(s, ...) if(TWOMBLY_SET_LOG) fprintf(stderr, s,  __VA_ARGS__)

typedef float* Pixel;

drawing draw_create(int64_t width, int64_t height, int channels, uint8_t *data);
drawing draw_create_bgr(int64_t width, int64_t height, int channels, uint8_t *data);
drawing draw_create16(int64_t width, int64_t height, int channels, uint16_t *data);
drawing draw_create16_bgr(int64_t width, int64_t height, int channels, uint16_t *data);
drawing draw_create_path();
drawing draw_empty();

void draw_free(drawing *d);

bool draw_get_preserve(drawing);
void draw_set_preserve(drawing, bool);
bool draw_get_antialias(drawing);
void draw_set_antialias(drawing, bool);
void draw_set_line_width(drawing, double);
double draw_get_line_width(drawing);
void draw_set_miter_limit(drawing, double);
double draw_get_miter_limit(drawing);
void draw_set_line_join(drawing, line_join_style);
line_join_style draw_get_line_join(drawing);
void draw_set_line_cap(drawing, line_cap_style);
line_cap_style draw_get_line_cap(drawing);
void draw_set_active_path(drawing, unsigned int);
unsigned int draw_get_active_path(drawing);
unsigned int draw_new_path(drawing);
void draw_rotate(drawing, double);
void draw_translate(drawing, double, double);
void draw_scale(drawing, double);
void draw_scale2(drawing, double, double);
void draw_skew(drawing, double, double);
void draw_clear_transforms(drawing);
void draw_close_polygon(drawing);
void draw_end_polygon(drawing);
void draw_invert_polygon(drawing);
void draw_reset(drawing);
void draw_clear(drawing, uint8_t, uint8_t, uint8_t, uint8_t);
void draw_remove_all(drawing);
void draw_ellipse(drawing, double, double, double, double);
void draw_rect(drawing, double, double, double, double, double);
void draw_clip(drawing, double, double, double, double);
void draw_reset_clip(drawing);
double draw_last_x(drawing);
double draw_last_y(drawing);
void draw_rel_to_abs(drawing, double *, double *);
void draw_move_to(drawing, double, double);
void draw_move_rel(drawing, double, double);
void draw_line_to(drawing, double, double);
void draw_line_rel(drawing, double, double);
void draw_hline_to(drawing d, double a);
void draw_hline_rel(drawing d, double a);
void draw_vline_to(drawing d, double a);
void draw_vline_rel(drawing d, double a);
void draw_curve_to2(drawing, double, double);
void draw_curve_rel2(drawing, double, double);
void draw_curve_to4(drawing, double, double, double, double);
void draw_curve_rel4(drawing, double, double, double, double);
void draw_curve_to6(drawing, double, double, double, double, double, double);
void draw_curve_rel6(drawing, double, double, double, double, double, double);
void draw_arc_to(drawing, double, double, double, double, double);
void draw_arc_rel(drawing, double, double, double, double, double);
double draw_text_simple(drawing, double, double, const char *, int, double, const char *);
#ifndef NO_FREETYPE
double draw_text(drawing, double, double, const char *, const char *, double, double);
#endif
void draw_set_color(drawing, uint8_t, uint8_t, uint8_t, uint8_t);
void draw_fill(drawing);
void draw_fill_color(drawing, Pixel);
void draw_stroke(drawing);
void draw_stroke_color(drawing, Pixel);
void draw_dash(drawing, double, double);
void draw_dash_color(drawing, Pixel, double, double);
void draw_paint(drawing);
void draw_auto_close(drawing, bool);
bool draw_in_path(drawing, double, double);
bool draw_is_drawn(drawing d, double x, double y);
unsigned int draw_get_vertex(drawing, unsigned int, double *, double *);
unsigned int draw_next_vertex(drawing, double *, double *);
unsigned int draw_get_command(drawing, unsigned int);
unsigned int draw_last_vertex(drawing, double *, double *);
unsigned int draw_prev_vertex(drawing, double *, double *);
void draw_modify_vertex(drawing, unsigned int, double, double, unsigned int);
unsigned int draw_total_vertices(drawing);

void draw_alpha_mask_init(drawing a);
void draw_alpha_mask_free(drawing a);
void draw_alpha_mask_fill(drawing a, uint8_t v);
void draw_alpha_mask_set(drawing a, int32_t x, int32_t y, uint8_t val);
uint8_t draw_alpha_mask_get(drawing a, int32_t x, int32_t y);
uint8_t *draw_alpha_mask_ptr(drawing a);
uint8_t *draw_alpha_mask_ptr_offs(drawing a, int32_t x, int32_t y);

void draw_join(drawing, drawing);
void draw_concat(drawing, drawing);

typedef struct transform_matrix {
    void *handle;
} transform_matrix;

typedef struct gradient {
    void *handle;
} gradient;

void draw_transform_matrix_scale(transform_matrix mtx, double a, double b);
void draw_transform_matrix_translate(transform_matrix mtx, double a, double b);
void draw_transform_matrix_rotate(transform_matrix mtx, double a);
void draw_transform_matrix_reset(transform_matrix mtx);
transform_matrix draw_get_transform_matrix(drawing d);
transform_matrix draw_transform_matrix_create();
void draw_transform_matrix_free(transform_matrix *mtx);
double draw_transform_matrix_determinant(transform_matrix mtx);
void draw_transform_matrix_inverse_transform(transform_matrix mtx, double *x, double *y);
void draw_transform_matrix_transform(transform_matrix mtx, double *x, double *y);
void draw_transform_matrix_to_double(transform_matrix mtx, double *d);
void draw_transform_matrix_from_double(transform_matrix mtx, double *d);

gradient draw_gradient_create();
void draw_gradient_free(gradient *grad);
void draw_gradient_add_stop(gradient grad, Pixel color);
transform_matrix draw_gradient_get_matrix(gradient grad);
void draw_gradient_set_matrix(gradient grad, transform_matrix mtx);
void draw_fill_gradient(drawing d, gradient grad, int s, int x, gradient_type grad_type);
void draw_stroke_gradient(drawing d, gradient grad, int s, int x, gradient_type grad_type);
gradient draw_gradient_create16();
void draw_gradient_add_stop16(gradient grad, Pixel color);
void draw_fill_gradient16(drawing d, gradient grad, int s, int x, gradient_type grad_type);
void draw_stroke_gradient16(drawing d, gradient grad, int s, int x, gradient_type grad_type);

#define DRAWING(d, fn, ...) do { switch(d.channels){ \
    case 1: \
        if (d.bits_per_channel == 8) return ((Drawing<gray8>*)d.handle)-> fn (__VA_ARGS__); \
        else if(d.bits_per_channel == 16) return ((Drawing<gray16>*)d.handle)-> fn (__VA_ARGS__); \
        break; \
    case 3: \
        if (d.bits_per_channel == 8) return d.is_bgr ? ((Drawing<bgr24>*)d.handle)-> fn (__VA_ARGS__) : ((Drawing<rgb24>*)d.handle)-> fn (__VA_ARGS__); \
        else if(d.bits_per_channel == 16) return d.is_bgr ? ((Drawing<bgr48>*)d.handle)-> fn (__VA_ARGS__) : ((Drawing<rgb48>*)d.handle)-> fn (__VA_ARGS__); \
        break; \
    case 4: \
        if (d.bits_per_channel == 8) return d.is_bgr ? ((Drawing<bgra32>*)d.handle)-> fn (__VA_ARGS__) : ((Drawing<rgba32>*)d.handle)-> fn (__VA_ARGS__); \
        else if(d.bits_per_channel == 16) return d.is_bgr ? ((Drawing<bgra64>*)d.handle)-> fn (__VA_ARGS__) : ((Drawing<rgba64>*)d.handle)-> fn (__VA_ARGS__); \
        break; \
    }\
} while(0)

// bimage
#define draw_frombimage(im) \
    bimageTypeSize(im->type) == 8 ? \
        draw_create(im->width, im->height, bimageTypeChannels(im->type), im->data) : \
        bimageTypeSize(im->type) == 16 ? draw_create16(im->width, im->height, bimageTypeChannels(im->type), im->data) : draw_empty()

// leptonica
#define draw_frompix(im) draw_create(im->w, im->h, im->d/8, (unint8_t*)im->data)


#ifdef __cplusplus
}
#endif

#endif //TWOMBLY_C_HEADER
