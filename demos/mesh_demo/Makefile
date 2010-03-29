CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -I/usr/include/SDL
OBJS=mesh_demo.o
OUT=mesh_demo

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lSDL -lSDL_image $^ -o $@

clean:
	${RM} ${OUT} ${OBJS}

