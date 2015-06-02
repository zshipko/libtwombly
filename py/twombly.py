from ctypes import *

twombly = cdll.LoadLibrary("libtwombly.so")

class DrawingType(Structure):
    _fields_ = [
            ("handle", c_void_p),
            ("channels", c_int),
            ("bits_per_channel", c_int)
    ]

def fn(c_fn, res=None, args=[DrawingType]):
    tmp = c_fn
    tmp.argtypes = args
    tmp.restype = res
    return tmp

_methods = dict(
    create = fn(twombly.draw_create,
        DrawingType, [c_long, c_long, c_long, c_void_p]),
    create16 = fn(twombly.draw_create16,
        DrawingType, [c_long, c_long, c_long, c_void_p]),
    free = fn(twombly.draw_free),
    get_antialias = fn(twombly.draw_getAntialias, c_bool),
    set_antialias = fn(twombly.draw_getAntialias,
        args=[DrawingType, c_bool]),
    set_line_width = fn(twombly.draw_setLineWidth,
        args=[DrawingType, c_double]),
    get_line_width = fn(twombly.draw_getLineWidth, c_double),
    set_miter_limit = fn(twombly.draw_setMiterLimit,
        args=[DrawingType, c_double]),
    get_miter_limit = fn(twombly.draw_getMiterLimit, c_double),
    set_line_join = fn(twombly.draw_setLineJoin,
        args=[DrawingType, c_int]),
    get_line_join = fn(twombly.draw_getLineJoin, c_int),
    set_line_cap = fn(twombly.draw_setLineCap,
        args=[DrawingType, c_int]),
    get_line_cap = fn(twombly.draw_getLineCap, c_int),
    set_active_path = fn(twombly.draw_setActivePath,
        args=[DrawingType, c_uint]),
    get_active_path = fn(twombly.draw_getActivePath, c_uint),
    new_path = fn(twombly.draw_newPath, c_uint),
    rotate = fn(twombly.draw_rotate, args=[DrawingType, c_double]),
    translate = fn(twombly.draw_translate,
        args=[DrawingType, c_double, c_double]),
    scale = fn(twombly.draw_scale,
        args=[DrawingType, c_double]),
    scale2 = fn(twombly.draw_scale2,
        args = [DrawingType, c_double, c_double]),
    skew = fn(twombly.draw_skew,
        args = [DrawingType, c_double, c_double]),
    clear_transforms = fn(twombly.draw_clearTransforms),
    close_polygon = fn(twombly.draw_closePolygon),
    end_polygon = fn(twombly.draw_endPolygon),
    invert_polygon = fn(twombly.draw_invertPolygon),
    reset = fn(twombly.draw_reset),
    clear = fn(twombly.draw_clear,
        args = [DrawingType, c_char, c_char, c_char, c_char]),
    remove_paths = fn(twombly.draw_removePaths),
    ellipse = fn(twombly.draw_ellipse,
        args = [DrawingType, c_double, c_double, c_double, c_double]),
    rect = fn(twombly.draw_rect,
        args= [DrawingType, c_double, c_double, c_double, c_double]),
    clip = fn(twombly.draw_clip,
        args= [DrawingType, c_double, c_double, c_double, c_double]),
    reset_clip = fn(twombly.draw_resetClip),
    last_x = fn(twombly.draw_lastX, c_double),
    last_y = fn(twombly.draw_lastY, c_double),
    rel_to_abs = fn(twombly.draw_relToAbs,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    move_to = fn(twombly.draw_moveTo,
            args = [DrawingType, c_double, c_double]),
    move_rel = fn(twombly.draw_moveRel,
            args = [DrawingType, c_double, c_double]),
    line_to = fn(twombly.draw_lineTo,
            args= [DrawingType, c_double, c_double]),
    line_rel = fn(twombly.draw_lineRel,
            args = [DrawingType, c_double, c_double]),
    curve_to2 = fn(twombly.draw_curveTo2,
            args= [DrawingType, c_double, c_double]),
    curve_rel2 = fn(twombly.draw_curveRel2,
            args= [DrawingType, c_double, c_double]),
    curve_to4 = fn(twombly.draw_curveTo4,
            args= [DrawingType, c_double, c_double, c_double, c_double]),
    curve_rel4 = fn(twombly.draw_curveRel4,
            args= [DrawingType, c_double, c_double, c_double, c_double]),
    curve_to6 = fn(twombly.draw_curveTo6,
            args= [DrawingType, c_double, c_double, c_double, c_double, c_double, c_double]),
    curve_rel6 = fn(twombly.draw_curveRel6,
            args= [DrawingType, c_double, c_double, c_double, c_double, c_double, c_double]),
    arc_to = fn(twombly.draw_arcTo,
            args= [DrawingType, c_double, c_double, c_double, c_double, c_double]),
    arc_rel = fn(twombly.draw_arcRel,
            args = [DrawingType, c_double, c_double, c_double, c_double, c_double]),
    put_text_simple = fn(twombly.draw_putTextSimple, c_double,
            args= [DrawingType, c_double, c_double, c_char_p, c_int, c_double, c_char_p]),
    put_text = fn(twombly.draw_putText, c_double,
            args= [DrawingType, c_double, c_double, c_char_p, c_char_p, c_double, c_double]),
    set_color = fn(twombly.draw_setColor,
            args = [DrawingType, c_char, c_char, c_char, c_char]),
    fill = fn(twombly.draw_fill),
    stroke = fn(twombly.draw_stroke),
    paint = fn(twombly.draw_paint),
    auto_close = fn(twombly.draw_autoClose,
            args = [c_bool]),
    in_path = fn(twombly.draw_inPath, c_bool,
            args=[c_double, c_double]),
    get_vertex = fn(twombly.draw_getVertex, c_uint,
            args= [DrawingType, c_uint, POINTER(c_double), POINTER(c_double)]),
    next_vertex = fn(twombly.draw_nextVertex, c_uint,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    get_command = fn(twombly.draw_getCommand, c_uint,
            args = [DrawingType, c_uint]),
    last_vertex = fn(twombly.draw_lastVertex, c_uint,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    prev_vertex = fn(twombly.draw_prevVertex, c_uint,
            args = [DrawingType, POINTER(c_double), POINTER(c_double)]),
    modify_vertex = fn(twombly.draw_modifyVertex,
            args = [DrawingType, c_uint, POINTER(c_double), POINTER(c_double), c_uint]),
    total_vertices = fn(twombly.draw_totalVertices, c_uint),
    join = fn(twombly.draw_join,
            args=[DrawingType, DrawingType]),
    concat = fn(twombly.draw_concat,
            args=[DrawingType, DrawingType]),
    fill_pattern = fn(twombly.draw_fillPattern,
            args = [DrawingType, c_long, c_long, c_int, c_char_p]),
    stroke_pattern = fn(twombly.draw_fillPattern,
            args = [DrawingType, c_long, c_long, c_int, c_char_p]),
)

class Drawing(object):
    def __init__(self, arr):
        self.array = arr
        if arr.dtype == 'uint8':
            self._drawing = _methods["create"](arr.shape[0], arr.shape[1], arr.shape[2], arr.ravel().ctypes.data)
        elif arr.dtype == 'uint16':
            self._drawing = _methods["create16"](arr.shape[0], arr.shape[1], arr.shape[2], arr.ravel().ctypes.data)
        else:
            raise ValueError("bad image type")

        self._as_parameter_ = self._drawing
        self._free = _methods["free"]

    def __getattr__(self, key):
        def wrapper(*args):
            return _methods[key](self._drawing, *args)
        return wrapper

    def clear(self, r, g, b, a=255):
        _methods["clear"](self._drawing, chr(r), chr(g), chr(b), chr(a))

    def set_color(self, r, g, b, a=255):
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
        self._free(self._drawing)

def draw(arr):
    return Drawing(arr)

