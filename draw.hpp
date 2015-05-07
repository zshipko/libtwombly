#ifndef TWOMBLY_DRAW_HEADER
#define TWOMBLY_DRAW_HEADER
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

#define TW_POLY_SIZE(p) (sizeof(p) / (sizeof(*p) * 2))

namespace tw {

typedef agg::rgba8 Color;

inline Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return Color(r, g, b, a);
}

inline Color rgb(uint8_t r, uint8_t g, uint8_t b){
    return Color(r, g, b, 255);
}

template <typename ColorType>
inline Color rgb(ColorType t){
    Color dst(0, 0, 0, 0);

    for(auto i = 0; i < t.channels(); i++){
        switch(i){
        case 0:
            dst.r = t[i];
            break;
        case 1:
            dst.g = t[i];
            break;
        case 2:
            dst.b = t[i];
            break;
        case 3:
            dst.a = t[i];
            break;
        }
    }
    return dst;
}

enum line_cap_style {
    butt_cap,
    square_cap,
    round_cap
};

enum line_join_style
{
    miter_join         = 0,
    miter_join_revert  = 1,
    round_join         = 2,
    bevel_join         = 3,
    miter_join_round   = 4
};

enum filling_rule
{
    fill_non_zero,
    fill_even_odd
};

template<class FillArray>
void fill_color_array_3_stop(FillArray& array,
                      Color begin,
                      Color middle,
                      Color end){
    unsigned i;
    unsigned half_size = array.size() / 2;
    for(i = 0; i < half_size; ++i){
        array[i] = begin.gradient(middle, i / double(half_size));
    }
    for(; i < array.size(); ++i){
        array[i] = middle.gradient(end, (i - half_size) / double(half_size));
    }
}

template <typename DrawingType>
class Drawing : public agg::path_storage {
    bool _antialias;
    double _width, _miterlimit;
    line_cap_style _linecap;
    line_join_style _linejoin;
    agg::renderer_scanline_aa_solid<agg::renderer_base<DrawingType> > render_aa;
    agg::renderer_scanline_bin_solid<agg::renderer_base<DrawingType> > render_bin;
    agg::renderer_base<DrawingType> base;
    unsigned pathid;

public:
    agg::rendering_buffer buffer;
    agg::rasterizer_scanline_aa<> *raster;
    agg::scanline_p8 *sl;
    agg::trans_affine mtx;
    DrawingType pix;

