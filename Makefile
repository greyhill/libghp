install: 
	(cd util && perl mk_cl_device_extensions_methods.pl)
	(mkdir /usr/include/ghp)
	(mkdir /usr/include/ghp/gfx && cp gfx/* /usr/include/ghp/gfx)
	(cp gfx.hpp /usr/include/ghp)
	(mkdir /usr/include/ghp/math && cp math/* /usr/include/ghp/math)
	(cp math.hpp /usr/include/ghp)
	(mkdir /usr/include/ghp/janus && cp janus/* janus/*.janus \
		/usr/include/ghp/janus)
	(cp janus.hpp /usr/include/ghp)
	(mkdir /usr/include/ghp/util && cp util/* /usr/include/ghp/util)
	(cp util.hpp /usr/include/ghp)
#	(mkdir /usr/include/ghp/net && cp net/*.hpp /usr/include/ghp/net)
#	(cp net.hpp /usr/include/ghp)

deinstall:
	(rm -rf /usr/include/ghp)

reinstall: deinstall install

