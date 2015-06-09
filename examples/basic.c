#include <bimage/bimage.h>
#include <twombly/draw_c.h>

int main(int argc, char **argv){
    bimage im = bimage_create(100, 100, 3, u8);
    drawing d = draw_create(im.width, im.height, im.channels, im.u8);
    draw_clear(d, 255, 0, 0, 255);
    draw_setColor(d, 0, 0, 255, 255);
    draw_moveTo(d, 10, 10);
    draw_lineTo(d, im.width - 10, im.height - 10);
    draw_stroke(d);
    draw_free(&d);
}
