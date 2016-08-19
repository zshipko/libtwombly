ccopt=-std=c99 -I /usr/local/include -L/usr/local/lib -lbimage -ltwombly `pkg-config --cflags --libs freetype2`

default:
	ocamlc -I `ocamlfind query bimage` -i twombly.ml > twombly.mli
	ocamlopt -a -o twombly.cmxa -ccopt "$(ccopt) -fPIC" -I `ocamlfind query bimage` twombly.ml twombly_stubs.c

install:
	$(MAKE) uninstall || :
	ocamlfind install twombly META twombly.cmxa twombly.cmx twombly.mli twombly.cmi *.o *.cmo twombly.a

uninstall:
	ocamlfind remove twombly
