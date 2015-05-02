#include "../twombly.hpp"

#include "lest.hpp"

using namespace std;
using namespace tw;

Image<uint8_t> test_Image(800, 600, 3);
Image<uint16_t> test_Imagew(800, 600, 4);
Image<float> test_Imagef(800, 600, 1);
maybe<int> test_maybe(12);

auto test_Drawing = DrawingRGB24(test_Image);
std::tuple<Image<uint8_t>, Point> test_layer;
Point test_Point;
Rectangle test_Rectangle(0,0, 10, 10);
extra::list<int> test_list;
extra::array<int, 20> test_array;
Layers<Image<uint8_t>> test_Layers;
Image<uint8_t> test_Image2(800, 600, 3);
auto test_Drawing2 = DrawingRGB24(test_Image);
double double_ptr;

#include "test-spec.h"

int main( int argc, char * argv[] )
{
    return lest::run( test_spec, argc, argv );
}
