all:
	$(CXX) -std=c++11 -o run main.cpp -L .. -l twombly -I .. -I../agg/include -I../agg/font_freetype `pkg-config --cflags --libs freetype2` -lopencv_core -lopencv_highgui -lopencv_imgproc
