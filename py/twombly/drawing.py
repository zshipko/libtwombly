from __future__ import absolute_import
from ctypes import *
from numpy import zeros, arange, ndarray, asarray
from twombly.colors import _colors
import platform, os
from functools import partial

if platform.system() == "Darwin":
    ext = "dylib"
else:
    ext = "so"

twombly = cdll.LoadLibrary("libtwombly." + ext)

PATH_CMD_STOP = 0
PATH_CMD_MOVE_TO = 1
PATH_CMD_LINE_TO = 2
PATH_CMD_CURVE3 = 3
PATH_CMD_CURVE4 = 4
PATH_CMD_CURVEN = 5
PATH_CMD_CATROM = 6
PATH_CMD_UBSPLINE = 7
PATH_CMD_END_POLY = 0x0f
PATH_CMD_MASK = 0x0f

class PathCommand(object):
    stop = PATH_CMD_STOP
    move_to = PATH_CMD_MOVE_TO
    line_to = PATH_CMD_LINE_TO
    curve3 = PATH_CMD_CURVE3
    curve4 = PATH_CMD_CURVE4
    curven = PATH_CMD_CURVEN
    catrom = PATH_CMD_CATROM
    ubspline = PATH_CMD_UBSPLINE
    end_poly = PATH_CMD_END_POLY
    mask = PATH_CMD_MASK

    class flags(object):
        none = 0
        ccw = 0x10
        cw = 0x20
        close = 0x40
        mask = 0xF0

GRADIENT_CIRCLE = 0
GRADIENT_RADIAL = 1
GRADIENT_RADIAL_D = 2
GRADIENT_RADIAL_FOCUS = 3
GRADIENT_X = 4
GRADIENT_Y = 5
GRADIENT_DIAMOND = 6
GRADIENT_XY = 7
GRADIENT_SQRT_XY = 8
GRADIENT_CONIC = 9

CAP_BUTT = 0
CAP_SQUARE = 1
CAP_ROUND = 2

JOIN_MITER = 0
JOIN_MITER_REVERT = 1
JOIN_ROUND = 2
JOIN_BEVEL = 3
JOIN_MITER_ROUND = 4

class DrawingStyle(object):
    class cap(object):
        butt = CAP_BUTT
        square = CAP_SQUARE
        round = CAP_ROUND

    class join(object):
        miter = JOIN_MITER
        miter_revert = JOIN_MITER_REVERT
        round = JOIN_ROUND
        bevel = JOIN_BEVEL
        miter_round = JOIN_MITER_ROUND

class DrawingType(Structure):
    ''' C struct for drawing type'''
    _fields_ = [
        ("handle", c_void_p),
        ("channels", c_int),
        ("bits_per_channel", c_int),
        ("is_bgr", c_bool)
    ]

class TransformType(Structure):
    ''' C struct for transformation matrix '''
    _fields_ = [
        ("handle", c_void_p),
        ("free_handle", c_int)
    ]

class GradientType(Structure):
    ''' C struct for gradient '''
    _fields_ = [
        ("handle", c_void_p)
    ]

def _method_decl(c_fn, res=None, args=[POINTER(DrawingType)]):
    ''' declare a ctypes function '''
    tmp = c_fn
    tmp.argtypes = args
    tmp.restype = res
    return tmp

