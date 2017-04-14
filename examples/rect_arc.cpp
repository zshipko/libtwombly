#include <bimage.h>
#include "../twombly.hpp"

using namespace tw;

int main(int argc, char *argv[]){

    bimage *im = bimageCreate(400, 400, RGBA32);
    auto ctx = draw(400, 400, (uint8_t*)im->data);

    ctx.rect(10, 20, 210, 120);
    ctx.fill(Color(0, 0, 255));

    ctx.new_path();
    ctx.line_width(10);
    ctx.move_to(50, 50);
    ctx.arc_to(10, 10, 70, 50, 0.3);
    ctx.stroke(Color(200, 0, 255));
    ctx.paint();

    bimageSave(im, "rect_arc.tiff");
    bimageDestroy(&im);
    return 0;
}
