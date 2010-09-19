ifdef ATI
CLINCLUDE=-I/usr/include/CL
else
CLINCLUDE=-I/usr/include/nvidia-current \
	-I/usr/include/nvidia-current/CL
endif

CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
	${CLINCLUDE}
OBJS=cl_demo.o
OUT=cl_demo

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lOpenCL $^ -o $@

clean:
	${RM} ${OUT} ${OBJS}

