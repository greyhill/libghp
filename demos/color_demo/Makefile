CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -I/usr/include/SDL
OBJS=color_demo.o
OUT=color_demo

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lSDL -lSDL_image $^ -o $@

clean:
	${RM} ${OUT} ${OBJS}

