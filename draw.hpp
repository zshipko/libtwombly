#ifndef TWOMBLY_DRAW_HEADER
#define TWOMBLY_DRAW_HEADER

// Defined for both C and C++

enum line_cap_style {
    butt_cap,
    square_cap,
    round_cap
};

enum line_join_style {
    miter_join         = 0,
    miter_join_revert  = 1,
    round_join         = 2,
    bevel_join         = 3,
    miter_join_round   = 4
};

enum filling_rule {
    fill_non_zero,
    fill_even_odd
};

enum path_commands {
    path_cmd_stop     = 0,
    path_cmd_move_to  = 1,
    path_cmd_line_to  = 2,
    path_cmd_curve3   = 3,
    path_cmd_curve4   = 4,
    path_cmd_curveN   = 5,
    path_cmd_catrom   = 6,
    path_cmd_ubspline = 7,
    path_cmd_end_poly = 0x0F,
    path_cmd_mask     = 0x0F
};

enum gradient_type {
    gradient_type_circle,
    gradient_type_radial,
    gradient_type_radial_d,
    gradient_type_radial_focus,
    gradient_type_x,
    gradient_type_y,
    gradient_type_diamond,
    gradient_type_xy,
    gradient_type_sqrt_xy,
    gradient_type_conic
};

#define TW_POLY_SIZE(p) (sizeof(p) / (sizeof(*p) * 2))

#ifdef __cplusplus

#include "image.hpp"

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_renderer_base.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_conv_stroke.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_dash.h"
#include "agg_conv_transform.h"
#include "agg_curves.h"
#include "agg_ellipse.h"
#include "agg_blur.h"
#include "agg_bezier_arc.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_path_storage.h"
#include "agg_span_allocator.h"
#include "agg_span_gouraud.h"
#include "agg_span_gradient.h"
#include "agg_span_image_filter.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_pattern_gray.h"
#include "agg_span_pattern_rgb.h"
#include "agg_span_pattern_rgba.h"
#include "agg_rounded_rect.h"
#include "agg_gamma_lut.h"
#include "agg_gsv_text.h"
#include "agg_image_accessors.h"
#include "agg_pixfmt_amask_adaptor.h"
#include "agg_alpha_mask_u8.h"
#include "agg_trans_affine.h"

#ifndef NO_FREETYPE
#include "agg_font_freetype.h"
#endif