_METHODS = dict(
    empty=_method_decl(twombly.draw_empty, POINTER(DrawingType), args=[]),
    create=_method_decl(twombly.draw_create,
                        POINTER(DrawingType), [c_int, c_int, c_int, c_void_p]),
    create16=_method_decl(twombly.draw_create16,
                          POINTER(DrawingType), [c_int, c_int, c_int, c_void_p]),
    create_bgr=_method_decl(twombly.draw_create_bgr,
                            POINTER(DrawingType), [c_int, c_int, c_int, c_void_p]),
    create16_bgr=_method_decl(twombly.draw_create16_bgr,
                              POINTER(DrawingType), [c_int, c_int, c_int, c_void_p]),
    free=_method_decl(twombly.draw_free, args=[POINTER(POINTER(DrawingType))]),
    get_antialias=_method_decl(twombly.draw_get_antialias, c_bool),
    set_antialias=_method_decl(twombly.draw_set_antialias, args=[POINTER(DrawingType), c_bool]),
    get_preserve=_method_decl(twombly.draw_get_preserve, c_bool),
    set_preserve=_method_decl(twombly.draw_set_preserve, args=[POINTER(DrawingType), c_bool]),
    set_line_width=_method_decl(twombly.draw_set_line_width,
                                args=[POINTER(DrawingType), c_double]),
    get_line_width=_method_decl(twombly.draw_get_line_width, c_double),
    set_miter_limit=_method_decl(twombly.draw_set_miter_limit,
                                 args=[POINTER(DrawingType), c_double]),
    get_miter_limit=_method_decl(twombly.draw_get_miter_limit, c_double),
    set_line_join=_method_decl(twombly.draw_set_line_join,
                               args=[POINTER(DrawingType), c_int]),
    get_line_join=_method_decl(twombly.draw_get_line_join, c_int),
    set_line_cap=_method_decl(twombly.draw_set_line_cap,
                              args=[POINTER(DrawingType), c_int]),
    get_line_cap=_method_decl(twombly.draw_get_line_cap, c_int),
    set_active_path=_method_decl(twombly.draw_set_active_path,
                                 args=[POINTER(DrawingType), c_uint]),
    get_active_path=_method_decl(twombly.draw_get_active_path, c_uint),
    new_path=_method_decl(twombly.draw_new_path, c_uint),
    rotate=_method_decl(twombly.draw_rotate, args=[POINTER(DrawingType), c_double]),
    translate=_method_decl(twombly.draw_translate,
                           args=[POINTER(DrawingType), c_double, c_double]),
    scale=_method_decl(twombly.draw_scale,
                       args=[POINTER(DrawingType), c_double]),
    scale2=_method_decl(twombly.draw_scale2,
                        args=[POINTER(DrawingType), c_double, c_double]),
    skew=_method_decl(twombly.draw_skew,
                      args=[POINTER(DrawingType), c_double, c_double]),
    clear_transforms=_method_decl(twombly.draw_clear_transforms),
    close_polygon=_method_decl(twombly.draw_close_polygon),
    end_polygon=_method_decl(twombly.draw_end_polygon),
    invert_polygon=_method_decl(twombly.draw_invert_polygon),
    reset=_method_decl(twombly.draw_reset),
    clear=_method_decl(twombly.draw_clear,
                       args=[POINTER(DrawingType), c_uint8, c_uint8, c_uint8, c_uint8]),
    remove_all=_method_decl(twombly.draw_remove_all),
    ellipse=_method_decl(twombly.draw_ellipse,
                         args=[POINTER(DrawingType), c_double, c_double, c_double, c_double]),
    rect=_method_decl(twombly.draw_rect,
                      args=[POINTER(DrawingType), c_double, c_double, c_double, c_double]),
    clip=_method_decl(twombly.draw_clip,
                      args=[POINTER(DrawingType), c_double, c_double, c_double, c_double]),
    reset_clip=_method_decl(twombly.draw_reset_clip),
    last_x=_method_decl(twombly.draw_last_x, c_double),
    last_y=_method_decl(twombly.draw_last_y, c_double),
    rel_to_abs=_method_decl(twombly.draw_rel_to_abs,
                            args=[POINTER(DrawingType), POINTER(c_double), POINTER(c_double)]),
    move_to=_method_decl(twombly.draw_move_to,
                         args=[POINTER(DrawingType), c_double, c_double]),
    move_rel=_method_decl(twombly.draw_move_rel,
                          args=[POINTER(DrawingType), c_double, c_double]),
    line_to=_method_decl(twombly.draw_line_to,
                         args=[POINTER(DrawingType), c_double, c_double]),
    vline_to=_method_decl(twombly.draw_vline_to,
                         args=[POINTER(DrawingType), c_double]),
    hline_to=_method_decl(twombly.draw_hline_to,
                         args=[POINTER(DrawingType), c_double]),
    vline_rel=_method_decl(twombly.draw_vline_rel,
                         args=[POINTER(DrawingType), c_double]),
    hline_rel=_method_decl(twombly.draw_hline_rel,
                         args=[POINTER(DrawingType), c_double]),
    line_rel=_method_decl(twombly.draw_line_rel,
                          args=[POINTER(DrawingType), c_double, c_double]),
    curve_to2=_method_decl(twombly.draw_curve_to2,
                           args=[POINTER(DrawingType), c_double, c_double]),
    curve_rel2=_method_decl(twombly.draw_curve_rel2,
                            args=[POINTER(DrawingType), c_double, c_double]),
    curve_to4=_method_decl(twombly.draw_curve_to4,
                           args=[POINTER(DrawingType), c_double, c_double, c_double, c_double]),
    curve_rel4=_method_decl(twombly.draw_curve_rel4,
                            args=[POINTER(DrawingType), c_double, c_double, c_double, c_double]),
    curve_to6=_method_decl(twombly.draw_curve_to6,
                           args=[POINTER(DrawingType), c_double, c_double, c_double, c_double, c_double, c_double]),
    curve_rel6=_method_decl(twombly.draw_curve_rel6,
                            args=[POINTER(DrawingType), c_double, c_double, c_double, c_double, c_double, c_double]),
    arc_to=_method_decl(twombly.draw_arc_to,
                        args=[POINTER(DrawingType), c_double, c_double, c_double, c_double, c_double]),
    arc_rel=_method_decl(twombly.draw_arc_rel,
                         args=[POINTER(DrawingType), c_double, c_double, c_double, c_double, c_double]),
    text_simple=_method_decl(twombly.draw_text_simple, c_double,
                                 args=[POINTER(DrawingType), c_double, c_double, c_char_p, c_int, c_double, c_char_p]),
    set_color=_method_decl(twombly.draw_set_color,
                           args=[POINTER(DrawingType), c_uint8, c_uint8, c_uint8, c_uint8]),
    fill=_method_decl(twombly.draw_fill),
    stroke=_method_decl(twombly.draw_stroke),
    stroke_color=_method_decl(twombly.draw_stroke_color, args=[POINTER(DrawingType), POINTER(c_float)]),
    fill_color=_method_decl(twombly.draw_fill_color, args=[POINTER(DrawingType), POINTER(c_float)]),
    dash_color=_method_decl(twombly.draw_stroke_color, args=[POINTER(DrawingType), POINTER(c_float), c_double, c_double]),
    dash=_method_decl(
        twombly.draw_dash, args=[POINTER(DrawingType), c_double, c_double]),
    paint=_method_decl(twombly.draw_paint),
    auto_close=_method_decl(twombly.draw_auto_close,
                            args=[POINTER(DrawingType), c_bool]),
    in_path=_method_decl(twombly.draw_in_path, c_bool,
                         args=[POINTER(DrawingType), c_double, c_double]),
    is_drawn=_method_decl(twombly.draw_in_path, c_bool,
                         args=[POINTER(DrawingType), c_double, c_double]),
    get_vertex=_method_decl(twombly.draw_get_vertex, c_uint,
                            args=[POINTER(DrawingType), c_uint, POINTER(c_double), POINTER(c_double)]),
    next_vertex=_method_decl(twombly.draw_next_vertex, c_uint,
                             args=[POINTER(DrawingType), POINTER(c_double), POINTER(c_double)]),
    get_command=_method_decl(twombly.draw_get_command, c_uint,
                             args=[POINTER(DrawingType), c_uint]),
    last_vertex=_method_decl(twombly.draw_last_vertex, c_uint,
                             args=[POINTER(DrawingType), POINTER(c_double), POINTER(c_double)]),
    prev_vertex=_method_decl(twombly.draw_prev_vertex, c_uint,
                             args=[POINTER(DrawingType), POINTER(c_double), POINTER(c_double)]),
    modify_vertex=_method_decl(twombly.draw_modify_vertex,
                               args=[POINTER(DrawingType), c_uint, c_double, c_double, c_uint]),
    total_vertices=_method_decl(twombly.draw_total_vertices, c_uint),
    join=_method_decl(twombly.draw_join,
                      args=[POINTER(DrawingType), POINTER(DrawingType)]),
    concat=_method_decl(twombly.draw_concat,
                        args=[POINTER(DrawingType), POINTER(DrawingType)]),
    # alpha mask
    alpha_mask_init=_method_decl(twombly.draw_alpha_mask_init),
    alpha_mask_free=_method_decl(twombly.draw_alpha_mask_free),
    alpha_mask_fill=_method_decl(twombly.draw_alpha_mask_fill,
                                  args=[POINTER(DrawingType), c_uint8]),
    alpha_mask_get=_method_decl(twombly.draw_alpha_mask_get, c_uint8,
                                 args=[POINTER(DrawingType), c_int32, c_int32]),
    alpha_mask_set=_method_decl(twombly.draw_alpha_mask_set,
                                 args=[POINTER(DrawingType), c_int32, c_int32, c_uint8]),
    alpha_mask_ptr_offs=_method_decl(twombly.draw_alpha_mask_ptr_offs, POINTER(c_uint8),
                                          args=[POINTER(DrawingType), c_int32, c_int32]),
    alpha_mask_ptr=_method_decl(twombly.draw_alpha_mask_ptr, POINTER(c_uint8)),

    # gradient
    fill_gradient=_method_decl(twombly.draw_fill_gradient,
                               args=[POINTER(DrawingType), POINTER(GradientType), c_int, c_int, c_int]),
    stroke_gradient=_method_decl(twombly.draw_stroke_gradient,
                                 args=[POINTER(DrawingType), POINTER(GradientType), c_int, c_int, c_int]),
)

