show?=yes

ifeq ($(show),no)
flags=-DNO_SHOW
endif
all:
	$(CXX) -std=c++11 -o run main.cpp -L .. -l twombly -I .. -I../agg/include -I../agg/font_freetype -I /usr/local/include -L/usr/local/lib `pkg-config --cflags --libs freetype2` -lopencv_core -lopencv_highgui -lopencv_imgproc $(flags)
