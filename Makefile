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
tw_src=draw.cpp capi/capi.cpp capi/gradient.cpp capi/transform.cpp
agg_hdrs=agg/include/*.h agg/include/util/*.h
tw_hdrs=twombly.hpp capi/twombly.h config.h

CONFIG_FT=

suffix_Darwin=dylib
suffix?=$(suffix_$(shell uname))
ifeq ($(suffix)X,X)
	suffix=so
endif

incl=-I./agg/include -I./agg/font_freetype -I./twombly -I/usr/local/include $(flags)
dest?=/usr/local
VERSION=0.1
RELEASE_DIR=./libtwombly-$(VERSION)-`uname`_`uname -m`
UNAME=$(shell uname)
freetype?=yes
bimage?=no
FREETYPE_PKG?=freetype2
svg=no

HAS_COMPILER=$(shell $(CXX) -v || printf "NO";)
ifeq ($(HAS_COMPILER)X,NOX)
	CXX=c++
endif

HAS_FREETYPE=$(shell pkg-config --cflags $(FREETYPE_PKG) > /dev/null; echo $$?)
ifeq ($(HAS_FREETYPE)X$(freetype)X,0XyesX)
	agg_src+= ./agg/font_freetype/agg_font_freetype.cpp
	agg_hdrs+= ./agg/font_freetype/agg_font_freetype.h
	libs+= `pkg-config --libs $(FREETYPE_PKG)`
	incl+= `pkg-config --cflags $(FREETYPE_PKG)`
else
	FREETYPE_PKG=
	CONFIG_FT+= "\#define NO_FREETYPE"
endif

ifeq ($(svg)X,yesX)
	agg_src+= ./agg/examples/svg_viewer/agg_svg_path_renderer.cpp ./agg/examples/svg_viewer/agg_svg_path_tokenizer.cpp ./agg/examples/svg_viewer/agg_svg_parser.cpp
	agg_hdrs+= ./agg/examples/svg_viewer/*.h
	libs+= -lexpat
else
	incl+= -fno-exceptions
endif

agg_obj=$(agg_src:.cpp=.o)
tw_obj=$(tw_src:.cpp=.o)

all: agg-static agg-shared tw-static tw-shared

conf:
	@echo '#ifndef TWOMBLY_CONFIG_HEADER' > config.h
	@echo '#define TWOMBLY_CONFIG_HEADER' >> config.h
	echo $(CONFIG_CV) >> config.h
	echo $(CONFIG_FT) >> config.h
	@echo '#endif' >> config.h

compile: conf $(agg_obj) $(tw_obj)

agg-static: compile
	ar rcs libagg.a $(agg_obj)

agg-shared: compile
	$(CXX) -shared -o libagg.$(suffix) $(agg_obj) $(libs)

tw-static: compile
	ar rcs libtwombly.a $(agg_obj) $(tw_obj)

tw-shared: compile
	$(CXX) -shared -o libtwombly.$(suffix) $(agg_obj) $(tw_obj) $(libs)
	$(CXX) -shared -o libtwombly.$(VERSION).$(suffix) $(agg_obj) $(tw_obj) $(libs)

.PHONY: install
install:
	mkdir -p $(dest)/include/twombly $(dest)/include/agg $(dest)/lib/pkgconfig $(dest)/bin
	cp -r $(tw_hdrs) $(agg_hdrs) $(dest)/include/twombly
	cp $(agg_hdrs) $(dest)/include/agg
	cp libagg.* $(dest)/lib/
	cp libtwombly.* $(dest)/lib/
	printf "Name: twombly\nDescription: A 2d graphics library for C++\nVersion: $(VERSION)\nLibs: -L$(dest)/lib -ltwombly $(libs)\nCflags: -I/usr/local/include -I$(dest)/include\n" > $(dest)/lib/pkgconfig/twombly.pc
	echo "Requires: $(FREETYPE_PKG)" >> $(dest)/lib/pkgconfig/twombly.pc

uninstall:
	rm -rf $(dest)/include/twombly/
	rm -rf $(dest)/include/agg/
	rm -f $(dest)/lib/twombly.*
	rm -f $(dest)/lib/libagg.*
	rm -f $(dest)/lib/pkgconfig/twombly.pc

clean:
	rm -f $(agg_obj) $(tw_obj)

clean-libs:
	rm -f libtwombly.$(suffix) libtwombly.a libagg.$(suffix) libagg.a

%.o: %.cpp
	$(CXX) -O3 -std=c++11 -c -fPIC $*.cpp -I. $(incl) -o $@

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
	cd tests && CXX=$(CXX) freetype=$(freetype) $(MAKE) show=$(show)
	tests/run

parser:
	git submodule init && git submodule update
	cd ccparser && sudo python setup.py install