try:
    _METHODS['text']= _method_decl(twombly.draw_text, c_double,
                          args=[POINTER(DrawingType), c_double, c_double, c_char_p, c_char_p, c_double, c_double])
except:
    pass

_transform_matrix_create = _method_decl(twombly.draw_transform_matrix_create, POINTER(TransformType), args=[])
_transform_matrix_free = _method_decl(twombly.draw_transform_matrix_free, args=[POINTER(POINTER(TransformType))])
_transform_matrix_get = _method_decl(twombly.draw_get_transform_matrix, POINTER(TransformType), args=[POINTER(DrawingType)])
_transform_matrix_to_double = _method_decl(twombly.draw_transform_matrix_to_double, args=[POINTER(TransformType), POINTER(c_double)])
_transform_matrix_from_double = _method_decl(twombly.draw_transform_matrix_from_double, args=[POINTER(TransformType), POINTER(c_double)])
_transform_matrix_inverse_transform = _method_decl(twombly.draw_transform_matrix_inverse_transform, args=[POINTER(TransformType), POINTER(c_double), POINTER(c_double)])
_transform_matrix_transform = _method_decl(twombly.draw_transform_matrix_transform, args=[POINTER(TransformType), POINTER(c_double), POINTER(c_double)])
_transform_matrix_translate = _method_decl(twombly.draw_transform_matrix_translate, args=[POINTER(TransformType), c_double, c_double])
_transform_matrix_rotate = _method_decl(twombly.draw_transform_matrix_rotate, args=[POINTER(TransformType), c_double])
_transform_matrix_reset =  _method_decl(twombly.draw_transform_matrix_reset, args=[POINTER(TransformType)])
_transform_matrix_scale = _method_decl(twombly.draw_transform_matrix_scale, args=[POINTER(TransformType), c_double, c_double])


