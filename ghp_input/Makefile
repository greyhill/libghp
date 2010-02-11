CXX = g++
CXXFLAGS = -g3 -Wall -fPIC -O2 \
	-std=c++0x \
	-I ../ghp_math
OUT = libghp_input.so
OFILES = ghp_input_main.o \
	ghp_input_mouse.o

all: ${OUT}

${OUT}: ${OFILES}
	${CXX} -shared ${CXXFLAGS} -o $@ $^

clean:
	${RM} ${OUT} ${OFILES}

