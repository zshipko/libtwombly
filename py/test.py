import unittest
from twombly import *
from skimage.io import imsave

class TestDrawing(unittest.TestCase):
    def setUp(self):
        self.image = new_image(1200, 800)
        self.drawing = draw(self.image)

    def test_new_drawing(self):
        self.assertIsNotNone(self.image)
        self.assertEquals(self.image.shape[0], 800)
        self.assertEquals(self.image.shape[1], 1200)
        self.assertEquals(self.image.shape[2], 3)
        self.assertIsInstance(self.drawing, Drawing)

    def test_draw_line(self):
        self.drawing.set_antialias(False)
        self.drawing.set_color(255, 0, 255)
        self.drawing.move_to(10, 10)
        self.drawing.line_to(120, 80)
        self.drawing.set_line_width(10)
        self.drawing.stroke()
        self.assertEquals(self.image[11, 11][0], 255)
        self.assertEquals(self.image[79, 119][0], 255)
        self.assertEquals(self.image[79, 119][0], 255)
        imsave("lines.png", self.image)

    def test_draw_rect(self):
        self.drawing.set_color(0, 255, 0)
        self.drawing.rect(150, 120, 180, 170)
        self.drawing.fill()
        self.assertEquals(self.image[150, 150][1], 255)
        self.assertEquals(self.image[150, 150][0], self.image[150, 150][2])

    def test_transform_matrix(self):
        m = self.drawing.matrix()
        arr = m.array()
        arr *= 0
        arr += 10
        m.array(arr)
        self.assertEquals(self.drawing.matrix().array()[0], 10)

    def test_gradient(self):
        g = Gradient()
        g.add_stop(Color(255, 0, 0, 255))
        g.add_stop(Color(0, 255, 0, 255))
        g.add_stop(Color(0, 0, 255, 255))
        self.drawing.rect(10, 10, 20, 20)
        self.drawing.set_line_width(10)
        self.drawing.fill_gradient(g, 0, 800, GRADIENT_Y)
        self.assertEqual(self.image[11, 11][0], 248, 10)


if __name__ == '__main__':
    unittest.main()
