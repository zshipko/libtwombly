#include <bimage/bimage.h>
#include <twombly/draw_c.h>

int main(int argc, char **argv){
    bimage im = bimage_create(100, 100, 3, u8);
    drawing d = draw_create(im.width, im.height, im.channels, im.u8);
    draw_clear(d, 255, 0, 0, 255);
    draw_set_color(d, 0, 0, 255, 255);
    draw_move_to(d, 10, 10);
    draw_line_to(d, im.width - 10, im.height - 10);
    draw_stroke(d);
    bimage_save("basic.tiff", im);
    draw_free(&d);
}
