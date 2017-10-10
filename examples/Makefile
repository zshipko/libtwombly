UNAME:=$(shell uname)

files=*.cpp

all: basic-run gradient-run rect_arc-run circles-run

basic-run:
	$(CC) -c -o basic.o basic.c -I/usr/local/include
	$(CXX) -o basic basic.o -L/usr/local/lib  -lbimage `pkg-config --cflags --libs freetype2` -L .. -ltwombly -lm
	./basic

%-run:
	$(CXX) -std=c++11 -I/usr/local/include -I../agg/include -I../agg/font_freetype `pkg-config --cflags --libs freetype2` -o $* $*.cpp ../libtwombly.a -L/usr/local/lib -lbimage
	./$*
