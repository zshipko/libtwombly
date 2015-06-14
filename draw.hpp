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
    path_cmd_curve_n   = 5,
    path_cmd_catrom   = 6,
    path_cmd_ubspline = 7,
    path_cmd_end_poly = 0xff,
    path_cmd_mask     = 0xff
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

template <typename color_type, int S=256>
class Gradient {
public:
    std::vector<color_type> colors;
    agg::pod_auto_array<color_type, S> arr;
    agg::trans_affine mtx;

    Gradient() {}

    Gradient(color_type beg, color_type mid, color_type end){
        add_stop(beg);
        add_stop(mid);
        add_stop(end);
    }

    void add_stop(color_type const & c){
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
template <typename drawing_type>
class Drawing : public agg::path_storage {
    // render settings
    bool _antialias;
    double _width, _miterlimit;
    line_cap_style _linecap;
    line_join_style _linejoin;

    // rendering
    agg::renderer_scanline_aa_solid<agg::renderer_base<drawing_type>> render_aa;
    agg::renderer_scanline_bin_solid<agg::renderer_base<drawing_type>> render_bin;
    agg::renderer_base<drawing_type> base;
    agg::scanline32_p8 *sl;

    unsigned pathid; // stores current path
    Color current_color;

public:
    struct size {
        size(int32_t _x, int32_t _y, int32_t _c) : x(_x), y(_y), c(_c){}
        int32_t x, y, c;
    } size;

    drawing_type pix;
    agg::rendering_buffer buffer;
    agg::rasterizer_scanline_aa<> *raster;

    agg::trans_affine mtx;
    uint8_t *alpha_mask;

    Drawing() : pix(buffer), size(0, 0, 0), sl(nullptr), raster(nullptr), alpha_mask(nullptr) {
        base = agg::renderer_base<drawing_type>(pix);
        render_aa = agg::renderer_scanline_aa_solid<agg::renderer_base<drawing_type> >(base);
        render_bin = agg::renderer_scanline_bin_solid<agg::renderer_base<drawing_type> >(base);
    }

    // Creates a drawing context from width, height, channels and data
    Drawing(int32_t w, int32_t h, int32_t c, uint8_t *d, uint8_t *_alpha_mask=nullptr) : buffer(d, w, h, w * c), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(w, h, c),  alpha_mask(_alpha_mask) {
        alloc();
    }

    Drawing(int32_t w, int32_t h, int32_t c, uint16_t *d, uint8_t *_alpha_mask=nullptr) : buffer((uint8_t*)d, w, h, w * c * 2), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr), size(w, h, c),  alpha_mask(_alpha_mask) {
        alloc();
    }

#ifndef NO_OPECV
    // Creates a drawing context from standard open_cV Mat types
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

        alpha_mask_free();
    }

    // Actually create the image,
    // this function will overwrite the current context if it already exists
    void alloc(){
        _miterlimit = 1;
        base = agg::renderer_base<drawing_type>(pix);
        render_aa = agg::renderer_scanline_aa_solid<agg::renderer_base<drawing_type> >(base);
        render_bin = agg::renderer_scanline_bin_solid<agg::renderer_base<drawing_type> >(base);

        if (raster)
            delete raster;

        raster = new agg::rasterizer_scanline_aa<>();

        if (sl)
            delete sl;

        sl = new agg::scanline32_p8();
    }

    void alpha_mask_init(){
        if (alpha_mask == nullptr){
            alpha_mask = new uint8_t[size.x * size.y];
            memset(alpha_mask, 255, size.x * size.y);
        }
    }

    void alpha_mask_fill(uint8_t a){
        if (alpha_mask){
            memset(alpha_mask, a, size.x * size.y);
        }
    }

    uint8_t &alpha_mask_get(int32_t x, int32_t y){
        return alpha_mask[(y * size.x) + x];
    }

    uint8_t *alpha_mask_ptr_offs(int32_t x, int32_t y){
        return alpha_mask + (y * size.x) + x;
    }

    uint8_t *alpha_mask_ptr(){
        return alpha_mask;
    }

