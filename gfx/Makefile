CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -O0 -I../
OUT=libghp_gfx.so
OFILES= \
	display.o \
	texture.o \
	vertex_buffer.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} -shared $^ -o $@

clean:
	${RM} ${OFILES} ${OUT}

