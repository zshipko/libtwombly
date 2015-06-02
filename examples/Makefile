files=*.cpp

basic-run:
	cc -o basic basic.c -I/usr/local/include -L/usr/local/lib -l twombly -l bimage
	./basic
%-run:
	rm -f $*
	$(CXX) -std=c++11 -I/usr/local/include -I../agg/include -I../agg/font_freetype `pkg-config --cflags --libs freetype2` -o $* $*.cpp ../libtwombly.a -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc
	./$*
