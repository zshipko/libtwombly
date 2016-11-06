// bimage stubs
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/custom.h>

#include <bimage/bimage.h>
#include <twombly/draw_c.h>

#define Draw_val(v) (*((drawing *) Data_custom_val(v)))
#define Gradient_val(v) (*((gradient *) Data_custom_val(v)))
#define Transform_val(v) (*((transform_matrix *) Data_custom_val(v)))
#define Image_val(v) (*((bimage**) Data_custom_val(v)))

#define TUPLE_D2(dst, _x, _y) \
    CAMLlocal1(dst); \
    dst = alloc_tuple(2); \
    Store_field(dst, 0, copy_double(_x)); \
    Store_field(dst, 1, copy_double(_y));


// Call draw_free when collected by GC
void finalize_transform_matrix (value v) {
    transform_matrix g = Transform_val(v);
    draw_transform_matrix_free(&g);
}

// OCaml image representation
static struct custom_operations transform_ops = {
    "zachshipko.twombly.transform_matrix",
    finalize_transform_matrix,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default
};


// Allocate OCaml image from bimage struct
static value alloc_transform (transform_matrix g){
    value v = alloc_custom(&transform_ops, sizeof(transform_matrix), 0, 1);
    Transform_val(v) = g;
    return v;
}

// Call draw_free when collected by GC
void finalize_gradient (value v) {
    gradient g = Gradient_val(v);
    draw_gradient_free(&g);
}

// OCaml image representation
static struct custom_operations gradient_ops = {
    "zachshipko.twombly.gradient",
    finalize_gradient,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default
};


// Allocate OCaml image from bimage struct
static value alloc_gradient (gradient g){
    value v = alloc_custom(&gradient_ops, sizeof(gradient), 0, 1);
    Gradient_val(v) = g;
    return v;
}

// Call draw_free when collected by GC
void finalize_draw (value v) {
    drawing d = Draw_val(v);
    draw_free(&d);
}

// OCaml image representation
static struct custom_operations draw_ops = {
    "zachshipko.twombly.draw",
    finalize_draw,
    custom_compare_default,
    custom_hash_default,
    custom_serialize_default,
    custom_deserialize_default,
    custom_compare_ext_default
};


// Allocate OCaml image from bimage struct
static value alloc_draw (drawing d){
    value v = alloc_custom(&draw_ops, sizeof(drawing), 0, 1);
    Draw_val(v) = d;
    return v;
}

value _of_image (value img) {
    CAMLparam1(img);

    bimage *im = Image_val(img);

    CAMLlocal1(d);
    d = alloc_draw(draw_frombimage(im));
    CAMLreturn(d);
}

value _drawing(value width, value height, value channels) {
    CAMLparam3(width, height, channels);
    CAMLlocal1(d);
    d = alloc_draw(draw_create(Int_val(width), Int_val(height), Int_val(channels), NULL));
    CAMLreturn(d);
}

value _preserve(value d){
    CAMLparam1(d);
    CAMLlocal1(b);
    drawing _d = Draw_val(d);
    b = Val_bool(draw_get_preserve(_d));
    CAMLreturn(b);
}

value _set_preserve(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_set_preserve(_d, Bool_val(b));
    CAMLreturn(Val_unit);
}

value _antialias(value d){
    CAMLparam1(d);
    CAMLlocal1(b);
    drawing _d = Draw_val(d);
    b = Val_bool(draw_get_antialias(_d));
    CAMLreturn(b);
}

value _set_antialias(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_set_antialias(_d, Bool_val(b));
    CAMLreturn(Val_unit);
}

value _linewidth(value d){
    CAMLparam1(d);
    CAMLlocal1(b);
    drawing _d = Draw_val(d);
    b = caml_copy_double(draw_get_line_width(_d));
    CAMLreturn(b);
}

value _set_linewidth(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_set_line_width(_d, Double_val(b));
    CAMLreturn(Val_unit);
}

value _miterlimit(value d){
    CAMLparam1(d);
    CAMLlocal1(b);
    drawing _d = Draw_val(d);
    b = caml_copy_double(draw_get_miter_limit(_d));
    CAMLreturn(b);
}

