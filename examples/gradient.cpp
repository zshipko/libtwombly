#include "../twombly.hpp"

using namespace tw;

#define SIZE 800

int main(int argc, char *argv[]){
    Mat4b im(SIZE, SIZE);
    auto d = draw(im);

    Gradient<Color> g;
    g.add_stop(Color(255, 0, 0));
    g.add_stop(Color(255, 255, 255));
    g.add_stop(Color(0, 255, 0));
    g.add_stop(Color(255, 255, 255));
    g.add_stop(Color(0, 0, 255));

    d.rect(0, 0, SIZE, SIZE);
    d.fill_gradient(g, 0, 800, gradient_type_x);

    imwrite("gradient.tiff", im);
}
