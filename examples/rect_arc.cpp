#include "../twombly.hpp"

using namespace tw;

int main(int argc, char *argv[]){
    Mat3b im(400, 400);

    auto ctx = draw(im);

    ctx.new_path();
    ctx.set_color(0, 0, 255);
    ctx.rect(10, 20, 210, 120);
    ctx.fill();

    ctx.new_path();
    ctx.set_color(200, 0, 255);
    ctx.line_width(10);
    ctx.move_to(50, 50);
    ctx.arc_to(70, 50, 10, 10, 0.3);
    ctx.stroke();

    imwrite("rect_arc.tiff", im);
    return 0;
}