    void alpha_mask_free(){
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

    void line_width(double w){
        _width = w;
    }

    double line_width(){
        return _width;
    }

    void miter_limit(double m){
        _miterlimit = m;
    }

    double miter_limit(){
        return _miterlimit;
    }

    void line_join(line_join_style lj){
        _linejoin = lj;
    }

    line_join_style line_join(){
        return _linejoin;
    }

    void line_cap(line_cap_style lc){
        _linecap = lc;
    }

    line_cap_style line_cap(){
        return _linecap;
    }

    // Set the current pathid
    void active_path(unsigned p){
        pathid = p;
    }

    // Get the current pathid;
    unsigned active_path(){
        return pathid;
    }

    // Start new path, returns new pathid
    unsigned new_path(){
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
    inline void clear_transforms(){
        mtx.reset();
    }

    // End the current polygon and begin a new one
    inline void end_polygon(){
        end_poly();
    }

    inline void invert_polygon(){
        agg::path_storage::invert_polygon(pathid);
    }

    template <typename path_type>
    void concat_polygon(path_type *data, size_t n = 0, bool closed = false){
        concat_poly(data, n ? n : TW_POLY_SIZE(data), closed);
    }

    template <typename path_type>
    void join_polygon(path_type *data, size_t n = 0, bool closed = false){
        join_poly(data,  n ? n : TW_POLY_SIZE(data), closed);
    }

    // Reset everything
    void reset(){
        _width = 1;
        _miterlimit = 1;
        raster->reset();
        reset_clip();
        clear_transforms();
        remove_all();
    }

    // Clear drawing context with specific rgba color
    inline void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        base.clear(Color(r, g, b, a));
    }

    inline void clear(uint8_t r, uint8_t g, uint8_t b){
        base.clear(Color(r, g, b));
    }

    // Create shape objects
    void ellipse(double x, double y, double rx, double ry, int n = 100){
        agg::ellipse ell(x, y, rx, ry, n);
        join_path<agg::ellipse>(ell);
    }

    inline void rect(double x, double y, double x1, double y1, double border_radius=0){
        agg::rounded_rect r(x, y, x1, y1, border_radius);
        concat_path(r);
    }

    inline void clip(double x, double y, double x1, double y1){
        raster->clip_box(x, y, x1, y1);
    }

    inline void reset_clip(){
        raster->reset_clipping();
    }

    inline void filling_rule(filling_rule fr){
        raster->filling_rule((agg::filling_rule_e)fr);
    }

    // Get previous position
    inline Point last_position(){
        return Point(last_x(), last_y());
    }

    inline void curve_to(double x, double y){
        curve3(x, y);
    }

    inline void curve_rel(double x, double y){
        curve3_rel(x, y);
    }

    inline void curve_to(double x, double y, double a, double b){
        curve3(a, b, x, y);
    }

    inline void curve_rel(double x, double y, double a, double b){
        curve3_rel(a, b, x, y);
    }

    inline void curve_to(double x, double y, double a, double b, double c, double d){
        curve4(a, b, c, d, x, y);
    }

    inline void curve_rel(double x, double y, double a, double b, double c, double d){
        curve4_rel(a, b, c, d, x, y);
    }

    inline void arc_to(double x, double y, double rx, double ry, double angle, bool large = false, bool sweep = false){
        agg::path_storage::arc_to(rx, ry, angle, large, sweep, x, y);
    }

    inline void arc_rel(double x, double y, double rx, double ry, double angle, bool large = false, bool sweep = false){
        agg::path_storage::arc_rel(rx, ry, angle, large, sweep, x, y);
    }

    /* A list of important functions provided directly by agg::path_storage
     * move_to
     * move_rel
     * line_to
     * line_rel
     * hline_to
     * hline_rel
     * vline_to
     * vline_rel
     * arc_to
     * arc_rel
     * curve3
     * curve3_rel
     * curve4
     * curve4_rel
     * rel_to_abs
     * last_x
     * last_y
     * modify_vertex
     * modify_command
     * vertex
     * flip_x
     * flip_y
     */

    template <typename color_type>
    inline void blur(double a){
        agg::recursive_blur<color_type, agg::recursive_blur_calc_rgb<> > sb;
        sb.blur(pix, a);
    }

    // Draw text without freetype
    double text_simple(double x, double y, const char *txt, int size=50, double width=2.0, const char *font=nullptr, bool flip_y = true){
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

    // Copy from another drawing of the same type
    void copy_from(Drawing<drawing_type> &d){
        buffer.copy_from(d.buffer);
    }

    // Set the current rendering color
    inline void set_color(Color c){
        render_aa.color(c);
        render_bin.color(c);
        current_color = c;
    }

    inline void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255){
        set_color(Color(r, g, b, a));
    }

    // Fill/stroke path with another drawing
    template<typename color_type>
    void fill_pattern (Drawing<drawing_type> &d) {
        typedef agg::wrap_mode_repeat wrap_x_type;
        typedef agg::wrap_mode_repeat wrap_y_type;
        typedef agg::image_accessor_wrap<drawing_type, wrap_x_type, wrap_y_type> img_source_type;
        typedef agg::pixfmt_amask_adaptor<drawing_type, agg::amask_no_clip_gray8> alpha_adaptor_type;

        agg::conv_curve<agg::path_storage> pth(*this);
        agg::span_allocator<color_type> sa;

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

    template <typename color_type>
    void stroke_pattern (Drawing<drawing_type> &d) {
        typedef agg::wrap_mode_repeat wrap_x_type;
        typedef agg::wrap_mode_repeat wrap_y_type;
        typedef agg::image_accessor_wrap<drawing_type, wrap_x_type, wrap_y_type> img_source_type;
        typedef agg::span_pattern_rgb<img_source_type> span_gen_type;
        typedef agg::pixfmt_amask_adaptor<drawing_type, agg::amask_no_clip_gray8> alpha_adaptor_type;

        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage>> pth(p);
        agg::span_allocator<color_type> sa;

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

    template<typename color_type, typename gradient_type>
    void fill_gradient(color_type b, color_type m, color_type e, int s, int x){
        Gradient<color_type> grad(b, m, e);
        grad.generate();
        fill_gradient<color_type, gradient_type>(grad.arr, s, x, grad.mtx);
    }

    template<typename color_type, typename gradient_type>
    void stroke_gradient(color_type b, color_type m, color_type e, int s, int x){
        Gradient<color_type> grad(b, m, e);
        grad.generate();
        stroke_gradient<color_type, gradient_type>(grad.arr, s, x, grad.mtx);
    }

    template <typename color_type, typename gradient_type>
    void stroke_gradient(Gradient<color_type> grad, int s, int x){
        grad.generate();
        stroke_gradient<color_type, gradient_type>(grad.arr, s, x, grad.mtx);
    }

    template <typename color_type, typename gradient_type>
    void fill_gradient(Gradient<color_type> grad, int s, int x){
        grad.generate();
        fill_gradient<color_type, gradient_type>(grad.arr, s, x, grad.mtx);
    }

    template <typename color_type>
    void stroke_gradient(Gradient<color_type> grad, int s, int x, gradient_type grad_type){
        grad.generate();
        switch(grad_type){
            case gradient_type_circle:
                stroke_gradient<color_type, agg::gradient_circle>(grad, s, x);
                break;
            case gradient_type_radial:
                stroke_gradient<color_type, agg::gradient_radial>(grad, s, x);
                break;
            case gradient_type_radial_d:
                stroke_gradient<color_type, agg::gradient_radial_d>(grad, s, x);
                break;
            case gradient_type_radial_focus:
                stroke_gradient<color_type, agg::gradient_radial_focus>(grad, s, x);
                break;
            case gradient_type_x:
                stroke_gradient<color_type, agg::gradient_x>(grad, s, x);
                break;
            case gradient_type_y:
                stroke_gradient<color_type, agg::gradient_y>(grad, s, x);
                break;
            case gradient_type_diamond:
                stroke_gradient<color_type, agg::gradient_diamond>(grad, s, x);
                break;
            case gradient_type_xy:
                stroke_gradient<color_type, agg::gradient_xy>(grad, s, x);
                break;
            case gradient_type_sqrt_xy:
                stroke_gradient<color_type, agg::gradient_sqrt_xy>(grad, s, x);
                break;
            case gradient_type_conic:
                stroke_gradient<color_type, agg::gradient_conic>(grad, s, x);
                break;
        }
    }

    template <typename color_type>
    void fill_gradient(Gradient<color_type> grad, int s, int x, gradient_type grad_type){
        grad.generate();
        switch(grad_type){
            case gradient_type_circle:
                fill_gradient<color_type, agg::gradient_circle>(grad, s, x);
                break;
            case gradient_type_radial:
                fill_gradient<color_type, agg::gradient_radial>(grad, s, x);
                break;
            case gradient_type_radial_d:
                fill_gradient<color_type, agg::gradient_radial_d>(grad, s, x);
                break;
            case gradient_type_radial_focus:
                fill_gradient<color_type, agg::gradient_radial_focus>(grad, s, x);
                break;
            case gradient_type_x:
                fill_gradient<color_type, agg::gradient_x>(grad, s, x);
                break;
            case gradient_type_y:
                fill_gradient<color_type, agg::gradient_y>(grad, s, x);
                break;
            case gradient_type_diamond:
                fill_gradient<color_type, agg::gradient_diamond>(grad, s, x);
                break;
            case gradient_type_xy:
                fill_gradient<color_type, agg::gradient_xy>(grad, s, x);
                break;
            case gradient_type_sqrt_xy:
                fill_gradient<color_type, agg::gradient_sqrt_xy>(grad, s, x);
                break;
            case gradient_type_conic:
                fill_gradient<color_type, agg::gradient_conic>(grad, s, x);
                break;
        }
    }

    template<typename color_type, typename gradient_type>
    void fill_gradient(agg::pod_auto_array<color_type, 256> color_array, int s, int x, agg::trans_affine _mtx=agg::trans_affine()){
        typedef agg::pod_auto_array<color_type, 256> color_array_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<color_type> span_allocator_type;
        typedef agg::span_gradient<color_type, interpolator_type, gradient_type, color_array_type> span_gradient_type;
        typedef agg::pixfmt_amask_adaptor<drawing_type, agg::amask_no_clip_gray8> alpha_adaptor_type;
        typedef agg::renderer_base<alpha_adaptor_type> renderer_base_type;
        typedef agg::renderer_scanline_aa<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type;

        gradient_type  gradient_func;
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
            agg::renderer_scanline_aa<agg::renderer_base<drawing_type>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
            agg::render_scanlines(*raster, *sl, ren_gradient);
        };
    }

    template<typename color_type, typename gradient_type>
    void stroke_gradient(agg::pod_auto_array<color_type, 256> color_array, int s, int x, agg::trans_affine _mtx=agg::trans_affine()){
        typedef agg::pod_auto_array<color_type, 256> color_array_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<color_type> span_allocator_type;
        typedef agg::span_gradient<color_type, interpolator_type, gradient_type, color_array_type> span_gradient_type;
        typedef agg::pixfmt_amask_adaptor<drawing_type, agg::amask_no_clip_gray8> alpha_adaptor_type;
        typedef agg::renderer_base<alpha_adaptor_type> renderer_base_type;
        typedef agg::renderer_scanline_aa<renderer_base_type, span_allocator_type, span_gradient_type> renderer_gradient_type;

        gradient_type gradient_func;
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
            agg::renderer_scanline_aa<agg::renderer_base<drawing_type>, span_allocator_type, span_gradient_type> ren_gradient(base, span_allocator, span_gradient);
            agg::render_scanlines(*raster, *sl, ren_gradient);
        };
    }

    // Adds current path and actually draws everything
    void paint(){
        paint(*this);
    }

    template <typename path_type>
    void paint(path_type &pth){
        typedef agg::pixfmt_amask_adaptor<drawing_type, agg::amask_no_clip_gray8> alpha_adaptor_type;

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

    inline void auto_close(bool c){
        raster->auto_close(c);
    }

    // Check if current point is in rasterizer
    inline bool in_path(double x, double y){
        return raster->hit_test(x, y);
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

