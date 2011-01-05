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

.SUFFIXES: .opencl

CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
	${CLINCLUDE}
OBJS=cl_demo.o kernel.opencl.o
OUT=cl_demo

${OUT}: kernels ${OBJS}
	${CXX} ${CXXFLAGS} -lOpenCL -lGL -lSDL -lSDL_image \
		-lGLU -lGLEW ${OBJS} -o $@

kernels:
	clc -x kernel.opencl

clean:
	${RM} ${OUT} ${OBJS} kernel.opencl.*

