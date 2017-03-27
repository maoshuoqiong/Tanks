# Makefile for Tanks Server
SOURCES=MyList.h test.cpp
TARGET=tank_server
OBJES=test.o

CC=g++
CFLAGS=-g
CLIBS=
CLIBS_PATHS=

all:objs
	${CC} ${CLIBS_PATHS} ${CLIBS} -o${TARGET} ${OBJES}
	@rm *.gch ${OBJES}

objs:
	${CC} ${CFLAGS} -c ${SOURCES}

clean:
	@if [ -e ${OBJES} ];then echo "rm ${OBJES}"; rm ${OBJES}; fi
	@if [ -e ${TARGET} ];then echo "rm ${TARGET}"; rm ${TARGET}; fi
	@if [ -e a.out ]; then echo "rm a.out"; rm a.out; fi
	@if [ -e *.gch ]; then echo "rm *.gch"; rm *.gch; fi
