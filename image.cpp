#include "image.hpp"
#include <iostream>

int main(int argc, char **argv){
    auto im = Image<float>(100, 100, 3);
    Image<uint8_t> b(100, 100, 3);

    for( auto &g : im){
        g = 100.0/3.9;
    }

    im.convertTo(b, [](uint8_t m){ return m << 2; });
    std::cout << (int)b[10] << std::endl;

    auto c = b.component(1);
    b.component(2, c);
}
