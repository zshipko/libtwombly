from __future__ import absolute_import
from .drawing import draw, zeros
from skimage.measure import find_contours
from skimage.color import rgb2gray, gray2rgb
from skimage.feature import canny

def image_get_contours(im, level=0, canny_sigma=None, canny_low=None, canny_high=None, fully_connected='high'):
    '''find image contours using canny and find_contours from skimage'''
    tmp = rgb2gray(im)
    if canny_sigma is not None:
        tmp = canny(tmp, canny_sigma, canny_low, canny_high)
    return find_contours(tmp, level, fully_connected=fully_connected)

def image_to_svg(im, distance=20, canny_sigma=None, canny_low=None, canny_high=None, fully_connected='high', contours=None, level=0, line=True):
    '''finds image contours and converts them to a primitive SVG string'''
    if contours is None:
        contours = image_get_contours(im, level, canny_sigma, canny_low, canny_high, fully_connected)
    pathstr='''<path
         d="M {A},{B} {points}  "
         id="path3006"
         style="fill:#000000;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />'''

    paths = []
    for a in contours:
        tmp = []
        if line:
            a = a[:len(a)/2:distance]
        for b in a:
            tmp.append('{0},{1}'.format(b[1], b[0]))
        paths.append(pathstr.format(A=a[0][1], B=a[0][0], points=' '.join(tmp)))

    return '''<?xml version="1.0" encoding="UTF-8" standalone="no"?>
    <svg
       xmlns:dc="http://purl.org/dc/elements/1.1/"
       xmlns:cc="http://creativecommons.org/ns#"
       xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"
       xmlns:svg="http://www.w3.org/2000/svg"
       xmlns="http://www.w3.org/2000/svg"
       version="1.1"
       width="{width}"
       height="{height}"
       id="svg2">
      <defs
         id="defs4" />
      <metadata
         id="metadata7">
        <rdf:RDF>
          <cc:Work
             rdf:about="">
            <dc:format>image/svg+xml</dc:format>
            <dc:type
               rdf:resource="http://purl.org/dc/dcmitype/StillImage" />
            <dc:title></dc:title>
          </cc:Work>
        </rdf:RDF>
      </metadata>
      <g
         id="layer1" />
      {paths}

    </svg>
    '''.format(width=im.shape[1], height=im.shape[0], \
                        paths='\n'.join(paths))

def image_to_drawing(im, distance=20, canny_sigma=None, canny_low=None, canny_high=None, fully_connected='high', contours=None, level=0, line=True):
    '''find image contours and draws them using a twombly Drawing'''
    if contours is None:
        contours = image_get_contours(im, level, canny_sigma, canny_low, canny_high, fully_connected)

    tmp = zeros(im.shape, dtype=im.dtype)
    ctx = draw(tmp)
    for a in contours:
        ctx.move_to(a[0][1], a[0][0])
        if line:
            a = a[:len(a)/2:distance]
        for b in a:
            ctx.line_to(b[1], b[0])
    return tmp, ctx