namespace tw {

// The Color type is used when dealing with
// colors withing the drawing context. comes in both 8 and 16 bit variations
typedef agg::rgba8 Color;
typedef agg::rgba16 Color16;

inline Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
inline Color rgb(uint8_t r, uint8_t g, uint8_t b);
inline Color16 rgba_16(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
inline Color16 rgb_16(uint8_t r, uint8_t g, uint8_t b);

#ifndef NO_OPENCV
inline Color rgba(Scalar s);
inline Color16 rgba_16(Scalar s);
#endif

template <typename ColorType, int S=256>
class Gradient {
public:
    std::vector<ColorType> colors;
    agg::pod_auto_array<ColorType, S> arr;
    agg::trans_affine mtx;

    Gradient() {}

    Gradient(ColorType beg, ColorType mid, ColorType end){
        addStop(beg);
        addStop(mid);
        addStop(end);
    }

    void addStop(ColorType const & c){
        colors.push_back(c);
    }

    void generate(){
        unsigned stop_len = arr.size() / (colors.size()-1);
        for(size_t color_index = 0; color_index < colors.size()-1; color_index++){
            for(int i = 0; i < stop_len; i++){
                arr[i + color_index*stop_len] = colors[color_index].gradient(colors[color_index+1], (i - stop_len) / double(stop_len));
            }
        }
    }
};

// Drawing type
template <typename DrawingType>
class Drawing : public agg::path_storage {
    // render settings
    bool _antialias;
    double _width, _miterlimit;
    line_cap_style _linecap;
    line_join_style _linejoin;

    // rendering
    agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType>> render_aa;
    agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType>> render_bin;
    agg::renderer_base<DrawingType> base;
    agg::scanline32_p8 *sl;

    unsigned pathid; // stores current path
    Color current_color;

public:
    struct size {
        size(int32_t _x, int32_t _y, int32_t _c) : x(_x), y(_y), c(_c){}
        int32_t x, y, c;
    } size;

    DrawingType pix;
    agg::rendering_buffer buffer;
    agg::rasterizer_scanline_aa<> *raster;

    agg::trans_affine mtx;
    uint8_t *alpha_mask;

    Drawing() : pix(buffer), size(0, 0, 0), sl(nullptr), raster(nullptr), alpha_mask(nullptr) {
        base = agg::renderer_base<DrawingType>(pix);
        render_aa = agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType> >(base);
        render_bin = agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType> >(base);
    }

    // Creates a drawing context from width, height, channels and data
    Drawing(int32_t w, int32_t h, int32_t c, uint8_t *d, uint8_t *_alpha_mask=nullptr) : buffer(d, w, h, w * c), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(w, h, c),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing(int32_t w, int32_t h, int32_t c, uint16_t *d, uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)d, w, h, w * c * 2), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(w, h, c),  alpha_mask(_alpha_mask) {
        alloc();
    }

#ifndef NO_OPECV
    // Creates a drawing context from standard OpenCV Mat types
    Drawing(Mat3b &im, uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im.data, im.cols, im.rows, im.cols * im.channels()),pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im.cols, im.rows, im.channels()),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing(Mat4b &im, uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im.data, im.cols, im.rows, im.cols * im.channels()), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im.cols, im.rows, im.channels()),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing(Mat3w &im, uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im.data, im.cols, im.rows, im.cols * im.channels() * 2),pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im.cols, im.rows, im.channels()),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing(Mat4w &im,  uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im.data, im.cols, im.rows, im.cols * im.channels() * 2), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im.cols, im.rows, im.channels()),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing(Mat &im,  uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im.data, im.cols, im.rows, im.cols * im.channels() * sizeof(im.data[0])), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im.cols, im.rows, im.channels()),  alpha_mask(_alpha_mask) {
        alloc();
    }
#endif

#ifdef bimage_header_file
    Drawing (bimage im,  uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im.ptr, im.width, im.height, im.width * im.channels * (im.depth == u16 ? 2 : 1)), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im.width, im.height, im.channels),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing (bimage *im, uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)im->ptr, im->width, im->height, im->width * im->channels * (im->depth == u16 ? 2 : 1)), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(im->width, im->height, im->channels), alpha_mask(_alpha_mask) {
        alloc();
    }
