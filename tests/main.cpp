#include "lest.hpp"

#include "../twombly.hpp"

#include <iostream>

using namespace tw;

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
        d.newPath();
        d.setColor(255, 255, 255);
        d.moveTo(10, 10);
        d.lineTo(490, 490);

        int pathid = d.activePath();

        EXPECT (d.totalVertices() == 2);

        double x, y;
        d.getVertex(1, &x, &y);
        EXPECT(x == 490);
        EXPECT(y == 490);

        d.modifyVertex(1, 248, 249);
        d.getVertex(1, &x, &y);
        EXPECT(x == 248);
        EXPECT(y == 249);

        d.newPath();
        EXPECT (d.activePath() != pathid);
        EXPECT (d.activePath() > pathid);

        d.newPath();
        d.lineWidth(2);
        d.ellipse(250, 250, 100, 100);
        d.stroke();

        d.activePath(pathid);
        d.stroke();

        namedWindow("Path test");
        imshow("Path test", im);
        waitKey(0);
        destroyAllWindows();
    },

    CASE ( "Curve" ) {
        Mat3b im2(500, 500);
        im2.setTo(Scalar(255, 255, 255));
        auto d = draw(im2);

        d.setColor(255, 0, 0);
        d.moveTo(100, 100);
        d.curveTo(400, 400, 100, 320);
        d.stroke();

        namedWindow("Curve test");
        imshow("Curve test", im2);
        waitKey(0);

        d.newPath();
        d.clear(255, 255, 255);
        d.setColor(0, 255, 0);
        d.moveTo(100, 100);
        d.curveTo(400, 400, 150, 320);
        d.stroke();
        imshow("Curve test", im2);
        waitKey(0);
        destroyAllWindows();
    }

};

int main(int argc, char **argv){
    lest::run(drawing_test, argc, argv);
}
