ifdef ATI
CLINCLUDE=-I${ATISTREAMSDKROOT}/include/CL \
	-I${ATISTREAMSDKROOT}/include \
	-L${ATISTREAMSDKROOT}/lib/x86
else
CLINCLUDE=-I/usr/include/nvidia-current \
	-I/usr/include/nvidia-current/CL
endif

CXX=g++
CXXFLAGS=-g3 -Wall -Wextra \
	${CLINCLUDE}
OBJS=clc.o
OUT=clc

${OUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -lOpenCL \
		-lboost_program_options -lboost_regex $^ -o $@

install:
	cp ${OUT} /usr/bin

deinstall:
	rm /usr/bin/${OUT}

clean:
	${RM} ${OUT} ${OBJS}

