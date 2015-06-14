from twombly import *
from imageio import imwrite

im = new_image(100, 100)
d = draw(im)
d.clear(255, 0, 0, 255)
d.set_color(0, 0, 255)
d.move_to(10, 10)
d.line_to(im.shape[1] - 10, im.shape[0] - 10)
d.stroke()
imwrite("basic-py.tiff", im)

