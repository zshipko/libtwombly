files=*.cpp

basic-run:
	cc -I/usr/local/include -o basic basic.c  -L/usr/local/lib -l twombly -l bimage `pkg-config --cflags --libs freetype2`
	./basic
%-run:
	rm -f $*
	$(CXX) -std=c++11 -I/usr/local/include -I../agg/include -I../agg/font_freetype `pkg-config --cflags --libs freetype2` -o $* $*.cpp ../libtwombly.a -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
	./$*
