#include "../twombly.hpp"

using namespace tw;

int main(int argc, char *argv[]){
    Mat3b im(400, 400);

    auto ctx = draw(im);

    ctx.rect(10, 20, 210, 120);
    ctx.fill(Color(0, 0, 255));

    ctx.new_path();
    ctx.line_width(10);
    ctx.move_to(50, 50);
    ctx.arc_to(10, 10, 70, 50, 0.3);
    ctx.stroke(Color(200, 0, 255));
    ctx.paint();

    imwrite("rect_arc.tiff", im);
    return 0;
}
