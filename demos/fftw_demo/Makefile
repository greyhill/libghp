CXX=g++
CXXFLAGS=-g3 -Wall -Wextra
OFILES=fftw_demo_main.o
OUT=fftw_demo

${OUT}: ${OFILES}
	${CXX} ${CCXFLAGS} -lboost_thread-mt -lfftw3 -o $@ $^

clean:
	${RM} ${OUT} ${OFILES}

