CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
		-I/usr/include/SDL -I/usr/include/GL
LIBARGS=-lSDL -lSDL_image -lGL -lGLU -lGLEW 
OUT=vbo_demo
OFILES=vbo_demo.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} ${LIBARGS} -o ${OUT} ${OFILES}

clean:
	${RM} ${OFILES} ${OUT}

