#include "../twombly.hpp"

using namespace tw;

#define SIZE 800

int main(int argc, char *argv[]){
    Image<uint8_t> im(SIZE, SIZE, 4);
    auto d = DrawingRGBA32(im);

    d.antialias(false);

    map(range(0, 255, 20), [&d](int i){
        d.newPath();
        d.ellipse(SIZE/2, SIZE/2, (SIZE/2)-10 - i, (SIZE/2)-10 - i);
        d.setColor(i, 0, 255 - i / 2);
        d.fill();
    });

    imwrite(im, "circles.tiff");
    return 0;
}
