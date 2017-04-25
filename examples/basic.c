#include <bimage.h>
#include <twombly/draw.h>

int main(int argc, char **argv){
    bimage* im = bimageCreate(100, 100, BIMAGE_RGB24);
    drawing *d = draw_frombimage(im);
    draw_clear(d, 255, 0, 0, 255);
    draw_set_color(d, 0, 0, 255, 255);
    draw_move_to(d, 10, 10);
    draw_line_to(d, im->width - 10, im->height - 10);
    draw_stroke(d);
    bimageSave(im, "basic.tiff");
    draw_free(d);
    bimageDestroy(&im);
    return 0;
}
