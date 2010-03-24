install: 
	(mkdir /usr/include/ghp)
	(mkdir /usr/include/ghp/gfx && cp gfx/*.hpp /usr/include/ghp/gfx)
	(cp gfx.hpp /usr/include/ghp)
	(mkdir /usr/include/ghp/math && cp math/*.hpp /usr/include/ghp/math)
	(cp math.hpp /usr/include/ghp)

deinstall:
	(rm -rf /usr/include/ghp)

reinstall: deinstall install

