from twombly import *
from skimage.io import imsave

SIZE=800
im = new_image(SIZE, SIZE)
d = draw(im)
#d.set_antialias(False)

for i in xrange(0, 255, 20):
    d.new_path()
    d.ellipse(SIZE/2, SIZE/2, SIZE/2-10 - i, SIZE/2-10 -i)
    d.set_color(i, 0, 255 - i / 2)
    d.fill()

imsave("circles.png", im)

