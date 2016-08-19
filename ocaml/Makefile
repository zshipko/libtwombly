ccopt=-std=c99 -I /usr/local/include -L/usr/local/lib -lbimage -ltwombly `pkg-config --cflags --libs freetype2`

default:
	$(CC) $(ccopt) -c -o twombly_stubs.o twombly_stubs.c -I `ocamlc -where`
	ar rcs libtwombly_stubs.a twombly_stubs.o
	ocamlopt -ccopt "$(ccopt)" -a -o twombly.cmxa -I `ocamlfind query bimage` -cclib -ltwombly_stubs twombly.ml

install:
	$(MAKE) uninstall || :
	ocamlfind install twombly META twombly.cmxa twombly.cmx twombly.cmi twombly.a libtwombly_stubs.a

uninstall:
	ocamlfind remove twombly

clean:
	rm -f *.o *.cm* *.so *.a *.mli
