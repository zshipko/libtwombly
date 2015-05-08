all:
	python gen.py 1> test-spec.h
	$(CXX) -DUSE_OPENCV -std=c++11 -o run test-template.cpp -L .. -ltwombly -I ../agg/include -I../agg/font_freetype -I../font_win32_tt -I/usr/local/include `pkg-config --cflags --libs freetype2`