    Drawing(int32_t w, int32_t h, int32_t c, uint8_t *d) : buffer(d, w, h, w * c), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr) {
        alloc();
    }

    Drawing(Image<uint8_t> &im) : buffer((uint8_t*)im.data, im.width, im.height, im.width * im.channels), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr) {
        alloc();
    }

    Drawing(Image<uint16_t> &im) : buffer((uint8_t*)im.data, im.width, im.height, im.width * im.channels * 2), pix(buffer), _antialias(true), _width(1), pathid(0), raster(nullptr), sl(nullptr) {
        alloc();
    }

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

        sl = new agg::scanline_p8();
    }

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

    void activePath(unsigned p){
        pathid = p;
    }

    unsigned activePath(){
        return pathid;
    }

    line_cap_style lineCap(){
        return _linecap;
    }

    unsigned newPath(){
        raster->reset();
        pathid = start_new_path();
        return pathid;
    }

    void rewind(unsigned pid){
        rewind(pid);
        pathid = pid;
    }

    void rewind(){
        rewind(0);
        pathid = 0;
    }

    inline void rotate(double rad){
        mtx *= agg::trans_affine_rotation(rad);
    }

    inline void translate(double x, double y){
        mtx *= agg::trans_affine_translation(x, y);
    }

    inline void scale(double n){
        mtx *= agg::trans_affine_scaling(n);
    }

    inline void scale(double n, double m){
        mtx *= agg::trans_affine_scaling(n, m);
    }

    void skew(double x, double y){
        mtx *= agg::trans_affine_skewing(x, y);
    }

    inline void clearTransforms(){
        mtx.reset();
    }

    template<typename S>
    void joinPath(S &s){
        join_path(s, pathid);
    }

    template<typename S>
    void concatPath(S &s){
        concat_path(s, pathid);
    }

    inline void closePolygon(){
        close_polygon();
    }

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

    void reset(){
        _width = 1;
        _miterlimit = 1;
        raster->reset();
        resetClip();
        clearTransforms();
        removePaths();
    }

    inline void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        base.clear(Color(r, g, b, a));
    }

    inline void clear(uint8_t r, uint8_t g, uint8_t b){
        base.clear(Color(r, g, b));
    }

    inline void removePaths(){
        remove_all();
    }

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

    inline void addVertex(double x, double y, int cmd = agg::path_cmd_line_to){
        raster->add_vertex(x, y, cmd);
    }

    inline double lastX(){
        return last_x();
    }

    inline double lastY(){
        return last_y();
    }

    inline void relToAbs(double *x, double *y){
        rel_to_abs(x, y);
    }

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

    inline void blur(double a){
        agg::recursive_blur<Color, agg::recursive_blur_calc_rgb<> > sb;
        sb.blur(pix, a);
    }

    double putTextSimple(double x, double y, const char *txt, int size=50, double width=2.0, const char *font=nullptr, bool flip_y = true){
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
    double putText(double x, double y, const char *txt, const char *font, double width, double height){
        agg::glyph_rendering font_ren = agg::glyph_ren_outline;
        agg::font_engine_freetype_int32 font_engine;
        agg::font_cache_manager< agg::font_engine_freetype_int32> font_manager(font_engine);
        const char *ptr = txt;
        double start_x = x;
        if (font_engine.load_font(font, 0, font_ren)){
            font_engine.width(width);
            font_engine.height(height);
            font_engine.flip_y(true);
            font_engine.hinting(true);
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

    double textLengthSimple(const char *txt, int size=50, double width=2.0, void *font=nullptr, bool flip_y = true){
        agg::gsv_text text;
        agg::trans_affine mt;
        agg::gsv_text_outline<agg::trans_affine> outline(text, mt);
        text.text(txt);
        text.flip(flip_y);
        text.size(size);
        outline.width(width);
        if (font){
            text.font(font);
        }
        return text.text_width();
    }


    template <typename Q>
    void fillPattern (Q &im) {
        agg::conv_curve<agg::path_storage> p(*this);
        typedef agg::wrap_mode_repeat wrap_x_type;
        typedef agg::wrap_mode_repeat wrap_y_type;

        typedef agg::image_accessor_wrap<DrawingType, wrap_x_type, wrap_y_type> img_source_type;
        typedef agg::span_pattern_rgb<img_source_type> span_gen_type;

        agg::span_allocator<Color> sa;

        auto d = draw(im);

        DrawingType img_pixf (d.buffer);
        img_source_type img_src (img_pixf);
        span_gen_type sg ( img_src, im.width, im.height);

        // apply transforms
        agg::conv_transform<agg::conv_curve<agg::path_storage>> m(p, mtx);
        raster->add_path(m, pathid);
        agg::render_scanlines_aa( *raster, *sl, base, sa, sg);
    }

    template <typename Q>
    void strokePattern (Q &im) {
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage>> pth(p);

        typedef agg::wrap_mode_repeat wrap_x_type;
        typedef agg::wrap_mode_repeat wrap_y_type;

        typedef agg::image_accessor_wrap<DrawingType, wrap_x_type, wrap_y_type> img_source_type;
        typedef agg::span_pattern_rgb<img_source_type> span_gen_type;

        agg::span_allocator<Color> sa;

        auto d = draw(im);

        DrawingType img_pixf (d.buffer);
        img_source_type img_src (img_pixf);
        span_gen_type sg ( img_src, im.width, im.height);

        pth.width(_width);
        pth.line_cap((agg::line_cap_e)_linecap);
        pth.line_join((agg::line_join_e)_linejoin);
        pth.miter_limit(_miterlimit);

        // apply transforms
        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage>>> m(pth, mtx);
        raster->add_path(m, pathid);
        agg::render_scanlines_aa( *raster, *sl, base, sa, sg);
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


    void paint(){
        agg::path_storage g = *this;
        raster->add_path(g, pathid);
        if (_antialias){
            agg::render_scanlines(*raster, *sl, render_aa);
        } else {
            agg::render_scanlines(*raster, *sl, render_bin);
        }
    }

    template <typename S>
    void paint(S &pth){
        raster->add_path(pth, pathid);
        if (_antialias){
            agg::render_scanlines(*raster, *sl, render_aa);
        } else {
            agg::render_scanlines(*raster, *sl, render_bin);
        }
    }

    inline void autoClose(bool c){
        raster->auto_close(c);
    }

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

    inline unsigned getCommand(unsigned idx){
        return command(idx);
    }

    inline unsigned lastVertex(double *x, double *y){
        return last_vertex(x, y);
    }

    inline unsigned prevVertex(double *x, double *y){
        return prev_vertex(x, y);
    }

    inline void modifyVertex(unsigned idx, double x, double y){
        if (idx > totalVertices()){
            return;
        }
        modify_vertex(idx, x, y);
    }

    inline void modifyVertex(unsigned idx, double x, double y, unsigned cmd){
        if (idx > totalVertices()){
            return;
        }
        modify_vertex(idx, x, y, cmd);
    }

    inline unsigned totalVertices(){
        return total_vertices();
    }

    template<typename S>
    void fillGradient(Color b, Color m, Color e, int s, int x){
        agg::pod_auto_array<Color, 256> color_array;
        fill_color_array_3_stop(color_array, b, m, e);
        fillGradient<S>(color_array, s, x );
    }

    template<typename S>
    void strokeGradient(Color b, Color m, Color e, int s, int x){
        agg::pod_auto_array<Color, 256> color_array;
        fill_color_array_3_stop(color_array, b, m, e);
        strokeGradient<S>(color_array, s, x);
    }

    template<typename S>
    void fillGradient(agg::pod_auto_array<Color, 256> color_array, int s, int x){
        agg::conv_curve<agg::path_storage> p(*this);

        typedef agg::renderer_base<DrawingType> renderer_base_type;
        typedef agg::pod_auto_array<Color, 256> color_array_type;
        typedef S gradient_func_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<Color> span_allocator_type;
        typedef agg::span_gradient<Color,
                                   interpolator_type,
                                   gradient_func_type,
                                   color_array_type> span_gradient_type;
        typedef agg::renderer_scanline_aa<renderer_base_type,
                                      span_allocator_type, span_gradient_type> renderer_gradient_type;
        gradient_func_type  gradient_func;                   // The gradient function
        agg::trans_affine   gradient_mtx;                    // Affine transformer
        interpolator_type   span_interpolator(gradient_mtx); // Span interpolator
        span_allocator_type span_allocator;

        span_gradient_type span_gradient(span_interpolator,
                                     gradient_func,
                                     color_array,
                                     s, x);

        renderer_gradient_type ren_gradient(base, span_allocator, span_gradient);

        // transform
        agg::conv_transform<agg::conv_curve<agg::path_storage>> m(p, mtx);
        raster->add_path(m, pathid);
        agg::render_scanlines(*raster, *sl, ren_gradient);
    }

    template<typename S>
    void strokeGradient(agg::pod_auto_array<Color, 256> color_array, int s, int x){
        agg::conv_curve<agg::path_storage> p(*this);
        agg::conv_stroke<agg::conv_curve<agg::path_storage>> q(p);
        q.width(_width);
        q.line_cap((agg::line_cap_e)_linecap);
        q.line_join((agg::line_join_e)_linejoin);
        q.miter_limit(_miterlimit);


        typedef agg::renderer_base<DrawingType> renderer_base_type;
        typedef agg::pod_auto_array<Color, 256> color_array_type;
        typedef S gradient_func_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef agg::span_allocator<Color> span_allocator_type;
        typedef agg::span_gradient<Color,
                                   interpolator_type,
                                   gradient_func_type,
                                   color_array_type> span_gradient_type;
        typedef agg::renderer_scanline_aa<renderer_base_type,
                                      span_allocator_type, span_gradient_type> renderer_gradient_type;
        gradient_func_type  gradient_func;                   // The gradient function
        agg::trans_affine   gradient_mtx;                    // Affine transformer
        interpolator_type   span_interpolator(gradient_mtx); // Span interpolator
        span_allocator_type span_allocator;

        span_gradient_type span_gradient(span_interpolator,
                                     gradient_func,
                                     color_array,
                                     s, x);

        renderer_gradient_type ren_gradient(base, span_allocator, span_gradient);

        // transform
        agg::conv_transform<agg::conv_stroke<agg::conv_curve<agg::path_storage>>> m(q, mtx);
        raster->add_path(m, pathid);
        agg::render_scanlines(*raster, *sl, ren_gradient);
    }

    void fillLinearGradientH(Color b, Color m, Color e, int s, int x){
        fillGradient<agg::gradient_x>(b, m, e, s, x);
    }

    void fillLinearGradientV(Color b, Color m, Color e, int s, int x){
        fillGradient<agg::gradient_y>(b, m, e, s, x);
    }

    void fillRadialGradient(Color b, Color m, Color e, int s, int x){
        fillGradient<agg::gradient_radial>(b, m, e, s, x);
    }

    void strokeLinearGradientH(Color b, Color m, Color e, int s, int x){
        strokeGradient<agg::gradient_x>(b, m, e, s, x);
    }

    void strokeLinearGradientV(Color b, Color m, Color e, int s, int x){
        strokeGradient<agg::gradient_y>(b, m, e, s, x);
    }

    void strokeRadialGradient(Color b, Color m, Color e, int s, int x){
        strokeGradient<agg::gradient_radial>(b, m, e, s, x);
    }

    void copyFrom(Drawing<DrawingType> &d){
        buffer.copy_from(d.buffer);
    }

    inline void setColor(Color c){
        render_aa.color(c);
        render_bin.color(c);
    }

    inline void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255){
        setColor(Color(r, g, b, a));
    }

    inline void setColor(Pixel p){
        setColor(Color(p[0], p[1], p[2], p[3] ? p[3] : 255));
    }



    ~Drawing(){
        if (raster){
            delete raster;
        }

        if (sl){
            delete sl;
        }
    }
};

typedef agg::pixfmt_rgba32 rgba32;
typedef agg::pixfmt_rgb24 rgb24;
typedef agg::pixfmt_gray8 gray8;
typedef agg::pixfmt_rgba64 rgba64;
typedef agg::pixfmt_rgb48 rgb48;
typedef agg::pixfmt_gray16 gray16;

typedef Drawing<rgba32> DrawingRGBA32;
typedef Drawing<rgb24> DrawingRGB24;
typedef Drawing<gray8> DrawingGray8;
typedef Drawing<rgba64> DrawingRGBA64;
typedef Drawing<rgb48> DrawingRGB48;
typedef Drawing<gray16> DrawingGray16;

template <typename DrawingType, typename ImageType>
Drawing<DrawingType> draw(ImageType &im){
    return Drawing<DrawingType>(im);
}


} // namesapce tw
#endif // TWOMBLY_DRAW_HEADER

