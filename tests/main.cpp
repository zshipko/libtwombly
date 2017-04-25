#include <bimage.h>
#include "lest.hpp"

#include "../twombly.hpp"
#define NO_SHOW

#include <iostream>

using namespace tw;

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

const lest::test drawing_test[] = {
    CASE ( "BIMAGE_RGB24 drawing" ) {
        bimage *im = bimageCreate(100, 100, BIMAGE_RGB24);
        auto d = Drawing<rgb24>(100, 100, 3, (uint8_t*)im->data);
        d.clear(255, 0, 0);

        bpixel px;
        bimageGetPixel(im, 10, 10, &px);

        EXPECT(px.data[RED] == 255);
        EXPECT(px.data[GREEN] == 0);
        EXPECT(px.data[BLUE] == 0);
        bimageDestroy(&im);
    },

    CASE ( "BIMAGE_RGBA32 drawing" ) {
        bimage *im = bimageCreate(100, 100, BIMAGE_RGBA32);
        auto d = Drawing<rgba32>(100, 100, 4, (uint8_t*)im->data);
        d.clear(255, 0, 0);

        bpixel px;
        bimageGetPixel(im, 10, 10, &px);

        EXPECT(px.data[RED] == 255);
        EXPECT(px.data[GREEN] == 0);
        EXPECT(px.data[BLUE] == 0);
        EXPECT(px.data[ALPHA] == 255);
        bimageDestroy(&im);
    },

    CASE ( "BIMAGE_RGB48 drawing" ) {
        bimage *im = bimageCreate(100, 100, BIMAGE_RGB48);
        auto d = Drawing<rgb48>(100, 100, 3, (uint16_t*)im->data);
        d.clear(255, 0, 0);

        bpixel px;
        bimageGetPixel(im, 10, 10, &px);

        EXPECT(px.data[RED] == 65535);
        EXPECT(px.data[GREEN] == 0);
        EXPECT(px.data[BLUE] == 0);
        bimageDestroy(&im);
    },

    CASE ( "BIMAGE_RGBA64 drawing" ) {
        bimage *im = bimageCreate(100, 100, BIMAGE_RGBA64);
        auto d = Drawing<rgba64>(100, 100, 4, (uint16_t*)im->data);
        d.clear(255, 0, 0);

        bpixel px;
        bimageGetPixel(im, 10, 10, &px);

        EXPECT(px.data[RED] == 65535);
        EXPECT(px.data[GREEN] == 0);
        EXPECT(px.data[BLUE] == 0);
        EXPECT(px.data[ALPHA] == 65535);
        bimageDestroy(&im);
    },

    CASE ( "Path" ) {
        bimage *im = bimageCreate(500, 500, BIMAGE_RGB24);
        auto d = Drawing<rgb24>(500, 500, 3, (uint8_t*)im->data);
        d.preserve(true);
        d.new_path();
        d.set_color(255, 255, 255);
        d.move_to(10, 10);
        d.line_to(490, 490);
        d.stroke();

        int pathid = d.active_path();

        EXPECT (d.total_vertices() == 2);

        double x, y;
        d.vertex(1, &x, &y);
        EXPECT(x == 490);
        EXPECT(y == 490);

        d.modify_vertex(1, 248, 249);
        d.vertex(1, &x, &y);
        EXPECT(x == 248);
        EXPECT(y == 249);

        d.new_path();
        EXPECT (d.active_path() != pathid);
        EXPECT (d.active_path() > pathid);

        d.new_path();
        d.line_width(2);
        d.ellipse(250, 250, 100, 100);
        d.stroke();

        d.active_path(pathid);
        d.stroke();
        d.preserve(false);

        bimageRelease(im);
    },

    CASE ( "Gradient" ) {
        bimage *im2 = bimageCreate(500, 500, BIMAGE_RGB48);
        memset(im2->data, 65535, im2->width * im2->height * 16 * 3);
        auto d = Drawing<rgb48>(500, 500, 3, (uint16_t*)im2->data);

        d.set_color(255, 0, 0);
        d.move_to(100, 100);
        d.curve_to(400, 400, 100, 320);
        d.stroke();

        d.new_path();
        d.ellipse(250, 250, 250, 250);

        Gradient<Color16> g;
        g.add_stop(Color16(255<<8, 0, 0, 127<<8));
        g.add_stop(Color16(0, 255<<8, 0, 127<<8));
        g.add_stop(Color16(0, 0, 255<<8, 127<<8));

        d.fill_gradient(g, 0, 300, gradient_type_y);

        bpixel px;
        bimageGetPixel(im2, 250, 250, &px);
        EXPECT(px.data[0] > 0);
        bimageRelease(im2);
    },

    CASE( "cairo-arc-infinite-loop"){
        bimage *im = bimageCreate(8, 8, BIMAGE_RGB24);
        auto d = Drawing<rgb24>(8, 8, 3, (uint8_t*)im->data);
        d.clear(255, 255, 255);

        d.move_to(0, 0);
        d.arc_to(0, 0, 1, 1024.0 / std::numeric_limits<double>::epsilon() * M_PI, 0);
        d.arc_to(0, 0, 1, 0, 1024.0 / std::numeric_limits<double>::epsilon() * M_PI);

        d.set_color(255, 0, 0);
        d.stroke();

        bpixel px;
        bimageGetPixel(im, 0, 0, &px);

        EXPECT((px.data[0] == 255 && px.data[0] > px.data[1] && px.data[0] > px.data[2]));

        bimageRelease(im);
    }
};

int main(int argc, char **argv){
    lest::run(drawing_test, argc, argv);
}
