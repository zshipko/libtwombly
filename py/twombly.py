from ctypes import *
from numpy import zeros, arange, ndarray, asarray
from twombly_colors import _colors

twombly = cdll.LoadLibrary("libtwombly.so")

PATH_CMD_STOP = 0
PATH_CMD_MOVE_TO = 1
PATH_CMD_LINE_TO = 2
PATH_CMD_CURVE3 = 3
PATH_CMD_CURVE4 = 4
PATH_CMD_CURVEN = 5
PATH_CMD_CATROM = 6
PATH_CMD_UBSPLINE = 7
PATH_CMD_END_POLY = 0x0F
PATH_CMD_MASK = 0x0F

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
        ("handle", c_void_p)
    ]

class GradientType(Structure):
    ''' C struct for gradient '''
    _fields_ = [
        ("handle", c_void_p)
    ]

def _method_decl(c_fn, res=None, args=[DrawingType]):
    ''' declare a ctypes function '''
    tmp = c_fn
    tmp.argtypes = args
    tmp.restype = res
    return tmp

_METHODS = dict(
    create_path=_method_decl(twombly.draw_createPath, DrawingType, args=[]),
    create=_method_decl(twombly.draw_create,
                        DrawingType, [c_long, c_long, c_long, c_void_p]),
    create16=_method_decl(twombly.draw_create16,
                          DrawingType, [c_long, c_long, c_long, c_void_p]),
    create_bgr=_method_decl(twombly.draw_create_bgr,
                            DrawingType, [c_long, c_long, c_long, c_void_p]),
    create16_bgr=_method_decl(twombly.draw_create16_bgr,
                              DrawingType, [c_long, c_long, c_long, c_void_p]),
    free=_method_decl(twombly.draw_free, args=[POINTER(DrawingType)]),
    get_antialias=_method_decl(twombly.draw_getAntialias, c_bool),
    set_antialias=_method_decl(twombly.draw_getAntialias,
                               args=[DrawingType, c_bool]),
    set_line_width=_method_decl(twombly.draw_setLineWidth,
                                args=[DrawingType, c_double]),
    get_line_width=_method_decl(twombly.draw_getLineWidth, c_double),
    set_miter_limit=_method_decl(twombly.draw_setMiterLimit,
                                 args=[DrawingType, c_double]),
    get_miter_limit=_method_decl(twombly.draw_getMiterLimit, c_double),
    set_line_join=_method_decl(twombly.draw_setLineJoin,
                               args=[DrawingType, c_int]),
    get_line_join=_method_decl(twombly.draw_getLineJoin, c_int),
    set_line_cap=_method_decl(twombly.draw_setLineCap,
                              args=[DrawingType, c_int]),
    get_line_cap=_method_decl(twombly.draw_getLineCap, c_int),
    set_active_path=_method_decl(twombly.draw_setActivePath,
                                 args=[DrawingType, c_uint]),
    get_active_path=_method_decl(twombly.draw_getActivePath, c_uint),
    new_path=_method_decl(twombly.draw_newPath, c_uint),
    rotate=_method_decl(twombly.draw_rotate, args=[DrawingType, c_double]),
    translate=_method_decl(twombly.draw_translate,
                           args=[DrawingType, c_double, c_double]),
    scale=_method_decl(twombly.draw_scale,
                       args=[DrawingType, c_double]),
    scale2=_method_decl(twombly.draw_scale2,
                        args=[DrawingType, c_double, c_double]),
    skew=_method_decl(twombly.draw_skew,
                      args=[DrawingType, c_double, c_double]),
    clear_transforms=_method_decl(twombly.draw_clearTransforms),
    close_polygon=_method_decl(twombly.draw_closePolygon),
    end_polygon=_method_decl(twombly.draw_endPolygon),
    invert_polygon=_method_decl(twombly.draw_invertPolygon),
    reset=_method_decl(twombly.draw_reset),
    clear=_method_decl(twombly.draw_clear,
                       args=[DrawingType, c_uint8, c_uint8, c_uint8, c_uint8]),
    remove_paths=_method_decl(twombly.draw_removePaths),
    ellipse=_method_decl(twombly.draw_ellipse,
                         args=[DrawingType, c_double, c_double, c_double, c_double]),
    rect=_method_decl(twombly.draw_rect,
                      args=[DrawingType, c_double, c_double, c_double, c_double]),
    clip=_method_decl(twombly.draw_clip,
                      args=[DrawingType, c_double, c_double, c_double, c_double]),
    reset_clip=_method_decl(twombly.draw_resetClip),
    last_x=_method_decl(twombly.draw_lastX, c_double),
    last_y=_method_decl(twombly.draw_lastY, c_double),
    rel_to_abs=_method_decl(twombly.draw_relToAbs,
                            args=[DrawingType, POINTER(c_double), POINTER(c_double)]),
    move_to=_method_decl(twombly.draw_moveTo,
                         args=[DrawingType, c_double, c_double]),
    move_rel=_method_decl(twombly.draw_moveRel,
                          args=[DrawingType, c_double, c_double]),
    line_to=_method_decl(twombly.draw_lineTo,
                         args=[DrawingType, c_double, c_double]),
    vline_to=_method_decl(twombly.draw_vLineTo,
                         args=[DrawingType, c_double]),
    hline_to=_method_decl(twombly.draw_hLineTo,
                         args=[DrawingType, c_double]),
    vline_rel=_method_decl(twombly.draw_vLineRel,
                         args=[DrawingType, c_double]),
    hline_rel=_method_decl(twombly.draw_hLineRel,
                         args=[DrawingType, c_double]),
    line_rel=_method_decl(twombly.draw_lineRel,
                          args=[DrawingType, c_double, c_double]),
    curve_to2=_method_decl(twombly.draw_curveTo2,
                           args=[DrawingType, c_double, c_double]),
    curve_rel2=_method_decl(twombly.draw_curveRel2,
                            args=[DrawingType, c_double, c_double]),
    curve_to4=_method_decl(twombly.draw_curveTo4,
                           args=[DrawingType, c_double, c_double, c_double, c_double]),
    curve_rel4=_method_decl(twombly.draw_curveRel4,
                            args=[DrawingType, c_double, c_double, c_double, c_double]),
    curve_to6=_method_decl(twombly.draw_curveTo6,
                           args=[DrawingType, c_double, c_double, c_double, c_double, c_double, c_double]),
    curve_rel6=_method_decl(twombly.draw_curveRel6,
                            args=[DrawingType, c_double, c_double, c_double, c_double, c_double, c_double]),
    arc_to=_method_decl(twombly.draw_arcTo,
                        args=[DrawingType, c_double, c_double, c_double, c_double, c_double]),
    arc_rel=_method_decl(twombly.draw_arcRel,
                         args=[DrawingType, c_double, c_double, c_double, c_double, c_double]),
    text_simple=_method_decl(twombly.draw_textSimple, c_double,
                                 args=[DrawingType, c_double, c_double, c_char_p, c_int, c_double, c_char_p]),
    text=_method_decl(twombly.draw_text, c_double,
                          args=[DrawingType, c_double, c_double, c_char_p, c_char_p, c_double, c_double]),
    set_color=_method_decl(twombly.draw_setColor,
                           args=[DrawingType, c_uint8, c_uint8, c_uint8, c_uint8]),
    fill=_method_decl(twombly.draw_fill),
    stroke=_method_decl(twombly.draw_stroke),
    dash=_method_decl(
        twombly.draw_dash, args=[DrawingType, c_double, c_double]),
    paint=_method_decl(twombly.draw_paint),
    auto_close=_method_decl(twombly.draw_autoClose,
                            args=[DrawingType, c_bool]),
    in_path=_method_decl(twombly.draw_inPath, c_bool,
                         args=[DrawingType, c_double, c_double]),
    get_vertex=_method_decl(twombly.draw_getVertex, c_uint,
                            args=[DrawingType, c_uint, POINTER(c_double), POINTER(c_double)]),
    next_vertex=_method_decl(twombly.draw_nextVertex, c_uint,
                             args=[DrawingType, POINTER(c_double), POINTER(c_double)]),
    get_command=_method_decl(twombly.draw_getCommand, c_uint,
                             args=[DrawingType, c_uint]),
    last_vertex=_method_decl(twombly.draw_lastVertex, c_uint,
                             args=[DrawingType, POINTER(c_double), POINTER(c_double)]),
    prev_vertex=_method_decl(twombly.draw_prevVertex, c_uint,
                             args=[DrawingType, POINTER(c_double), POINTER(c_double)]),
    modify_vertex=_method_decl(twombly.draw_modifyVertex,
                               args=[DrawingType, c_uint, c_double, c_double, c_uint]),
    total_vertices=_method_decl(twombly.draw_totalVertices, c_uint),
    join=_method_decl(twombly.draw_join,
                      args=[DrawingType, DrawingType]),
    concat=_method_decl(twombly.draw_concat,
                        args=[DrawingType, DrawingType]),

    # pattern
    fill_pattern=_method_decl(twombly.draw_fillPattern,
                              args=[DrawingType, DrawingType]),
    stroke_pattern=_method_decl(twombly.draw_fillPattern,
                                args=[DrawingType, DrawingType]),
    # alpha mask
    alpha_mask_init=_method_decl(twombly.draw_alphaMaskInit),
    alpha_mask_free=_method_decl(twombly.draw_alphaMaskFree),
    alpha_mask_fill=_method_decl(twombly.draw_alphaMaskFill,
                                  args=[DrawingType, c_uint8]),
    alpha_mask=_method_decl(twombly.draw_alphaMaskGet, c_uint8,
                                 args=[DrawingType, c_int32, c_int32]),
    alpha_mask_ptr_offs=_method_decl(twombly.draw_alphaMaskPtrOffs, POINTER(c_uint8),
                                          args=[DrawingType, c_int32, c_int32]),
    alpha_mask_ptr=_method_decl(twombly.draw_alphaMaskPtr, POINTER(c_uint8)),

    # gradient
    fill_gradient=_method_decl(twombly.draw_fillGradient,
                               args=[DrawingType, GradientType, c_int, c_int, c_int]),
    stroke_gradient=_method_decl(twombly.draw_strokeGradient,
                                 args=[DrawingType, GradientType, c_int, c_int, c_int]),
)

