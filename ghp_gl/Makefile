CXX = g++
CXXFLAGS = -g3 -Wall -fPIC -O2 \
	--std=c++0x -I../ghp_math
OUT = libghp_gl.so
OFILES = ghp_gl_window.o

all: ${OUT}

${OUT}: ${OFILES}
	${CXX} -shared ${CXXFLAGS} -o $@ $^ 

clean:
	${RM} ${OUT} ${OFILES}

