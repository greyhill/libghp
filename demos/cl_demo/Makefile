CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
	-I/usr/include/nvidia-current \
	-I/usr/include/nvidia-current/CL
OBJS=cl_demo.o
OUT=cl_demo

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lOpenCL $^ -o $@

clean:
	${RM} ${OUT} ${OBJS}

