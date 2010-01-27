CXX=g++
CXXFLAGS=-g3 -Wall -Wextra
OFILES=sqlite.o
OUT=libghp_sqlite.so

all: ${OUT}

${OUT}: ${OFILES}
	${CXX} ${CXXFLAGS} -shared -o $@ $^
	
clean:
	${RM} ${OUT} ${OFILES}
