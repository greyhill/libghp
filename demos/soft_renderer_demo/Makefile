CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -I/usr/include/SDL
OBJS=soft_renderer_demo.o
OUT=soft_renderer_demo

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lSDL -lSDL_image $^ -o $@

clean:
	${RM} ${OUT} ${OBJS}