value _set_miterlimit(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_set_miter_limit(_d, Double_val(b));
    CAMLreturn(Val_unit);
}

value _linejoin(value d){
    CAMLparam1(d);
    CAMLlocal1(b);
    drawing _d = Draw_val(d);
    b = Int_val(draw_get_line_join(_d));
    CAMLreturn(b);
}

value _set_linejoin(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_set_line_join(_d, Int_val(b));
    CAMLreturn(Val_unit);
}

value _linecap(value d){
    CAMLparam1(d);
    CAMLlocal1(b);
    drawing _d = Draw_val(d);
    b = Int_val(draw_get_line_cap(_d));
    CAMLreturn(b);
}

value _set_linecap(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_set_line_cap(_d, Int_val(b));
    CAMLreturn(Val_unit);
}

value _active_path(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    CAMLlocal1(path);
    path = Val_int(draw_get_active_path(_d));
    CAMLreturn(path);
}

value _set_active_path(value d, value i) {
    CAMLparam2(d, i);
    drawing _d = Draw_val(d);
    draw_set_active_path(_d, Int_val(i));
    CAMLreturn(Val_unit);
}

value _new_path(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    CAMLlocal1(path);
    path = Val_int(draw_new_path(_d));
    CAMLreturn(path);
}

value _rotate(value d, value f) {
    CAMLparam2(d, f);
    drawing _d = Draw_val(d);
    draw_rotate(_d, Double_val(f));
    CAMLreturn(Val_unit);
}

value _translate(value d, value f, value g) {
    CAMLparam3(d, f, g);
    drawing _d = Draw_val(d);
    draw_translate(_d, Double_val(f), Double_val(g));
    CAMLreturn(Val_unit);
}

value _scale(value d, value f) {
    CAMLparam2(d, f);
    drawing _d = Draw_val(d);
    draw_scale(_d, Double_val(f));
    CAMLreturn(Val_unit);
}

value _scale2(value d, value f, value g) {
    CAMLparam3(d, f, g);
    drawing _d = Draw_val(d);
    draw_scale2(_d, Double_val(f), Double_val(g));
    CAMLreturn(Val_unit);
}

value _skew(value d, value f, value g) {
    CAMLparam3(d, f, g);
    drawing _d = Draw_val(d);
    draw_skew(_d, Double_val(f), Double_val(g));
    CAMLreturn(Val_unit);
}

value _clear_transforms(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_clear_transforms(_d);
    CAMLreturn(Val_unit);
}

value _close_polygon(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_close_polygon(_d);
    CAMLreturn(Val_unit);
}

value _end_polygon(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_end_polygon(_d);
    CAMLreturn(Val_unit);
}

value _invert_polygon(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_invert_polygon(_d);
    CAMLreturn(Val_unit);
}

value _reset(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_reset(_d);
    CAMLreturn(Val_unit);
}

value _clear(value d, value x, value y, value x1, value y1) {
    CAMLparam5(d, x, y, x1, y1);
    drawing _d = Draw_val(d);
    draw_clear(_d, Int_val(x), Int_val(y), Int_val(x1), Int_val(y1));
    CAMLreturn(Val_unit);
}

value _remove_all(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_remove_all(_d);
    CAMLreturn(Val_unit);
}

value _ellipse(value d, value x, value y, value x1, value y1) {
    CAMLparam5(d, x, y, x1, y1);
    drawing _d = Draw_val(d);
    draw_ellipse(_d, Double_val(x), Double_val(y), Double_val(x1), Double_val(y1));
    CAMLreturn(Val_unit);
}

value _rect(value d, value x, value y, value x1, value y1) {
    CAMLparam5(d, x, y, x1, y1);
    drawing _d = Draw_val(d);
    draw_rect(_d, Double_val(x), Double_val(y), Double_val(x1), Double_val(y1), 0);
    CAMLreturn(Val_unit);
}

