#ifndef TWOMBLY_C_HEADER
#define TWOMBLY_C_HEADER
#include <stdint.h>
#include <stdbool.h>

#include "draw.hpp"



#ifdef __cplusplus
struct drawing {
    void *handle;
    int channels;
    int bits_per_channel;
    bool is_bgr;
};
typedef struct drawing drawing;

using namespace tw;
extern "C" {
#endif

#define draw_frombimage(im) (im.depth == u16 ? draw_create(im.width, im.height, im.channels, im.u16) : draw_create(im.width, im.height, im.channels, im.u8))

typedef float* Pixel; //  __attribute__ ((vector_size (16)));

drawing draw_create(int64_t width, int64_t height, int channels, uint8_t *data);
drawing draw_create_bgr(int64_t width, int64_t height, int channels, uint8_t *data);
drawing draw_create16(int64_t width, int64_t height, int channels, uint16_t *data);
drawing draw_create16_bgr(int64_t width, int64_t height, int channels, uint16_t *data);

void draw_free(drawing *d);

bool draw_getAntialias(drawing);
void draw_setAntialias(drawing, bool);
void draw_setLineWidth(drawing, double);
double draw_getLineWidth(drawing);
void draw_setMiterLimit(drawing, double);
double draw_getMiterLimit(drawing);
void draw_setLineJoin(drawing, line_join_style);
line_join_style draw_getLineJoin(drawing);
void draw_setLineCap(drawing, line_cap_style);
line_cap_style draw_getLineCap(drawing);
void draw_setActivePath(drawing, unsigned int);
unsigned int draw_getActivePath(drawing);
unsigned int draw_newPath(drawing);
void draw_rotate(drawing, double);
void draw_translate(drawing, double, double);
void draw_scale(drawing, double);
void draw_scale2(drawing, double, double);
void draw_skew(drawing, double, double);
void draw_clearTransforms(drawing);
void draw_closePolygon(drawing);
void draw_endPolygon(drawing);
void draw_invertPolygon(drawing);
void draw_reset(drawing);
void draw_clear(drawing, uint8_t, uint8_t, uint8_t, uint8_t);
void draw_removePaths(drawing);
void draw_ellipse(drawing, double, double, double, double);
void draw_rect(drawing, double, double, double, double, double);
void draw_clip(drawing, double, double, double, double);
void draw_resetClip(drawing);
double draw_lastX(drawing);
double draw_lastY(drawing);
void draw_relToAbs(drawing, double *, double *);
void draw_moveTo(drawing, double, double);
void draw_moveRel(drawing, double, double);
void draw_lineTo(drawing, double, double);
void draw_lineRel(drawing, double, double);
void draw_hLineTo(drawing d, double a);
void draw_hLineRel(drawing d, double a);
void draw_vLineTo(drawing d, double a);
void draw_vLineRel(drawing d, double a);
void draw_curveTo2(drawing, double, double);
void draw_curveRel2(drawing, double, double);
void draw_curveTo4(drawing, double, double, double, double);
void draw_curveRel4(drawing, double, double, double, double);
void draw_curveTo6(drawing, double, double, double, double, double, double);
void draw_curveRel6(drawing, double, double, double, double, double, double);
void draw_arcTo(drawing, double, double, double, double, double);
void draw_arcRel(drawing, double, double, double, double, double);
double draw_putTextSimple(drawing, double, double, const char *, int, double, const char *);
#ifndef NO_FREETYPE
double draw_putText(drawing, double, double, const char *, const char *, double, double);
#endif
void draw_setColor(drawing, uint8_t, uint8_t, uint8_t, uint8_t);
void draw_fill(drawing);
void draw_stroke(drawing);
void draw_dash(drawing, double, double);
void draw_paint(drawing);
void draw_autoClose(drawing, bool);
bool draw_inPath(drawing, double, double);
unsigned int draw_getVertex(drawing, unsigned int, double *, double *);
unsigned int draw_nextVertex(drawing, double *, double *);
unsigned int draw_getCommand(drawing, unsigned int);
unsigned int draw_lastVertex(drawing, double *, double *);
unsigned int draw_prevVertex(drawing, double *, double *);
void draw_modifyVertex(drawing, unsigned int, double, double, unsigned int);
unsigned int draw_totalVertices(drawing);

void draw_alphaLayerInit(drawing a);
void draw_alphaLayerFree(drawing a);
void draw_alphaLayerFill(drawing a, uint8_t v);
uint8_t draw_alphaLayerGet(drawing a, int32_t x, int32_t y);
uint8_t *draw_alphaLayerPtr(drawing a);
uint8_t *draw_alphaLayerPtrOffs(drawing a, int32_t x, int32_t y);

void draw_join(drawing, drawing);
void draw_concat(drawing, drawing);
void draw_fillPattern (drawing d, drawing e);
void draw_strokePattern (drawing d, drawing e);

typedef struct transform_matrix {
    void *handle;
} transform_matrix;

void draw_transformMatrixScale(transform_matrix mtx, double a, double b);
void draw_transformMatrixTranslate(transform_matrix mtx, double a, double b);
void draw_transformMatrixRotate(transform_matrix mtx, double a);
void draw_transformMatrixReset(transform_matrix mtx);
transform_matrix draw_getTransformMatrix(drawing d);
transform_matrix draw_transformMatrixCreate();
void draw_transformMatrixFree(transform_matrix *mtx);
double draw_transformMatrixDeterminant(transform_matrix mtx);
void draw_transformMatrixInverseTransform(transform_matrix mtx, double *x, double *y);
void draw_transformMatrixTransform(transform_matrix mtx, double *x, double *y);
void draw_transformMatrixToDouble(transform_matrix mtx, double *d);
void draw_transformMatrixFromDouble(transform_matrix mtx, double *d);

void draw_fillLinearGradientH(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix mtx);
void draw_fillLinearGradientV(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix mtx);
void draw_fillRadialGradient(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix mtx);
void draw_strokeLinearGradientH(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix mtx);
void draw_strokeLinearGradientV(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix mtx);
void draw_strokeRadialGradient(drawing d, Pixel b, Pixel m, Pixel e, int s, int x, transform_matrix mtx);

#ifdef __cplusplus
}
#endif

#endif //TWOMBLY_C_HEADER
