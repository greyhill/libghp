CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -O0 \
	-I/usr/include/SDL -I/usr/include/GL
LIBARGS=-lSDL -lSDL_image -lGL -lGLU -lGLEW -lboost_thread \
		-lboost_filesystem -lsqlite3
OUT=rshow
OFILES=rshow.o rshow_shared.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} ${LIBARGS} -o ${OUT} ${OFILES}

clean:
	${RM} ${OFILES} ${OUT}

