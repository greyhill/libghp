libghp_gfx.so:
	(cd gfx && make && cp libghp_gfx.so ../)

install: libghp_gfx.so
	(mkdir /usr/include/ghp)
	(mkdir /usr/include/ghp/gfx && cp gfx/*.hpp /usr/include/ghp/gfx)
	(cp gfx.hpp /usr/include/ghp)
	(cp libghp_gfx.so /usr/lib)
	(mkdir /usr/include/ghp/math && cp math/*.hpp /usr/include/ghp/math)
	(cp math.hpp /usr/include/ghp)

deinstall:
	(rm -rf /usr/include/ghp)
	(rm -rf /usr/include/libghp_gfx.so)
	(rm /usr/lib/libghp_gfx.so)

reinstall: deinstall install

clean:
	(rm libghp_gfx.so && cd gfx && make clean)