class TransformMatrix(object):
    '''
        transforms points and drawings
    '''
    def __init__(self, m=None):
        if m is None:
            m = _transform_matrix_create()
            self._free = True
        else: self._free = False

        self._mtx = m
        _as_parameter_ = self._mtx

    def scale(self, x, y):
        _transform_matrix_scale(self._mtx, x, y)

    def translate(self, x, y):
        _transform_matrix_translate(self._mtx, x, y)

    def rotate(self, a):
        _transform_matrix_rotate(self._mtx, a)

    def reset(self):
        ''' clear all transformations'''
        _transform_matrix_reset(self._mtx)

    def __del__(self):
        if self._free and self._mtx is not None:
            _transform_matrix_free(pointer(self._mtx))
            self._free = False
            self._mtx = None

    def transform(self, x, y, inverse=False):
        ''' transform a point '''
        x_ptr = pointer(c_double(x))
        y_ptr = pointer(c_double(y))
        if inverse:
            _transform_matrix_inverse_transform(self._mtx, x_ptr, y_ptr)
        else:
            _transform_matrix_transform(self._mtx, x_ptr, y_ptr)
        return (x_ptr[0], y_ptr[0])

    @property
    def array(self):
        ''' Get and set transformation matrix data using numpy arrays'''
        arr = zeros(6, dtype="double")
        _transform_matrix_to_double(self._mtx, cast(arr.ctypes.data, POINTER(c_double)))
        return arr

    @array.setter
    def array(self, arr):
        _transform_matrix_from_double(self._mtx, cast(asarray(arr, dtype='double').ctypes.data,
                                                          POINTER(c_double)))

