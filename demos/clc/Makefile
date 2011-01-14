ifdef ATI
CLINCLUDE=-I${ATISTREAMSDKROOT}/include/CL \
	-I${ATISTREAMSDKROOT}/include \
	-L${ATISTREAMSDKROOT}/lib/x86
endif
ifdef UBUNTU_NV
CLINCLUDE=-I/usr/include/nvidia-current \
	-I/usr/include/nvidia-current/CL \
	-I/usr/include/GL
endif
ifdef RH_NV
CLINCLUDE=-I/usr/local/cuda/include \
	-I/usr/local/cuda/include/CL \
	-I/usr/include/GL
endif

ifndef GHPDIR
GHPDIR=/usr/include
endif

CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
	${CLINCLUDE} -I${GHPDIR} \
	-D GHP_CL_NOGL
OBJS=clc.o
OUT=clc

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lOpenCL \
		-lboost_program_options -lboost_regex -lboost_filesystem $^ -o $@

install:
	cp ${OUT} /usr/bin

deinstall:
	rm /usr/bin/${OUT}

clean:
	${RM} ${OUT} ${OBJS}

