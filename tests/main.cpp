#include "lest.hpp"

#include "../twombly.hpp"

#include <iostream>

using namespace tw;

#define RED 2
#define GREEN 1
#define BLUE 0
#define ALPHA 3

int opencv_error_handler(int status, const char* func_name, const char* err_msg,
                   const char* file_name, int line, void*)
{
    return 0;
}

const lest::test drawing_test[] = {
    CASE ( "Mat3b drawing" ) {
        Mat3b im(100, 100);
        auto d = draw(im);
        d.clear(255, 0, 0);
        EXPECT(im.at<Vec3b>(10, 10)[RED] == 255);
        EXPECT(im.at<Vec3b>(10, 10)[GREEN] == 0);
        EXPECT(im.at<Vec3b>(10, 10)[BLUE] == 0);
    },

    CASE ( "Mat4b drawing" ) {
        Mat4b im(100, 100);
        auto d = draw(im);
        d.clear(255, 0, 0);
        EXPECT(im.at<Vec4b>(10, 10)[RED] == 255);
        EXPECT(im.at<Vec4b>(10, 10)[GREEN] == 0);
        EXPECT(im.at<Vec4b>(10, 10)[BLUE] == 0);
        EXPECT(im.at<Vec4b>(10, 10)[ALPHA] == 255);
    },

    CASE ( "Mat3w drawing" ) {
        Mat3w im(100, 100);
        auto d = draw(im);
        d.clear(255, 0, 0);
        EXPECT(im.at<Vec3w>(10, 10)[RED] == 65535);
        EXPECT(im.at<Vec3w>(10, 10)[GREEN] == 0);
        EXPECT(im.at<Vec3w>(10, 10)[BLUE] == 0);
    },

    CASE ( "Mat4w drawing" ) {
        Mat4w im(100, 100);
        auto d = draw(im);
        d.clear(255, 0, 0);
        EXPECT(im.at<Vec4w>(10, 10)[RED] == 65535);
        EXPECT(im.at<Vec4w>(10, 10)[GREEN] == 0);
        EXPECT(im.at<Vec4w>(10, 10)[BLUE] == 0);
        EXPECT(im.at<Vec4w>(10, 10)[ALPHA] == 65535);
    },

    CASE ( "Path" ) {
        Mat3b im(500, 500);
        auto d = draw(im);
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

#ifndef NO_SHOW
        try{
            namedWindow("Path test");
            imshow("Path test", im);
            waitKey(0);
            destroyAllWindows();
        } catch(std::exception &exc){

        }
#endif
    },

    CASE ( "Curve" ) {
        Mat3b im2(500, 500);
        im2.setTo(Scalar(255, 255, 255));
        auto d = draw(im2);

        d.set_color(255, 0, 0);
        d.move_to(100, 100);
        d.curve_to(400, 400, 100, 320);
        d.stroke();

#ifndef NO_SHOW
        try {
            namedWindow("Curve test");
            imshow("Curve test", im2);
            waitKey(0);
        } catch (std::exception &exc){

        }
#endif

        d.new_path();
        d.clear(255, 255, 255);
        d.set_color(0, 255, 0);
        d.move_to(100, 100);
        d.curve_to(400, 400, 150, 320);
        d.stroke();

#ifndef NO_SHOW
        try{
            imshow("Curve test", im2);
            waitKey(0);
            destroyAllWindows();
        } catch(std::exception &exc){

        }
#endif
    },

    CASE ( "Gradient" ) {
        Mat3w im2(500, 500);
        im2.setTo(Scalar(255, 255, 255));
        auto d = draw(im2);

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
        EXPECT((im2.at<Scalar>(250, 250)[0] > 0));

#ifndef NO_SHOW
        try{
            imshow("Gradient test", im2);
            waitKey(0);
            destroyAllWindows();
        } catch(std::exception &exc){

        }
#endif
    },

    CASE( "cairo-arc-infinite-loop"){
        Mat3b im2(8, 8);
        auto d = draw(im2);
        d.clear(255, 255, 255);

        d.move_to(0, 0);
        d.arc_to(0, 0, 1, 1024 / DBL_EPSILON * M_PI, 0);
        d.arc_to(0, 0, 1, 0, 1024 / DBL_EPSILON * M_PI);

        d.set_color(255, 0, 0);
        d.stroke();

#ifndef NO_SHOW
        try{
            imshow("infinte loop", im2);
            waitKey(0);
            destroyAllWindows();
        } catch(std::exception &exc){

        }
#endif
    }
};

int main(int argc, char **argv){
    cv::redirectError(opencv_error_handler);
    lest::run(drawing_test, argc, argv);
}
