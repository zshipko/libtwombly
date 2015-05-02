#include "../twombly/twombly.h"

#include "lest.hpp"

using namespace std;
using namespace tw;

image4u test_image(800, 600);
image3u test_image3u(800, 600);
image1u test_image1u(800, 600);
image4s test_image4s(800, 600);
image3s test_image3s(800, 600);
image1s test_image1s(800, 600);
image4f test_image4f(800, 600);
image3f test_image3f(800, 600);
image1f test_image1f(800, 600);

auto test_drawing = draw(test_image);
auto test_pixel = pixel4u(100, 100, 100, 100);
std::tuple<image4u, point> test_layer;
tw::point test_point;
extra::list<int> test_list;
extra::array<int, 20> test_array;
layers<image4u> test_layers;
image4u test_image2(800, 600);
auto test_drawing2 = draw(test_image2);
auto test_pixel2 = pixel4u(90, 90, 90, 100);
double double_ptr;

#include "test-spec.h"

int main( int argc, char * argv[] )
{
    return lest::run( test_spec, argc, argv );
}
