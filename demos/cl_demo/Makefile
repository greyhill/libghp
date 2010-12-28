ifdef ATI
CLINCLUDE=-I${ATISTREAMSDKROOT}/include/CL \
	-I${ATISTREAMSDKROOT}/include \
	-L${ATISTREAMSDKROOT}/lib/x86
else
CLINCLUDE=-I/usr/include/nvidia-current \
	-I/usr/include/nvidia-current/CL \
	-I/usr/include/GL \
	-I/usr/include/SDL
endif

CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
	${CLINCLUDE}
OBJS=cl_demo.o
OUT=cl_demo

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lOpenCL -lGL -lSDL -lGLU -lGLEW $^ -o $@

clean:
	${RM} ${OUT} ${OBJS}

