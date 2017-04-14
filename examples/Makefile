files=*.cpp

all: basic-run gradient-run rect_arc-run circles-run

basic-run:
	$(CC) -I/usr/local/include -o basic basic.c  -L/usr/local/lib -l twombly -l bimage `pkg-config --cflags --libs freetype2`
	./basic

%-run:
	$(CXX) -std=c++11 -I/usr/local/include -I../agg/include -I../agg/font_freetype `pkg-config --cflags --libs freetype2` -o $* $*.cpp ../libtwombly.a -L/usr/local/lib -lbimage
	./$*