_gradient_create = _method_decl(twombly.draw_gradient_create, POINTER(GradientType), args=[])
_gradient_create16 = _method_decl(twombly.draw_gradient_create16, POINTER(GradientType), args=[])
_gradient_free = _method_decl(twombly.draw_gradient_free, args=[POINTER(POINTER(GradientType))])
_gradient_add_stop = _method_decl(twombly.draw_gradient_add_stop, args=[POINTER(GradientType), POINTER(c_float)])
_gradient_add_stop16 = _method_decl(twombly.draw_gradient_add_stop16, args=[POINTER(GradientType), POINTER(c_float)])
_gradient_get_matrix = _method_decl(twombly.draw_gradient_get_matrix, POINTER(TransformType), args=[POINTER(GradientType)])

class Gradient(object):
    ''' Gradient Class '''
    circle = GRADIENT_CIRCLE
    radial = GRADIENT_RADIAL
    radial_d = GRADIENT_RADIAL_D
    radial_focus = GRADIENT_RADIAL_FOCUS
    x = GRADIENT_X
    y = GRADIENT_Y
    diamond = GRADIENT_DIAMOND
    xy = GRADIENT_XY
    sqrt_xy = GRADIENT_SQRT_XY
    conic = GRADIENT_CONIC


    def __init__(self, *args, **kw):
        self.depth = kw.get('depth', 8)
        self._gradient= None

        if  self.depth == 16:
            self._gradient = _gradient_create16()
        else:
            self._gradient = _gradient_create()

        for arg in args:
            self.add_stop(Color(arg))

        self._as_parameter_ = self._gradient

    def add_stop(self, c):
        if self.depth == 8:
            _gradient_add_stop(self._gradient, (c_float * 4)(*c))
        elif self.depth == 16:
            _gradient_add_stop16(self._gradient, (c_float * 4)(*c))

    @property
    def matrix(self):
        return TransformMatrix(_gradient_get_matrix(self._gradient))

    def __del__(self):
        if hasattr(self, "_gradient") and self._gradient is not None:
            _gradient_free(pointer(self._gradient))
            self._gradient = None