value _clip(value d, value x, value y, value x1, value y1) {
    CAMLparam5(d, x, y, x1, y1);
    drawing _d = Draw_val(d);
    draw_clip(_d, Double_val(x), Double_val(y), Double_val(x1), Double_val(y));
    CAMLreturn(Val_unit);
}

value _reset_clip(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_reset_clip(_d);
    CAMLreturn(Val_unit);
}

value _last_x(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    CAMLlocal1(x);
    x = Val_int(draw_last_x(_d));
    CAMLreturn(x);
}

value _last_y(value d) {
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    CAMLlocal1(y);
    y = Val_int(draw_last_y(_d));
    CAMLreturn(y);
}

value _move_to(value d, value x, value y) {
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_move_to(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _move_rel(value d, value x, value y) {
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_move_rel(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _line_to(value d, value x, value y) {
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_line_to(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _line_rel(value d, value x, value y) {
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_line_rel(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _hline_to(value d, value a) {
    CAMLparam2(d, a);
    drawing _d = Draw_val(d);
    draw_hline_to(_d, Double_val(a));
    CAMLreturn(Val_unit);
}

value _hline_rel(value d, value a) {
    CAMLparam2(d, a);
    drawing _d = Draw_val(d);
    draw_hline_rel(_d, Double_val(a));
    CAMLreturn(Val_unit);
}

value _vline_to(value d, value a) {
    CAMLparam2(d, a);
    drawing _d = Draw_val(d);
    draw_vline_to(_d, Double_val(a));
    CAMLreturn(Val_unit);
}

value _vline_rel(value d, value a) {
    CAMLparam2(d, a);
    drawing _d = Draw_val(d);
    draw_vline_rel(_d, Double_val(a));
    CAMLreturn(Val_unit);
}

value _curve_to2(value d, value x, value y) {
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_curve_to2(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _curve_rel2(value d, value x, value y) {
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_curve_rel2(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _curve_to4(value d, value w, value x, value y, value z) {
    CAMLparam5(d, w, x, y, z);
    drawing _d = Draw_val(d);
    draw_curve_to4(_d, Double_val(w), Double_val(x), Double_val(y), Double_val(z));
    CAMLreturn(Val_unit);
}

value _curve_rel4(value d, value w, value x, value y, value z) {
    CAMLparam5(d, w, x, y, z);
    drawing _d = Draw_val(d);
    draw_curve_rel4(_d, Double_val(w), Double_val(x), Double_val(y), Double_val(z));
    CAMLreturn(Val_unit);
}

value _set_color(value d, value r, value g, value b, value a){
    CAMLparam5(d, r, g, b, a);
    drawing _d = Draw_val(d);
    draw_set_color(_d, Int_val(r), Int_val(g), Int_val(b), Int_val(a));
    CAMLreturn(Val_unit);
}

value _twombly_fill(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_fill(_d);
    CAMLreturn(Val_unit);
}

value _fill_color(value d, value r, value g, value b, value a){
    CAMLparam5(d, r, g, b, a);
    drawing _d = Draw_val(d);
    float p[] = {Double_val(r), Double_val(g), Double_val(b), Double_val(a)};
    draw_fill_color(_d, p);
    CAMLreturn(Val_unit);
}

value _stroke(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_stroke(_d);
    CAMLreturn(Val_unit);
}

value _stroke_color(value d, value r, value g, value b, value a){
    CAMLparam5(d, r, g, b, a);
    drawing _d = Draw_val(d);
    float p[] = {Double_val(r), Double_val(g), Double_val(b), Double_val(a)};
    draw_stroke_color(_d, p);
    CAMLreturn(Val_unit);
}

value _dash(value d, value x, value y){
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_dash(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _paint(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_paint(_d);
    CAMLreturn(Val_unit);
}

value _set_autoclose(value d, value b){
    CAMLparam2(d, b);
    drawing _d = Draw_val(d);
    draw_auto_close(_d, Bool_val(b));
    CAMLreturn(Val_unit);
}

value _in_path(value d, value x, value y){
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_in_path(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _is_drawn(value d, value x, value y){
    CAMLparam3(d, x, y);
    drawing _d = Draw_val(d);
    draw_is_drawn(_d, Double_val(x), Double_val(y));
    CAMLreturn(Val_unit);
}

value _get_vertex(value d, value idx){
    CAMLparam2(d, idx);
    drawing _d = Draw_val(d);
    double x = 0, y = 0;
    draw_get_vertex(_d, Int_val(idx), &x, &y);
    TUPLE_D2(dst, x, y);
    CAMLreturn(dst);
}

value _next_vertex(value d){
    CAMLparam1(d);
    double x = 0, y = 0;
    drawing _d = Draw_val(d);
    draw_next_vertex(_d, &x, &y);
    TUPLE_D2(dst, x, y);
    CAMLreturn(dst);
}

value _get_command (value d, value idx){
    CAMLparam2(d, idx);
    drawing _d = Draw_val(d);
    int cmd = draw_get_command(_d, Int_val(idx));
    CAMLlocal1(dst);
    dst = Int_val(cmd);
    CAMLreturn(dst);
}

value _last_vertex(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    double x = 0, y = 0;
    draw_last_vertex(_d, &x, &y);
    TUPLE_D2(dst, x, y);
    CAMLreturn(dst);
}

value _prev_vertex(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    double x = 0, y = 0;
    draw_prev_vertex(_d, &x, &y);
    TUPLE_D2(dst, x, y);
    CAMLreturn(dst);
}

value _modify_vertex(value d, value idx, value x, value y, value cmd){
    CAMLparam5(d, idx, x, y, cmd);
    drawing _d = Draw_val(d);
    draw_modify_vertex(_d, Int_val(idx), Double_val(x), Double_val(y), Int_val(cmd));
    CAMLreturn(Val_unit);
}

value _total_vertices(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);

    CAMLlocal1(dst);
    dst = Val_int(draw_total_vertices(_d));
    CAMLreturn(dst);
}


value _alpha_mask_init(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_alpha_mask_init(_d);
    CAMLreturn(Val_unit);
}

value _alpha_mask_free(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    draw_alpha_mask_free(_d);
    CAMLreturn(Val_unit);
}

value _alpha_mask_fill(value d, value i){
    CAMLparam2(d, i);
    drawing _d = Draw_val(d);
    draw_alpha_mask_fill(_d, Int_val(i));
    CAMLreturn(Val_unit);
}

value _alpha_mask_set(value d, value x, value y, value v) {
    CAMLparam4(d, x, y, v);
    drawing _d = Draw_val(d);
    draw_alpha_mask_set(_d, Int_val(x), Int_val(y), Int_val(v));
    CAMLreturn(Val_unit);
}

value _alpha_mask_get(value d, value x, value y){
    CAMLparam3(d, x, y);
    CAMLlocal1(v);

    drawing _d = Draw_val(d);
    v = Int_val(draw_alpha_mask_get(_d, Int_val(x), Int_val(y)));
    CAMLreturn(v);
}

value _join(value d, value e) {
    CAMLparam2(d, e);
    drawing _d = Draw_val(d);
    drawing _e = Draw_val(e);
    draw_join(_d, _e);
    CAMLreturn(Val_unit);
}

value _concat(value d, value e){
    CAMLparam2(d, e);
    drawing _d = Draw_val(d);
    drawing _e = Draw_val(e);
    draw_concat(_d, _e);
    CAMLreturn(Val_unit);
}

value _get_transform_matrix(value d){
    CAMLparam1(d);
    drawing _d = Draw_val(d);
    CAMLlocal1(m);
    m = alloc_transform(draw_get_transform_matrix(_d));
    CAMLreturn(m);
}

value _fill_gradient (value d, value g, value i, value j, value t){
    CAMLparam5(d, g, i, j, t);
    drawing _d = Draw_val(d);
    gradient _g = Gradient_val(g);
    draw_fill_gradient(_d, _g, Int_val(i), Int_val(j), Int_val(t));
    CAMLreturn(Val_unit);
}

value _stroke_gradient (value d, value g, value i, value j, value t){
    CAMLparam5(d, g, i, j, t);
    drawing _d = Draw_val(d);
    gradient _g = Gradient_val(g);
    draw_stroke_gradient(_d, _g, Int_val(i), Int_val(j), Int_val(t));
    CAMLreturn(Val_unit);
}

value _transform_create(value unit){
    CAMLparam1(unit);
    CAMLlocal1(t);
    t = alloc_transform(draw_transform_matrix_create());
    CAMLreturn(t);
}

value _transform_scale(value m, value a, value b){
    CAMLparam3(m, a, b);
    transform_matrix _m = Transform_val(m);
    draw_transform_matrix_scale(_m, Double_val(a), Double_val(b));
    CAMLreturn(Val_unit);
}

value _transform_translate(value m, value a, value b){
    CAMLparam3(m, a, b);
    transform_matrix _m = Transform_val(m);
    draw_transform_matrix_translate(_m, Double_val(a), Double_val(b));
    CAMLreturn(Val_unit);
}

value _transform_rotate(value m, value a){
    CAMLparam2(m, a);
    transform_matrix _m = Transform_val(m);
    draw_transform_matrix_rotate(_m, Double_val(a));
    CAMLreturn(Val_unit);
}

value _transform_transform(value m, value x, value y) {
    CAMLparam3(m, x, y);
    transform_matrix _m = Transform_val(m);
    double _x = Double_val(x);
    double _y = Double_val(y);
    draw_transform_matrix_transform(_m, &_x, &_y);

    TUPLE_D2(dst, _x, _y);
    CAMLreturn(dst);
}

value _transform_inverse_transform(value m, value x, value y) {
    CAMLparam3(m, x, y);
    transform_matrix _m = Transform_val(m);
    double _x = Double_val(x);
    double _y = Double_val(y);
    draw_transform_matrix_inverse_transform(_m, &_x, &_y);

    TUPLE_D2(dst, _x, _y);
    CAMLreturn(dst);
}

value _gradient_create(value unit){
    CAMLparam1(unit);
    CAMLlocal1(g);
    g = alloc_gradient(draw_gradient_create ());
    CAMLreturn(g);
}

value _gradient_add_stop(value g, value c){
    CAMLparam2(g, c);
    gradient _g = Gradient_val(g);
    float v[Wosize_val(c)];
    int i;
    for(i = 0; i < Wosize_val(c); i++){
        v[i] = Field(c, i);
    }
    draw_gradient_add_stop(_g, v);
    CAMLreturn(Val_unit);
}

value _gradient_get_transform_matrix(value g){
    CAMLparam1(g);
    gradient _g = Gradient_val(g);
    CAMLlocal1(m);
    m = alloc_transform(draw_gradient_get_matrix(_g));
    CAMLreturn(m);
}

value _gradient_set_transform_matrix(value g, value m){
    CAMLparam2(g, m);
    gradient _g = Gradient_val(g);
    transform_matrix _m = Transform_val(m);
    draw_gradient_set_matrix(_g, _m);
    CAMLreturn(Val_unit);
}

value _font = 0;

value _text(value d, value x, value y, value str, value sz){
    CAMLparam5(d, x, y, str, sz);
    if (_font == 0){
        failwith("No font selected");
        CAMLreturn(Val_unit);
    }

    value w = Field(sz, 0);
    value h = Field(sz, 1);

    drawing _d = Draw_val(d);
    draw_text(_d, Double_val(x), Double_val(y), String_val(str), String_val(_font), Double_val(w), Double_val(h));
    CAMLreturn(Val_unit);
}

value _set_font(value f){
    CAMLparam1(f);
    _font = copy_string(String_val(f));
    CAMLreturn(Val_unit);
}

value _text_simple(value d, value x, value y, value str, value sz){
    CAMLparam5(d, x, y, str, sz);

    drawing _d = Draw_val(d);

    value s = Field(sz, 0);
    value w = Field(sz, 1);

    draw_text_simple(_d, Double_val(x), Double_val(y), String_val(str), Int_val(s), Double_val(w), _font ? String_val(_font) : NULL);
    CAMLreturn(Val_unit);
}
