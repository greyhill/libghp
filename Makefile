libghp_gfx.so:
	(cd gfx && make && cp libghp_gfx.so ../)

clean:
	(rm libghp_gfx.so && cd gfx && make clean)

