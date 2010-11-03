CXX=clang++
CXXFLAGS=-g3 -Wall -Wextra -O2 \
	-I/usr/include/SDL \
	-I/usr/include/GL
OFILES=sdl_input_demo.o
OUT=sdl_input_demo

${OUT}: ${OFILES}
	${CXX} ${CCXFLAGS} -o $@ $^ \
		-lSDL -lGL -lGLU -lGLEW

clean:
	${RM} ${OUT} ${OFILES}