#endif

    ~Drawing(){
        if (raster){
            delete raster;
            raster = nullptr;
        }

        if (sl){
            delete sl;
            sl = nullptr;
        }

        alphaMaskFree();
    }

    // Actually create the image,
    // this function will overwrite the current context if it already exists
    void alloc(){
        _miterlimit = 1;
        base = agg::renderer_base<DrawingType>(pix);
        render_aa = agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType> >(base);
        render_bin = agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType> >(base);

        if (raster)
            delete raster;

        raster = new agg::rasterizer_scanline_aa<>();

        if (sl)
            delete sl;

        sl = new agg::scanline32_p8();
    }

    void alphaMaskInit(){
        if (alpha_mask == nullptr){
            alpha_mask = new uint8_t[size.x * size.y];
            memset(alpha_mask, 255, size.x * size.y);
        }
    }

    void alphaMaskFill(uint8_t a){
        if (alpha_mask){
            memset(alpha_mask, a, size.x * size.y);
        }
    }

    uint8_t &alphaMaskGet(int32_t x, int32_t y){
        return alpha_mask[(y * size.x) + x];
    }

    uint8_t *alphaMaskPtrOffs(int32_t x, int32_t y){
        return alpha_mask + (y * size.x) + x;
    }

    uint8_t *alphaMaskPtr(){
        return alpha_mask;
    }

    void alphaMaskFree(){
        if (alpha_mask){
            delete[] alpha_mask;
            alpha_mask = nullptr;
        }
    }

    // Render getter/setters
    bool antialias(){
        return _antialias;
    }

    void antialias(bool aa){
        _antialias = aa;
    }

    void lineWidth(double w){
        _width = w;
    }

    double lineWidth(){
        return _width;
    }

    void miterLimit(double m){
        _miterlimit = m;
    }

    double miterLimit(){
        return _miterlimit;
    }

    void lineJoin(line_join_style lj){
        _linejoin = lj;
    }

    line_join_style lineJoin(){
        return _linejoin;
    }

    void lineCap(line_cap_style lc){
        _linecap = lc;
    }

    line_cap_style lineCap(){
        return _linecap;
    }

    // Set the current pathid
    void activePath(unsigned p){
        pathid = p;
    }

    // Get the current pathid;
    unsigned activePath(){
        return pathid;
    }

    // Start new path, returns new pathid
    unsigned newPath(){
        raster->reset();
        pathid = start_new_path();
        return pathid;
    }

    // Set rotation
    inline void rotate(double rad){
        mtx *= agg::trans_affine_rotation(rad);
    }

    // Set translation
    inline void translate(double x, double y){
        mtx *= agg::trans_affine_translation(x, y);
    }

    // Set scale
    inline void scale(double n){
        mtx *= agg::trans_affine_scaling(n);
    }

    inline void scale(double n, double m){
        mtx *= agg::trans_affine_scaling(n, m);
    }

    // Set skew
    void skew(double x, double y){
        mtx *= agg::trans_affine_skewing(x, y);
    }

    // Clear all transformations
    inline void clearTransforms(){
        mtx.reset();
    }

    // joinPath inserts an object into the current path
    template<typename S>
    void joinPath(S &s){
        join_path(s, pathid);
    }

    // concatPath appends a path to the current one
    template<typename S>
    void concatPath(S &s){
        concat_path(s, pathid);
    }

    // End the polygon and close it
    inline void closePolygon(){
        close_polygon();
    }

    // End the current polygon and begin a new one
    inline void endPolygon(){
        end_poly();
    }

    inline void invertPolygon(){
        invert_polygon(pathid);
    }

    template <typename PathType>
    void concatPolygon(PathType *data, size_t n = 0, bool closed = false){
        concat_poly(data, n ? n : TW_POLY_SIZE(data), closed);
    }

    template <typename PathType>
    void joinPolygon(PathType *data, size_t n = 0, bool closed = false){
        join_poly(data,  n ? n : TW_POLY_SIZE(data), closed);
    }

    // Reset everything
    void reset(){
        _width = 1;
        _miterlimit = 1;
        raster->reset();
        resetClip();
        clearTransforms();
        removePaths();
    }

    // Clear drawing context with specific rgba color
    inline void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        base.clear(Color(r, g, b, a));
    }

    inline void clear(uint8_t r, uint8_t g, uint8_t b){
        base.clear(Color(r, g, b));
    }

    inline void removePaths(){
        remove_all();
    }

    // Create shape objects
    void ellipse(double x, double y, double rx, double ry, int n = 100){
        agg::ellipse ell(x, y, rx, ry, n);
        joinPath<agg::ellipse>(ell);
    }

    inline void rect(double x, double y, double x1, double y1, double border_radius=0){
        agg::rounded_rect r(x, y, x1, y1, border_radius);
        concatPath(r);
    }

    inline void clip(double x, double y, double x1, double y1){
        raster->clip_box(x, y, x1, y1);
    }

    inline void resetClip(){
        raster->reset_clipping();
    }

    inline void fillingRule(filling_rule fr){
        raster->filling_rule((agg::filling_rule_e)fr);
    }

    // Get previous position
    inline Point lastPosition(){
        return Point(lastX(), lastY());
    }

    inline double lastX(){
        return last_x();
    }

    inline double lastY(){
        return last_y();
    }

    // Convert relative coordinates to absolute positions
    inline void relToAbs(double *x, double *y){
        rel_to_abs(x, y);
    }

    // Move current cursor to point
    inline void moveTo(double x, double y){
        move_to(x, y);
    }

    inline void moveRel(double x, double y){
        move_rel(x, y);
    }

    inline void lineTo(double x, double y){
        line_to(x, y);
    }

    inline void lineRel(double x, double y){
        line_rel(x, y);
    }

    inline void hLineTo(double a){
        hline_to(a);
    }

    inline void hLineRel(double a){
        hline_rel(a);
    }

    inline void vLineTo(double a){
        vline_to(a);
    }

    inline void vLineRel(double a){
        vline_rel(a);
    }

    inline void curveTo(double x, double y){
        curve3(x, y);
    }

    inline void curveRel(double x, double y){
        curve3_rel(x, y);
    }

    inline void curveTo(double x, double y, double a, double b){
        curve3(a, b, x, y);
    }

    inline void curveRel(double x, double y, double a, double b){
        curve3_rel(a, b, x, y);
    }

    inline void curveTo(double x, double y, double a, double b, double c, double d){
        curve4(a, b, c, d, x, y);
    }

    inline void curveRel(double x, double y, double a, double b, double c, double d){
        curve4_rel(a, b, c, d, x, y);
    }

    inline void arcTo(double x, double y, double rx, double ry, double angle, bool large = false, bool sweep = false){
        arc_to(rx, ry, angle, large, sweep, x, y);
    }

    inline void arcRel(double x, double y, double rx, double ry, double angle, bool large = false, bool sweep = false){
        arc_rel(rx, ry, angle, large, sweep, x, y);
    }

    template <typename ColorType>
    inline void blur(double a){
        agg::recursive_blur<ColorType, agg::recursive_blur_calc_rgb<> > sb;
        sb.blur(pix, a);
    }

    // Draw text without freetype
    double textSimple(double x, double y, const char *txt, int size=50, double width=2.0, const char *font=nullptr, bool flip_y = true){
        agg::gsv_text text;
        agg::gsv_text_outline<agg::trans_affine> outline(text, mtx);

        text.text(txt);
        text.flip(flip_y);
        text.size(size);
        text.start_point(x, y);
        outline.width(width);

        if (font){
            text.load_font(font);
        }

        raster->add_path(outline);
        return text.text_width();
    }

