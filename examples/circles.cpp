#include "../twombly.hpp"

using namespace tw;

#define SIZE 800

int main(int argc, char *argv[]){
    Mat4b im(SIZE, SIZE);
    auto d = draw(im);

    d.antialias(false);

    for (size_t i = 0; i < 255; i += 20){
        d.newPath();
        d.ellipse(SIZE/2, SIZE/2, (SIZE/2)-10 - i, (SIZE/2)-10 - i);
        d.setColor(i, 0, 255 - i / 2);
        d.fill();
    }

    imwrite("circles.tiff", im);
    return 0;
}
