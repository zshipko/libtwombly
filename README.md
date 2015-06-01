# libtwombly - 2d graphics library

[![Build Status](https://travis-ci.org/zshipko/libtwombly.svg?branch=master)](https://travis-ci.org/zshipko/libtwombly)

libtwombly is a basic 2d graphics library for C99 and C++11. Once the interface stabilizes it will provide a somewhat familiar interface for drawing on images using paths, transforms, strokes, fills, gradients, text, images,  etc..

It it based on a lightly modified version of the [Anti-Grain Geometry](http://www.antigrain.com/) library (version 2.4) and is meant to provide an API similar to cairo or HTML5 canvas.

Tested on OSX, Linux, FreeBSD and OpenBSD

## Dependencies

- C++11 compiler (g++/clang++)
- [opencv](https://www.opencv.org/) (optional)
- [bimage](https://github.com/zshipko/bimage) (optional)
- [freetype](http://www.freetype.org/) (optional)

## Building
GNU Make is required

For the default configuration just run, all dependencies(except libtiff) must be available via pkg-config:

	make
	make install

To disable FreeType:

    make freetype=no

To disable OpenCV:

    make opencv=no

Building will compile both `libtwombly` and `libagg` libraries.

## Usage

See the [examples/](https://github.com/zshipko/libtwombly/tree/master/examples) folder for now.

To build and run an example:

    cd examples
    make circles-run