_transform_matrix_create = _method_decl(twombly.draw_transformMatrixCreate, TransformType, args=[])
_transform_matrix_free = _method_decl(twombly.draw_transformMatrixFree, args=[POINTER(TransformType)])
_transform_matrix_get = _method_decl(twombly.draw_getTransformMatrix, TransformType, args=[DrawingType])
_transform_matrix_to_double = _method_decl(twombly.draw_transformMatrixToDouble, args=[TransformType, POINTER(c_double)])
_transform_matrix_from_double = _method_decl(twombly.draw_transformMatrixFromDouble, args=[TransformType, POINTER(c_double)])
_transform_matrix_inverse_transform = _method_decl(twombly.draw_transformMatrixInverseTransform, args=[TransformType, POINTER(c_double), POINTER(c_double)])
_transform_matrix_transform = _method_decl(twombly.draw_transformMatrixTransform, args=[TransformType, POINTER(c_double), POINTER(c_double)])
_transform_matrix_translate = _method_decl(twombly.draw_transformMatrixTranslate, args=[TransformType, c_double, c_double])
_transform_matrix_rotate = _method_decl(twombly.draw_transformMatrixRotate, args=[TransformType, c_double])
_transform_matrix_reset =  _method_decl(twombly.draw_transformMatrixReset, args=[TransformType])
_transform_matrix_scale = _method_decl(twombly.draw_transformMatrixScale, args=[TransformType, c_double, c_double])