def as_uint8(arr):
    return [c_uint8(int(i)) for i in arr]

def as_uint16(arr):
    return [c_uint16(int(i)) for i in arr]

class Color(ndarray):
    ''' a a numpy array of 4 floats '''
    def __new__(cls, red, green=None, blue=None, alpha=255):
        return ndarray.__new__(cls, 4, 'float')

    def __init__(self, red, green=None, blue=None, alpha=255):
        if isinstance(red, str):
            red, green, blue, alpha = _colors.get(red.lower().replace(' ', ''), [0, 0, 0, alpha])
        elif green is None or blue is None:
            green = blue = red
        elif hasattr(red, '__getitem__') and hasattr(red, '__len__'):
            arr = red
            if len(red) == 3:
                red.append(alpha)
            elif len(red) != 4:
                raise Exception("bad color")
            red, green, blue, alpha = arr[0], arr[1], arr[2], arr[3]

        self[0] = red
        self[1] = green
        self[2] = blue
        self[3] = alpha

        _as_parameter_ = (c_float * 4)(*self)

    def as_uint8(self):
        ''' convert red, green, blue, alpha to chars 0-255 '''
        return as_uint8(self)

    def as_uint16(self):
        ''' convert red, green, blue, alpha to uint16s 0-65535 '''
        return as_uint16(self)

class Vertex(list):
    def __init__(self, iterable, update_fn=None):
        list.__init__(self, iterable)
        self.update_fn = update_fn

    @property
    def x(self):
        return self[0]

    @property
    def y(self):
        return self[1]

    @property
    def command(self):
        return self[2]

    @command.setter
    def command(self, cmd):
        self[2] = cmd
        if self.update_fn is not None:
            self.update_fn(*self)

    @x.setter
    def x(self, val):
        self[0] = val
        if self.update_fn is not None:
            self.update_fn(*self)

    @y.setter
    def y(self, val):
        self[1] = val
        if self.update_fn is not None:
            self.update_fn(*self)

