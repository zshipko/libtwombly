from ctypes import *
from numpy import zeros, arange
from twombly_colors import _colors

twombly = cdll.LoadLibrary("libtwombly.so")

class DrawingType(Structure):
    _fields_ = [
            ("handle", c_void_p),
            ("channels", c_int),
            ("bits_per_channel", c_int),
            ("is_bgr", c_bool)
    ]

def _method_decl(c_fn, res=None, args=[DrawingType]):
    tmp = c_fn
    tmp.argtypes = args
    tmp.restype = res
    return tmp

_methods = dict(
    create =_method_decl(twombly.draw_create,
        DrawingType, [c_long, c_long, c_long, c_void_p]),
    create16 =_method_decl(twombly.draw_create16,
        DrawingType, [c_long, c_long, c_long, c_void_p]),
    create_bgr =_method_decl(twombly.draw_create_bgr,
        DrawingType, [c_long, c_long, c_long, c_void_p]),
    create16_bgr =_method_decl(twombly.draw_create16_bgr,
        DrawingType, [c_long, c_long, c_long, c_void_p]),
    free =_method_decl(twombly.draw_free),
    get_antialias =_method_decl(twombly.draw_getAntialias, c_bool),
    set_antialias =_method_decl(twombly.draw_getAntialias,
        args=[DrawingType, c_bool]),
    set_line_width =_method_decl(twombly.draw_setLineWidth,
        args=[DrawingType, c_double]),
    get_line_width =_method_decl(twombly.draw_getLineWidth, c_double),
    set_miter_limit =_method_decl(twombly.draw_setMiterLimit,
        args=[DrawingType, c_double]),
    get_miter_limit =_method_decl(twombly.draw_getMiterLimit, c_double),
    set_line_join =_method_decl(twombly.draw_setLineJoin,
        args=[DrawingType, c_int]),
    get_line_join =_method_decl(twombly.draw_getLineJoin, c_int),
    set_line_cap =_method_decl(twombly.draw_setLineCap,
        args=[DrawingType, c_int]),
    get_line_cap =_method_decl(twombly.draw_getLineCap, c_int),
    set_active_path =_method_decl(twombly.draw_setActivePath,
        args=[DrawingType, c_uint]),
    get_active_path =_method_decl(twombly.draw_getActivePath, c_uint),
    new_path =_method_decl(twombly.draw_newPath, c_uint),
    rotate =_method_decl(twombly.draw_rotate, args=[DrawingType, c_double]),
    translate =_method_decl(twombly.draw_translate,
        args=[DrawingType, c_double, c_double]),
    scale =_method_decl(twombly.draw_scale,
        args=[DrawingType, c_double]),
    scale2 =_method_decl(twombly.draw_scale2,
        args = [DrawingType, c_double, c_double]),
    skew =_method_decl(twombly.draw_skew,
        args = [DrawingType, c_double, c_double]),
    clear_transforms =_method_decl(twombly.draw_clearTransforms),
    close_polygon =_method_decl(twombly.draw_closePolygon),
    end_polygon =_method_decl(twombly.draw_endPolygon),
    invert_polygon =_method_decl(twombly.draw_invertPolygon),
    reset =_method_decl(twombly.draw_reset),
    clear =_method_decl(twombly.draw_clear,
        args = [DrawingType, c_char, c_char, c_char, c_char]),
    remove_paths =_method_decl(twombly.draw_removePaths),
    ellipse =_method_decl(twombly.draw_ellipse,
        args = [DrawingType, c_double, c_double, c_double, c_double]),
    rect =_method_decl(twombly.draw_rect,
        args= [DrawingType, c_double, c_double, c_double, c_double]),
    clip =_method_decl(twombly.draw_clip,
        args= [DrawingType, c_double, c_double, c_double, c_double]),
    reset_clip =_method_decl(twombly.draw_resetClip),
    last_x =_method_decl(twombly.draw_lastX, c_double),
    last_y =_method_decl(twombly.draw_lastY, c_double),
    rel_to_abs =_method_decl(twombly.draw_relToAbs,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    move_to =_method_decl(twombly.draw_moveTo,
            args = [DrawingType, c_double, c_double]),
    move_rel =_method_decl(twombly.draw_moveRel,
            args = [DrawingType, c_double, c_double]),
    line_to =_method_decl(twombly.draw_lineTo,
            args= [DrawingType, c_double, c_double]),
    line_rel =_method_decl(twombly.draw_lineRel,
            args = [DrawingType, c_double, c_double]),
    curve_to2 =_method_decl(twombly.draw_curveTo2,
            args= [DrawingType, c_double, c_double]),
    curve_rel2 =_method_decl(twombly.draw_curveRel2,
            args= [DrawingType, c_double, c_double]),
    curve_to4 =_method_decl(twombly.draw_curveTo4,
            args= [DrawingType, c_double, c_double, c_double, c_double]),
    curve_rel4 =_method_decl(twombly.draw_curveRel4,
            args= [DrawingType, c_double, c_double, c_double, c_double]),
    curve_to6 =_method_decl(twombly.draw_curveTo6,
            args= [DrawingType, c_double, c_double, c_double, c_double, c_double, c_double]),
    curve_rel6 =_method_decl(twombly.draw_curveRel6,
            args= [DrawingType, c_double, c_double, c_double, c_double, c_double, c_double]),
    arc_to =_method_decl(twombly.draw_arcTo,
            args= [DrawingType, c_double, c_double, c_double, c_double, c_double]),
    arc_rel =_method_decl(twombly.draw_arcRel,
            args = [DrawingType, c_double, c_double, c_double, c_double, c_double]),
    put_text_simple =_method_decl(twombly.draw_putTextSimple, c_double,
            args= [DrawingType, c_double, c_double, c_char_p, c_int, c_double, c_char_p]),
    put_text =_method_decl(twombly.draw_putText, c_double,
            args= [DrawingType, c_double, c_double, c_char_p, c_char_p, c_double, c_double]),
    set_color =_method_decl(twombly.draw_setColor,
            args = [DrawingType, c_char, c_char, c_char, c_char]),
    fill =_method_decl(twombly.draw_fill),
    stroke =_method_decl(twombly.draw_stroke),
    dash =_method_decl(twombly.draw_dash, args = [DrawingType, c_double, c_double]),
    paint =_method_decl(twombly.draw_paint),
    auto_close =_method_decl(twombly.draw_autoClose,
            args = [c_bool]),
    in_path =_method_decl(twombly.draw_inPath, c_bool,
            args=[c_double, c_double]),
    get_vertex =_method_decl(twombly.draw_getVertex, c_uint,
            args= [DrawingType, c_uint, POINTER(c_double), POINTER(c_double)]),
    next_vertex =_method_decl(twombly.draw_nextVertex, c_uint,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    get_command =_method_decl(twombly.draw_getCommand, c_uint,
            args = [DrawingType, c_uint]),
    last_vertex =_method_decl(twombly.draw_lastVertex, c_uint,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    prev_vertex =_method_decl(twombly.draw_prevVertex, c_uint,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    modify_vertex =_method_decl(twombly.draw_modifyVertex,
            args = [DrawingType, c_uint, POINTER(c_double), POINTER(c_double), c_uint]),
    total_vertices =_method_decl(twombly.draw_totalVertices, c_uint),
    join =_method_decl(twombly.draw_join,
            args=[DrawingType, DrawingType]),
    concat =_method_decl(twombly.draw_concat,
            args=[DrawingType, DrawingType]),
    fill_pattern =_method_decl(twombly.draw_fillPattern,
            args = [DrawingType, c_long, c_long, c_int, c_char_p]),
    stroke_pattern =_method_decl(twombly.draw_fillPattern,
            args = [DrawingType, c_long, c_long, c_int, c_char_p]),
)

_HAS_SYMPY = False
try:
    import sympy as sym
    from sympy import geometry as geo
    from sympy.utilities import lambdify
    _HAS_SYMPY = True
except ImportError as e:
    pass

class Drawing(object):
    def __init__(self, arr, bgr=False, width=None, height=None):
        self._free = _methods["free"]
        self.array = arr

        bgr_str = ""
        if bgr:
            bgr_str = "_bgr"

        if not width:
            width = arr.shape[1]

        if not height:
            height = arr.shape[0]

        if arr.dtype == 'uint8':
            self._drawing = _methods["create" + bgr_str](width, height, arr.shape[2], arr.ravel().ctypes.data)
        elif arr.dtype == 'uint16':
            self._drawing = _methods["create16" + bgr_str](width, height, arr.shape[2], arr.ravel().ctypes.data)
        else:
            self._drawing= None
            raise ValueError("bad image type")

        self._as_parameter_ = self._drawing


    def __getattr__(self, key):
        def wrapper(*args):
            return _methods[key](self._drawing, *args)
        return wrapper

    def clear(self, r, g, b, a=255):
        _methods["clear"](self._drawing, chr(r), chr(g), chr(b), chr(a))

    def set_color(self, r, g=None, b=None, a=255):
        if type(r) == str:
            r, g, b, a = _colors.get(r.lower().replace(' ', ''), [0, 0, 0, 255])
        elif g is None or b is None:
            g = b = r
        _methods["set_color"](self._drawing, chr(r), chr(g), chr(b), chr(a))

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

    def add_geometry(self, g, move_to=False, step=1):
        if not _HAS_SYMPY:
            raise Exception("Not implemented, sympy not installed")

        if isinstance(g, geo.Ellipse):
            self.ellipse(g.center.x, g.center.y, g.hradius, g.vradius)
        elif hasattr(g, 'vertices'):
            try:
                if move_to: self.move_to(g.vertices[0].x, g.vertices[0].y)
                else: self.line_to(g.vertices[0].x, g.vertices[0].y)
                for v in g.vertices[1:]:
                    self.line_to(v.x, v.y)
            except AttributeError:
                if move_to: self.move_to(g.vertices[0][0], g.vertices[0][1])
                else: self.line_to(g.vertices[0][0], g.vertices[0][1])
                for v in g.vertices[1:]:
                    self.line_to(v[0], v[1])
        elif hasattr(g, 'points'):
            if move_to: self.move_to(g.points[0].x, g.points[0].y)
            else: self.line_to(g.points[0].x, g.points[0].y)
            for p in g.points[1:]:
                self.line_to(p.x, p.y)
        elif hasattr(g, 'x') and hasattr(g, 'y'):
            if move_to: self.move_to(g.x, g.y)
            else: self.line_to(g.x, g.y)
        elif hasattr(g, 'p1') and hasattr(g, 'p2'):
            if move_to: self.move_to(g.p1.x, g.py.y)
            else: self.line_to(g.p1.x, g.p1.y)
            self.line_to(g.p2.x, g.p2.y)
        elif hasattr(g, 'functions'):
            has_tried_move = False
            func_x = lambdify(g.limits[0], g.functions[0])
            func_y = lambdify(g.limits[0], g.functions[1])
            for p in arange(g.limits[1], g.limits[2]+1, step):
                if not has_tried_move:
                    has_tried_move = True
                    if move_to:
                        self.move_to(func_x(p), func_y(p))
                        continue
                self.line_to(func_x(p), func_y(p))


def draw(arr, *args, **kwargs):
    return Drawing(arr, *args, **kwargs)

def new_image(width, height, channels=3, depth='uint8'):
    return zeros((height, width, channels), depth)


