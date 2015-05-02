CC?=clang
CXX?=clang++
agg_src=\
agg/src/agg_arc.cpp \
agg/src/agg_arrowhead.cpp \
agg/src/agg_bezier_arc.cpp \
agg/src/agg_bspline.cpp \
agg/src/agg_curves.cpp \
agg/src/agg_vcgen_contour.cpp \
agg/src/agg_vcgen_dash.cpp \
agg/src/agg_vcgen_markers_term.cpp \
agg/src/agg_vcgen_smooth_poly1.cpp \
agg/src/agg_vcgen_stroke.cpp \
agg/src/agg_vcgen_bspline.cpp \
agg/src/agg_gsv_text.cpp \
agg/src/agg_image_filters.cpp \
agg/src/agg_line_aa_basics.cpp \
agg/src/agg_line_profile_aa.cpp \
agg/src/agg_rounded_rect.cpp \
agg/src/agg_sqrt_tables.cpp \
agg/src/agg_embedded_raster_fonts.cpp \
agg/src/agg_trans_affine.cpp \
agg/src/agg_trans_warp_magnifier.cpp \
agg/src/agg_trans_single_path.cpp \
agg/src/agg_trans_double_path.cpp \
agg/src/agg_vpgen_clip_polygon.cpp \
agg/src/agg_vpgen_clip_polyline.cpp \
agg/src/agg_vpgen_segmentator.cpp
tw_src=io.cpp tiff.cpp
agg_hdrs=agg/include/*.h agg/include/util/*.h
tw_hdrs=image.hpp draw.hpp stb_image.h stb_image_write.h twombly.hpp
libs=
incl=-I./agg/include -I./agg/font_freetype -I./twombly
dest?=/usr/local
VERSION=0.1
RELEASE_DIR=./libtwombly-$(VERSION)-`uname`_`uname -m`
UNAME=$(shell uname)
TIFF?=yes
FREETYPE?=yes
FREETYPE_PKG?=freetype2
SDL?=yes
SDL_PKG?=sdl2

HAS_FREETYPE=$(shell pkg-config --cflags $(FREETYPE_PKG) > /dev/null; echo $$?)
ifeq ($(HAS_FREETYPE)X$(FREETYPE)X,0XyesX)
	agg_src+= ./agg/font_freetype/agg_font_freetype.cpp
	agg_hdrs+= ./agg/font_freetype/agg_font_freetype.h
	libs+= `pkg-config --libs $(FREETYPE_PKG)`
	incl+= `pkg-config --cflags $(FREETYPE_PKG)`
else
	FREETYPE_PKG=
	incl+= -DNO_FREETYPE
endif

HAS_SDL=$(shell pkg-config --cflags sdl2 > /dev/null; echo $$?)
ifeq ($(HAS_SDL)X$(SDL)X,0XyesX)
	tw_src+=twombly/window.cpp
	tw_hdrs+= twombly/window.h
	libs+= `pkg-config --libs $(SDL_PKG)`
	incl+= `pkg-config --cflags $(SDL_PKG)`
else
	SDL_PKG=
endif

ifeq ($(TIFF)X,yesX)
	#tw_src+= twombly/tiff.cpp
	libs+= -L/usr/local/lib -ltiff
	incl+= -I/usr/local/include
else
	incl+= -DNO_TIFF
endif

agg_obj=$(agg_src:.cpp=.o)
tw_obj=$(tw_src:.cpp=.o)

$(shell git submodule init && git submodule update)

all: agg-static agg-shared tw-static tw-shared

compile: $(agg_obj) $(tw_obj)

agg-static: compile
	ar rcs libagg.a $(agg_obj)

agg-shared: compile
	$(CXX) -shared -o libagg.so $(agg_obj) $(libs)

tw-static: compile
	ar rcs libtwombly.a $(agg_obj) $(tw_obj)

tw-shared: compile
	$(CXX) -shared -o libtwombly.so $(agg_obj) $(tw_obj) $(libs)

.PHONY: install
install:
	mkdir -p $(dest)/include/twombly/agg $(dest)/include/twombly/extra $(dest)/lib/pkgconfig $(dest)/bin
	cp -r $(tw_hdrs) $(agg_hdrs) $(dest)/include/twombly
	cp twombly/extra/*.hpp twombly/extra/all $(dest)/include/twombly/extra
	cp libagg.* $(dest)/lib/
	cp libtwombly.* $(dest)/lib/
	cp twrun $(dest)/bin/twrun
	printf "Name: twombly\nDescription: A 2d graphics library for C++\nVersion: $(VERSION)\nLibs: -L$(dest)/lib -ltwombly\nCflags: -std=c++11 -I$(dest)/include\n" > $(dest)/lib/pkgconfig/twombly.pc
	echo "Requires: $(FREETYPE_PKG) $(SDL_PKG)" >> $(dest)/lib/pkgconfig/twombly.pc

uninstall:
	rm -rf $(dest)/include/twombly/
	rm -f $(dest)/lib/twombly.*
	rm -f $(dest)/lib/libagg.*
	rm -f $(dest)/bin/twrun
	rm -f $(dest)/lib/pkgconfig/twombly.pc

clean:
	rm -f $(agg_obj) $(tw_obj)

clean-libs:
	rm -f libtwombly.so libtwombly.a libagg.so libagg.a

%.o: %.cpp
	$(CXX) -O3 -std=c++11 -c -fPIC $*.cpp $(incl) -o $@

%.o: %.c
	$(CC) -O3 -c -fPIC $*.c $(incl) -o $@

release:
	rm -rf $(RELEASE_DIR)
	mkdir $(RELEASE_DIR)
	cp readme.md $(RELEASE_DIR)
	printf "#!/bin/sh\ncp lib/lib* /usr/local/lib && cp -r include/twombly /usr/local/include/\n" > $(RELEASE_DIR)/install.sh
	printf "#!/bin/sh\nrm /usr/local/lib/libtwombly.* && rm -r /usr/local/include/twombly\n" > $(RELEASE_DIR)/uninstall.sh
	dest=$(RELEASE_DIR) make install
	tar czf $(RELEASE_DIR).tar.gz $(RELEASE_DIR)

test:
	cd tests && $(MAKE)
	tests/run -p

parser:
	git submodule init && git submodule update
	cd ccparser && sudo python setup.py install