class Drawing(object):
    ''' python wrapper for libtwombly Drawing class '''
    def __init__(self, arr, bgr=False, width=None, height=None):
        self._free = _METHODS["free"]
        self._color = Color(0, 0, 0, 0)

        self.array = arr
        bgr_str = ""
        if bgr:
            bgr_str = "_bgr"

        if not width:
            width = arr.shape[1]

        if not height:
            height = arr.shape[0]

        if arr.dtype == 'uint8':
            self._drawing = _METHODS["create" + bgr_str](width, height,
                                                         arr.shape[2], arr.ravel().ctypes.data)
            self._is_16 = False
        elif arr.dtype == 'uint16':
            self._drawing = _METHODS["create16" + bgr_str](width, height,
                                                           arr.shape[2], arr.ravel().ctypes.data)
            self._is_16 = True
        else:
            self._drawing = None
            raise ValueError("bad image type")

        self._as_parameter_ = self._drawing

    def __getattr__(self, key):
        if key in _METHODS:
            def wrapper(*args):
                ''' get method by name'''
                return _METHODS[key](self._drawing, *args)
            return wrapper
        raise AttributeError

    @property
    def total_vertices(self):
        return _METHODS["total_vertices"](self)

    @property
    def vertices(self):
        class Vertices(object):
            def __init__(_self):
                _self._index = 0

            def __setitem__(_self, index, val):
                self.modify_vertex(index, val[0], val[1], val[2])

            def __getitem__(_self, index):
                return self.get_vertex(index)

            def __len__(_self):
                return self.total_vertices

            def __iter__(_self):
                return _self

            def __next__(_self):
                if _self.index >= self.total_vertices:
                    raise StopIteration

                result = self.get_vertex(_self.index)
                _self.index += 1
                return result

            def __str__(_self):
                return 'Vertices{%d}' % self.total_vertices

        return Vertices()

    @vertices.setter
    def vertices(self, arr):
        for index, vertex in enumerate(arr):
            self.set_vertex(index, vertex[0], vertex[1], vertex[2])

    def get_vertex(self, index):
        if index >= self.total_vertices:
            raise Exception("out of bounds")
        x_ptr = pointer(c_double(0))
        y_ptr = pointer(c_double(0))
        _METHODS["get_vertex"](self, index, x_ptr, y_ptr)
        cmd = _METHODS["get_command"](self, index)
        return Vertex([x_ptr[0], y_ptr[0], cmd], update_fn=partial(self.set_vertex, index))

    def set_vertex(self, index, x, y, cmd=None):
        if cmd is None:
            cmd = self.get_command(index)
        self.modify_vertex(index, x, y, cmd)

    def clear(self, r, g=None, b=None, a=255):
        if isinstance(r, Color):
            _METHODS["clear"](self._drawing, *r.as_uint8())
        else:
            _METHODS["clear"](self._drawing, *Color(r, g, b, a).as_uint8())

    @property
    def color(self):
        return self._color

    @color.setter
    def color(self, r, g=None, b=None, a=255):
        if isinstance(r, str):
            r = Color(r)
        elif isinstance(r, (tuple, list)) and len(r) >= 3:
            g = r[1]
            b = r[2]
            if len(r) > 3:
                a = r[3]
            r = r[0]

        if isinstance(r, Color):
            _METHODS["set_color"](self._drawing, *r.as_uint8())
            self._color = r.as_uint8()
        else:
            _METHODS["set_color"](self._drawing, *Color(r, g, b, a).as_uint8())
            self._color = Color(r, g, b, a).as_uint8()

    def curve_to(self, a, b, c=None, d=None, e=None, f=None):
        if c is None or d is None:
            return self.curve_to2(a, b)
        elif e is None or f is None:
            return self.curve_to4(a, b, c, d)
        return self.curve_to6(a, b, c, d, e, f)

    def curve_rel(self, a, b, c=None, d=None, e=None, f=None):
        if c is None or d is None:
            return self.curve_rel2(a, b)
        elif e is None or f is None:
            return self.curve_rel4(a, b, c, d)
        return self.curve_rel6(a, b, c, d, e, f)

    def __del__(self):
        if self._drawing is not None:
            self._free(self._drawing)
            self._drawing = None

    @property
    def matrix(self):
        return TransformMatrix(_transform_matrix_get(self))

    @property
    def antialias(self):
        return self.get_antialias()

    @antialias.setter
    def antialias(self, aa):
        self.set_antialias(aa)

    @property
    def preserve(self):
        return self.get_preserve()

    @preserve.setter
    def preserve(self, p):
        self.set_preserve(p)

    @property
    def line_width(self):
        return self.get_line_width()

    @line_width.setter
    def line_width(self, w):
        self.set_line_width(w)

    @property
    def miter_limit(self):
        return self.get_miter_limit()

    @miter_limit.setter
    def miter_limit(self, limit):
        self.set_miter_limit(limit)

    @property
    def line_join(self):
        return self.get_line_join()

    @line_join.setter
    def line_join(self, j):
        return self.set_line_join(j)

    @property
    def line_cap(self):
        return self.get_line_cap()

    @line_cap.setter
    def line_cap(self, cap):
        self.set_line_cap(cap)

    @property
    def path(self):
        return self.get_active_path()

    @path.setter
    def path(self, pth):
        self.set_active_path(pth)

    def alpha(self, x=0, y=0, val=None):
        self.alpha_mask_init()

        if val is None:
            return self.alpha_mask_ptr_offs(x, y)[0]
        else:
            self.alpha_mask_set(x, y, val)

def draw(arr, *args, **kwargs):
    return Drawing(arr, *args, **kwargs)

def new_image(width, height, channels=3, depth='uint8'):
    return zeros((height, width, channels), depth)


