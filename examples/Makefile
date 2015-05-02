files=*.cpp

%-run:
	rm -f $*
	$(CXX) -std=c++11 -I/usr/local/include -I../agg/include -I../agg/font_freetype `pkg-config --cflags freetype2` -o $* $*.cpp ../libtwombly.a -L/usr/local/lib -ltiff
	./$*
