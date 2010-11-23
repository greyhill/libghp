CXX=clang++
CXXFLAGS=-g3 -Wall -Wextra \
	-I/usr/include/SDL -I/usr/include/GL
LIBARGS=-lSDL -lSDL_image -lGL -lGLU -lGLEW
OUT=rshow
OFILES=rshow.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} ${LIBARGS} -o ${OUT} ${OFILES}

clean:
	${RM} ${OFILES} ${OUT}

