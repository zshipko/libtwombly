#include <bimage.h>
#include "../twombly.hpp"

using namespace tw;

#define SIZE 800

int main(int argc, char *argv[]){
    bimage *im = bimageCreate(SIZE, SIZE, BIMAGE_U8 | 4);
    auto d = draw(SIZE, SIZE, (uint8_t*)im->data);

    Gradient<Color> g;
    g.add_stop(Color(255, 0, 0));
    g.add_stop(Color(255, 255, 255));
    g.add_stop(Color(0, 255, 0));
    g.add_stop(Color(255, 255, 255));
    g.add_stop(Color(0, 0, 255));

    d.rect(0, 0, SIZE, SIZE);
    d.fill_gradient(g, 0, 800, gradient_type_x);

    bimageSave(im, "gradient.tiff");
    bimageDestroy(&im);
    return 0;
}
