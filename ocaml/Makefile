ccopt=-std=c99 -I /usr/local/include -L/usr/local/lib `pkg-config --cflags freetype2`

all: lib.native lib.byte

lib.native:
	$(CC) $(ccopt) -c -o twombly_stubs.o twombly_stubs.c -I `ocamlc -where`
	ar rcs libtwombly_stubs.a twombly_stubs.o
	ocamlopt -ccopt "$(ccopt) -lbimage -ltwombly `pkg-config --cflags --libs freetype2`" -a -o twombly.cmxa -I `ocamlfind query bimage` -cclib -ltwombly_stubs twombly.ml

lib.byte:
	$(CC) $(ccopt) -shared -fPIC -o dlltwombly_stubs.so twombly_stubs.c -I `ocamlc -where`
	ocamlc -ccopt "$(ccopt) -lbimage -ltwombly `pkg-config --cflags --libs freetype2`" -I `ocamlfind query bimage` -a -o twombly.cma -I . -dllib -ltwombly_stubs twombly.ml

install:
	$(MAKE) uninstall || :
	ocamlfind install twombly META twombly.cmxa twombly.cmx twombly.cmi twombly.a libtwombly_stubs.a dlltwombly_stubs.so twombly.cma

uninstall:
	ocamlfind remove twombly

clean:
	rm -f *.o *.cm* *.so *.a *.mli
