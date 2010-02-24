CXX=g++
CXXFLAGS=-g3 -Wall -Wextra
LIBARGS=-lSDL -lSDL_image -lSDL_ttf -lGL -lGLU -lGLEW -lghp_gfx
OUT=vbo_demo
OFILES=vbo_demo.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} ${LIBARGS} -o ${OUT} ${OFILES}

clean:
	${RM} ${OFILES} ${OUT}

