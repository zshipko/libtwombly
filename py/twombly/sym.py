from drawing import TransformMatrix, _transform_matrix_get
from numpy import arange
import sympy as sym
from sympy import geometry as geo
from sympy.utilities import lambdify

def add_geometry(drawing, g, move_to=False, step=1):
    ''' draw sympy equations '''
    if isinstance(g, geo.Ellipse):
        drawing.ellipse(g.center.x, g.center.y, g.hradius, g.vradius)
    elif hasattr(g, 'vertices'):
        try:
            if move_to:
                drawing.move_to(g.vertices[0].x, g.vertices[0].y)
            else:
                drawing.line_to(g.vertices[0].x, g.vertices[0].y)
            for vertex in g.vertices[1:]:
                drawing.line_to(vertex.x, vertex.y)
        except AttributeError:
            if move_to:
                drawing.move_to(g.vertices[0][0], g.vertices[0][1])
            else:
                drawing.line_to(g.vertices[0][0], g.vertices[0][1])
            for vertex in g.vertices[1:]:
                drawing.line_to(vertex[0], vertex[1])
    elif hasattr(g, 'points'):
        if move_to:
            drawing.move_to(g.points[0].x, g.points[0].y)
        else:
            drawing.line_to(g.points[0].x, g.points[0].y)
        for point in g.points[1:]:
            drawing.line_to(point.x, point.y)
    elif hasattr(g, 'x') and hasattr(g, 'y'):
        if move_to:
            drawing.move_to(g.x, g.y)
        else:
            drawing.line_to(g.x, g.y)
    elif hasattr(g, 'p1') and hasattr(g, 'p2'):
        if move_to:
            drawing.move_to(g.p1.x, g.py.y)
        else:
            drawing.line_to(g.p1.x, g.p1.y)
    elif hasattr(g, 'functions'):
        has_tried_move = not move_to
        for x_pos in arange(g.limits[1], g.limits[2]+1, step):
            if not has_tried_move:
                has_tried_move = True
                if move_to:
                    drawing.move_to(g.functions[0].subs(g.limits[0], x_pos),
                                 g.functions[1].subs(g.limits[0], x_pos))
                    continue
            drawing.line_to(g.functions[0].subs(g.limits[0], x_pos),
                         g.functions[1].subs(g.limits[0], x_pos))

