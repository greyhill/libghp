CXX=clang++
CXXFLAGS=-g3 -Wall -Wextra \
		-I/usr/include/SDL -I/usr/include/GL
LIBARGS=-lSDL -lSDL_ttf -lGL -lGLU -lGLEW 
OUT=font_demo
OFILES=font_demo.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} ${LIBARGS} -o ${OUT} ${OFILES}

clean:
	${RM} ${OFILES} ${OUT}

