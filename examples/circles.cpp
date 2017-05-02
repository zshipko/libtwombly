#include <bimage.h>
#include "../twombly.hpp"

using namespace tw;

#define SIZE 800

int main(int argc, char *argv[]){
    bimage *im = bimageCreate(SIZE, SIZE, BIMAGE_U16 | 4);
    auto d = draw(SIZE, SIZE, (uint16_t*)im->data);

    d.antialias(false);

    for (size_t i = 0; i < 255; i += 20){
        d.ellipse(SIZE/2, SIZE/2, (SIZE/2)-10 - i, (SIZE/2)-10 - i);
        d.fill(Color(i, 0, 255 - i / 2));
        d.paint();
    }

    bimageSave(im, "circles.tiff");
    bimageDestroy(&im);
    return 0;
}
