CXX=g++
CXXFLAGS=-g3 -Wall -Wextra -O2 -I../
OUT=libghp_gfx.so
OFILES= \
	display.o \
	texture.o

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} -shared $^ -o $@

clean:
	${RM} ${OFILES} ${OUT}