#ifndef NO_FREETYPE
    double text(double x, double y, const char *txt, const char *font, double width, double height){
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
                    concatPath(curves);
                }
                x += glyph->advance_x;
                y += glyph->advance_y;
                ptr++;
            }
        }
        return x-start_x;
    }
#endif // NO_FREETYPE

    // Copy from another drawing of the same type
    void copyFrom(Drawing<DrawingType> &d){
        buffer.copy_from(d.buffer);
    }

    // Set the current rendering color
    inline void setColor(Color c){
        render_aa.color(c);
        render_bin.color(c);
        current_color = c;
    }

    inline void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255){
        setColor(Color(r, g, b, a));
    }

    // Fill/stroke path with another drawing
    template<typename ColorType>
    void fillPattern (Drawing<DrawingType> &d) {
        typedef agg::wrap_mode_repeat wrap_x_type;
        typedef agg::wrap_mode_repeat wrap_y_type;
        typedef agg::image_accessor_wrap<DrawingType, wrap_x_type, wrap_y_type> img_source_type;
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;

        agg::conv_curve<agg::path_storage> pth(*this);
        agg::span_allocator<ColorType> sa;

        // set up image
        img_source_type img_src (d.pix);
        agg::span_pattern_rgb<img_source_type> sg ( img_src, d.size.x, d.size.y);

        // apply transforms
        agg::conv_transform<agg::conv_curve<agg::path_storage>> m(pth, mtx);
        raster->add_path(m, pathid);

        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);
            agg::render_scanlines_aa(*raster, *sl, alpha_base, sa, sg);
         } else {
            agg::render_scanlines_bin(*raster, *sl, base, sa, sg);
         }
    }

    template <typename ColorType>
    void strokePattern (Drawing<DrawingType> &d) {
        typedef agg::wrap_mode_repeat wrap_x_type;
        typedef agg::wrap_mode_repeat wrap_y_type;
        typedef agg::image_accessor_wrap<DrawingType, wrap_x_type, wrap_y_type> img_source_type;
        typedef agg::span_pattern_rgb<img_source_type> span_gen_type;
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;

        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage>> pth(p);
        agg::span_allocator<ColorType> sa;

        // get image set up
        img_source_type img_src (d.pix);
        span_gen_type sg ( img_src, d.size.x, d.size.y);

        // apply settings
        pth.width(_width);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);

        // apply transforms
        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage>>> m(pth, mtx);
        raster->add_path(m, pathid);

        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);
            agg::render_scanlines_aa(*raster, *sl, alpha_base, sa, sg);
         } else {
            agg::render_scanlines_bin(*raster, *sl, base, sa, sg);
         }
    }

    void fill(){
        agg::conv_curve<agg::path_storage> pth(*this);
        agg::conv_transform<agg::conv_curve<agg::path_storage>> m(pth, mtx);
        paint(m);
    }

    void stroke(){
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage>> pth(p);

        pth.width(_width);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);

        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage>>> m(pth, mtx);
        paint(m);
    }

    void dash(double a, double b){
        agg::conv_curve<agg::path_storage> curve(*this);
        agg::conv_dash<agg::conv_curve<agg::path_storage>> p(curve);
        p.add_dash(a, b);
        agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>>> pth(p);

        pth.width(_width);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);

        agg::conv_transform<agg::conv_stroke<agg::conv_dash<agg::conv_curve<agg::path_storage>>>> m(pth, mtx);
        paint(m);
    }

    template<typename ColorType, typename GradientType>
    void fillGradient(ColorType b, ColorType m, ColorType e, int s, int x){
        Gradient<ColorType> grad(b, m, e);
        grad.generate();
        fillGradient<ColorType, GradientType>(grad.arr, s, x, grad.mtx);
    }

    template<typename ColorType, typename GradientType>
    void strokeGradient(ColorType b, ColorType m, ColorType e, int s, int x){
        Gradient<ColorType> grad(b, m, e);
        grad.generate();
        strokeGradient<ColorType, GradientType>(grad.arr, s, x, grad.mtx);
    }

    template <typename ColorType, typename GradientType>
    void strokeGradient(Gradient<ColorType> grad, int s, int x){
        grad.generate();
        strokeGradient<ColorType, GradientType>(grad.arr, s, x, grad.mtx);
    }

    template <typename ColorType, typename GradientType>
    void fillGradient(Gradient<ColorType> grad, int s, int x){
        grad.generate();
        fillGradient<ColorType, GradientType>(grad.arr, s, x, grad.mtx);
    }

    template <typename ColorType>
    void strokeGradient(Gradient<ColorType> grad, int s, int x, gradient_type grad_type){
        grad.generate();
        switch(grad_type){
            case gradient_type_circle:
                strokeGradient<ColorType, agg::gradient_circle>(grad, s, x);
                break;
            case gradient_type_radial:
                strokeGradient<ColorType, agg::gradient_radial>(grad, s, x);
                break;
            case gradient_type_radial_d:
                strokeGradient<ColorType, agg::gradient_radial_d>(grad, s, x);
                break;
            case gradient_type_radial_focus:
                strokeGradient<ColorType, agg::gradient_radial_focus>(grad, s, x);
                break;
            case gradient_type_x:
                strokeGradient<ColorType, agg::gradient_x>(grad, s, x);
                break;
            case gradient_type_y:
                strokeGradient<ColorType, agg::gradient_y>(grad, s, x);
                break;
            case gradient_type_diamond:
                strokeGradient<ColorType, agg::gradient_diamond>(grad, s, x);
                break;
            case gradient_type_xy:
                strokeGradient<ColorType, agg::gradient_xy>(grad, s, x);
                break;
            case gradient_type_sqrt_xy:
                strokeGradient<ColorType, agg::gradient_sqrt_xy>(grad, s, x);
                break;
            case gradient_type_conic:
                strokeGradient<ColorType, agg::gradient_conic>(grad, s, x);
                break;
        }
    }

    template <typename ColorType>
    void fillGradient(Gradient<ColorType> grad, int s, int x, gradient_type grad_type){
        grad.generate();
        switch(grad_type){
            case gradient_type_circle:
                fillGradient<ColorType, agg::gradient_circle>(grad, s, x);
                break;
            case gradient_type_radial:
                fillGradient<ColorType, agg::gradient_radial>(grad, s, x);
                break;
            case gradient_type_radial_d:
                fillGradient<ColorType, agg::gradient_radial_d>(grad, s, x);
                break;
            case gradient_type_radial_focus:
                fillGradient<ColorType, agg::gradient_radial_focus>(grad, s, x);
                break;
            case gradient_type_x:
                fillGradient<ColorType, agg::gradient_x>(grad, s, x);
                break;
            case gradient_type_y:
                fillGradient<ColorType, agg::gradient_y>(grad, s, x);
                break;
            case gradient_type_diamond:
                fillGradient<ColorType, agg::gradient_diamond>(grad, s, x);
                break;
            case gradient_type_xy:
                fillGradient<ColorType, agg::gradient_xy>(grad, s, x);
                break;
            case gradient_type_sqrt_xy:
                fillGradient<ColorType, agg::gradient_sqrt_xy>(grad, s, x);
                break;
            case gradient_type_conic:
                fillGradient<ColorType, agg::gradient_conic>(grad, s, x);
                break;
        }
    }

    template<typename ColorType, typename GradientType>
    void fillGradient(agg::pod_auto_array<ColorType, 256> color_array, int s, int x, agg::trans_affine _mtx=agg::trans_affine()){
        typedef agg::pod_auto_array<ColorType, 256> color_array_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<ColorType> span_allocator_type;
        typedef agg::span_gradient<ColorType, interpolator_type, GradientType, color_array_type> span_gradient_type;
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;
        typedef agg::renderer_base<alpha_adaptor_type> renderer_base_type;
        typedef agg::renderer_scanline_aa<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type;

        GradientType  gradient_func;
        interpolator_type   span_interpolator(_mtx);
        span_allocator_type span_allocator;

        span_gradient_type span_gradient(span_interpolator,
                                         gradient_func,
                                         color_array,
                                         s, x);

        // transform
        agg::conv_curve<agg::path_storage> pth(*this);
        agg::conv_transform<agg::conv_curve<agg::path_storage>> m(pth, mtx);
        raster->add_path(m, pathid);

        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);
            renderer_gradient_type ren_gradient(alpha_base, span_allocator, span_gradient);
            agg::render_scanlines(*raster, *sl, ren_gradient);
         } else {
            agg::renderer_scanline_aa<agg::renderer_base<DrawingType>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
            agg::render_scanlines(*raster, *sl, ren_gradient);
        };
    }

    template<typename ColorType, typename GradientType>
    void strokeGradient(agg::pod_auto_array<ColorType, 256> color_array, int s, int x, agg::trans_affine _mtx=agg::trans_affine()){
        typedef agg::pod_auto_array<ColorType, 256> color_array_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<ColorType> span_allocator_type;
        typedef agg::span_gradient<ColorType, interpolator_type, GradientType, color_array_type> span_gradient_type;
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;
        typedef agg::renderer_base<alpha_adaptor_type> renderer_base_type;
        typedef agg::renderer_scanline_aa<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type;

        GradientType gradient_func;
        interpolator_type span_interpolator(_mtx);
        span_allocator_type span_allocator;
        span_gradient_type span_gradient(span_interpolator,
                                         gradient_func,
                                         color_array,
                                         s, x);

        // transform
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage>> q(p);
        q.width(_width);
        q.line_cap((agg::line_cap_e)_linecap);
        q.line_join((agg::line_join_e)_linejoin);
        q.miter_limit(_miterlimit);
        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage>>> m(q, mtx);
        raster->add_path(m, pathid);

        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);
            renderer_gradient_type ren_gradient(alpha_base, span_allocator, span_gradient);
            agg::render_scanlines(*raster, *sl, ren_gradient);
         } else {
            agg::renderer_scanline_aa<agg::renderer_base<DrawingType>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
            agg::render_scanlines(*raster, *sl, ren_gradient);
        };
    }

    // Adds current path and actually draws everything
    void paint(){
        paint(*this);
    }

    template <typename PathType>
    void paint(PathType &pth){
        typedef agg::pixfmt_amask_adaptor<DrawingType, agg::amask_no_clip_gray8> alpha_adaptor_type;

        raster->add_path(pth, pathid);
        if (alpha_mask != nullptr){
            agg::rendering_buffer alpha_mask_rbuf(alpha_mask, size.x, size.y, size.x);
            agg::amask_no_clip_gray8 alpha_mask(alpha_mask_rbuf);
            alpha_adaptor_type alpha_mask_adaptor(pix, alpha_mask);
            agg::renderer_base<alpha_adaptor_type> alpha_base(alpha_mask_adaptor);
            if (_antialias){
                agg::renderer_scanline_aa_solid<agg::renderer_base<alpha_adaptor_type>> alpha_ren(alpha_base);
                alpha_ren.color(current_color);
                agg::render_scanlines(*raster, *sl, alpha_ren);
            } else {
                agg::renderer_scanline_aa_solid<agg::renderer_base<alpha_adaptor_type>> alpha_ren(alpha_base);
                alpha_ren.color(current_color);
                agg::render_scanlines(*raster, *sl, alpha_ren);
            }
         } else {
            if (_antialias){
                agg::render_scanlines(*raster, *sl, render_aa);
            } else {
                agg::render_scanlines(*raster, *sl, render_bin);
            }
        }
    }

    inline void autoClose(bool c){
        raster->auto_close(c);
    }

    // Check if current point is in rasterizer
    inline bool inPath(double x, double y){
        return raster->hit_test(x, y);
    }

    inline unsigned getVertex(unsigned idx, double *x, double *y){
        if (idx > totalVertices()){
            return 0;
        }
        return vertex(idx, x, y);
    }

    inline unsigned getVertex(double *x, double *y){
        return vertex(x, y);
    }


    // Get command for vertex index
    inline unsigned getCommand(unsigned idx){
        return command(idx);
    }

    // Get last vertex
    inline unsigned lastVertex(double *x, double *y){
        return last_vertex(x, y);
    }

    // Get previous vertex
    inline unsigned prevVertex(double *x, double *y){
        return prev_vertex(x, y);
    }

    // Change vertex index to new point
    inline void modifyVertex(unsigned idx, double x, double y){
        if (idx > totalVertices()){
            return;
        }
        modify_vertex(idx, x, y);
    }

    // Change vertex index to new point and set new command
    inline void modifyVertex(unsigned idx, double x, double y, unsigned cmd){
        if (idx > totalVertices()){
            return;
        }
        modify_vertex(idx, x, y, cmd);
    }

    // Count vertices
    inline unsigned totalVertices(){
        return total_vertices();
    }

};