class TransformMatrix(object):
    '''
        transforms points and drawings

        >>> mtx = TransformMatrix()
        >>> arr = mtx.array()     # get matrix data
        >>> arr = arr * 2
        >>> mtx.array(arr)        # sets matrix sata
        >>> mtx.transform(10, 10) # returns (20.0, 20.0)
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

    def array(self, arr=None):
        ''' Get and set transformation matrix data using numpy arrays'''
        if arr is not None:
            _transform_matrix_from_double(self._mtx, cast(asarray(arr, dtype='double').ctypes.data,
                                                          POINTER(c_double)))
            return

        arr = zeros(6, dtype="double")
        _transform_matrix_to_double(self._mtx, cast(arr.ctypes.data, POINTER(c_double)))
        return arr

_gradient_create = _method_decl(twombly.draw_gradientCreate, GradientType, args=[])
_gradient_create16 = _method_decl(twombly.draw_gradientCreate16, GradientType, args=[])
_gradient_free = _method_decl(twombly.draw_gradientFree, args=[POINTER(GradientType)])
_gradient_add_stop = _method_decl(twombly.draw_gradientAddStop, args=[GradientType, POINTER(c_float)])
_gradient_add_stop16 = _method_decl(twombly.draw_gradientAddStop16, args=[GradientType, POINTER(c_float)])
_gradient_get_matrix = _method_decl(twombly.draw_gradientGetMatrix, TransformType, args=[GradientType])

class Gradient(object):
    ''' Gradient Class '''
    def __init__(self, *args, **kw):
        self.depth = kw.get('depth', 8)
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

    def matrix(self):
        return TransformMatrix(_gradient_get_matrix(self._gradient))

    def __del__(self):
        if self._gradient is not None:
            _gradient_free(pointer(self._gradient))
            self._gradient = None


_HAS_SYMPY = False
try:
    import sympy as sym
    from sympy import geometry as geo
    from sympy.utilities import lambdify
    _HAS_SYMPY = True
except ImportError:
    pass

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
        elif hasattr(red, '__getitem__'):
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

    def as_uint8(self):
        ''' convert red, green, blue, alpha to chars 0-255 '''
        return as_uint8(self)

    def as_uint16(self):
        ''' convert red, green, blue, alpha to uint16s 0-65535 '''
        return as_uint16(self)

class Drawing(object):
    ''' python wrapper for libtwombly Drawing class '''
    def __init__(self, arr=None, bgr=False, width=None, height=None):
        self._free = _METHODS["free"]

        if arr is None:
            self._drawing = _METHODS["create_path"]()
            self._as_parameter_ = self._drawing
            return

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
        elif arr.dtype == 'uint16':
            self._drawing = _METHODS["create16" + bgr_str](width, height,
                                                           arr.shape[2], arr.ravel().ctypes.data)
        else:
            self._drawing = None
            raise ValueError("bad image type")

        self._as_parameter_ = self._drawing

    def __getattr__(self, key):
        def wrapper(*args):
            ''' get method by name'''
            return _METHODS[key](self._drawing, *args)
        return wrapper

    def get_vertices(self):
        return [self.get_vertex(i) for i in range(0, self.total_vertices())]

    def set_vertices(self, arr):
        for index, vertex in enumerate(arr):
            self.set_vertex(index, vertex[0], vertex[1], vertex[2])

    def set_vertex(self, index, x, y, cmd=None):
        if cmd is None:
            cmd = self.get_command(index)

        self.modify_vertex(index, x, y, cmd)

    def get_vertex(self, index):
        if index >= self.total_vertices():
            raise Exception("out of bounds")
        x_ptr = pointer(c_double(0))
        y_ptr = pointer(c_double(0))
        _METHODS["get_vertex"](self, index, x_ptr, y_ptr)
        cmd = _METHODS["get_command"](self, index)
        return [x_ptr[0], y_ptr[0], cmd]

    def clear(self, r, g=None, b=None, a=255):
        _METHODS["clear"](self._drawing, *Color(r, g, b, a).as_uint8())

    def set_color(self, r, g=None, b=None, a=255):
        _METHODS["set_color"](self._drawing, *Color(r, g, b, a).as_uint8())

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
            self._free(pointer(self._drawing))
            self._drawing = None

    def add_geometry(self, g, move_to=False, step=1):
        ''' draw sympy equations '''
        if not _HAS_SYMPY:
            raise Exception("Not implemented, sympy not installed")

        if isinstance(g, geo.Ellipse):
            self.ellipse(g.center.x, g.center.y, g.hradius, g.vradius)
        elif hasattr(g, 'vertices'):
            try:
                if move_to:
                    self.move_to(g.vertices[0].x, g.vertices[0].y)
                else:
                    self.line_to(g.vertices[0].x, g.vertices[0].y)
                for vertex in g.vertices[1:]:
                    self.line_to(vertex.x, vertex.y)
            except AttributeError:
                if move_to:
                    self.move_to(g.vertices[0][0], g.vertices[0][1])
                else:
                    self.line_to(g.vertices[0][0], g.vertices[0][1])
                for vertex in g.vertices[1:]:
                    self.line_to(vertex[0], vertex[1])
        elif hasattr(g, 'points'):
            if move_to:
                self.move_to(g.points[0].x, g.points[0].y)
            else:
                self.line_to(g.points[0].x, g.points[0].y)
            for point in g.points[1:]:
                self.line_to(point.x, point.y)
        elif hasattr(g, 'x') and hasattr(g, 'y'):
            if move_to:
                self.move_to(g.x, g.y)
            else:
                self.line_to(g.x, g.y)
        elif hasattr(g, 'p1') and hasattr(g, 'p2'):
            if move_to:
                self.move_to(g.p1.x, g.py.y)
            else:
                self.line_to(g.p1.x, g.p1.y)
        elif hasattr(g, 'functions'):
            has_tried_move = False
            for x_pos in arange(g.limits[1], g.limits[2]+1, step):
                if not has_tried_move:
                    has_tried_move = True
                    if move_to:
                        self.move_to(g.functions[0].subs(g.limits[0], x_pos),
                                     g.functions[1].subs(g.limits[0], x_pos))
                        continue
                self.line_to(g.functions[0].subs(g.limits[0], x_pos),
                             g.functions[1].subs(g.limits[0], x_pos))

    def matrix(self):
        return TransformMatrix(_transform_matrix_get(self))


def draw(arr, *args, **kwargs):
    return Drawing(arr, *args, **kwargs)

def new_image(width, height, channels=3, depth='uint8'):
    return zeros((height, width, channels), depth)