typedef agg::pixfmt_rgba32 rgba32;
typedef agg::pixfmt_rgb24 rgb24;
typedef agg::pixfmt_bgra32 bgra32;
typedef agg::pixfmt_bgr24 bgr24;
typedef agg::pixfmt_gray8 gray8;
typedef agg::pixfmt_rgba64 rgba64;
typedef agg::pixfmt_rgb48 rgb48;
typedef agg::pixfmt_bgra64 bgra64;
typedef agg::pixfmt_bgr48 bgr48;
typedef agg::pixfmt_gray16 gray16;

typedef Drawing<rgba32> DrawingRGBA32;
typedef Drawing<rgb24> DrawingRGB24;
typedef Drawing<rgba64> DrawingRGBA64;
typedef Drawing<rgb48> DrawingRGB48;
typedef Drawing<bgra32> DrawingBGRA32;
typedef Drawing<bgr24> DrawingBGR24;
typedef Drawing<bgra64> DrawingBGRA64;
typedef Drawing<bgr48> DrawingBGR48;

#ifndef NO_OPECV
Drawing<bgra32> draw(Mat4b& im, uint8_t* alpha_mask=nullptr);
Drawing<bgr24>draw(Mat3b& im, uint8_t* alpha_mask=nullptr);
Drawing<bgra64> draw(Mat4w& im, uint8_t* alpha_mask=nullptr);
Drawing<bgr48> draw(Mat3w& im, uint8_t* alpha_mask=nullptr);
#endif

#endif // cplusplus

#ifdef __cplusplus
} // namesapce tw
#endif

#endif // TWOMBLY_DRAW_HEADER

