# Makefile for Tanks
SERVER_TARGET=server
SERVER_SOURCES=${SERVER_TARGET}.c
SERVER_OBJES=${SERVER_TARGET}.o

CLIENT_TARGET=client
CLIENT_SOURCES=${CLIENT_TARGET}.c
CLIENT_OBJES=${CLIENT_TARGET}.o

CC=g++
CFLAGS=-g
CLIBS=
CLIBS_PATHS=

all:server client
	@echo Done

server:
	${CC} ${CFLAGS} ${CLIBS_PATHS} ${CLIBS} -o${SERVER_TARGET} ${SERVER_SOURCES}
	@if [ $$? ]; then echo "create server success."; else echo "create server faild."; fi

client:
	${CC} ${CFLAGS} ${CLIBS_PATHS} ${CLIBS} -o${CLIENT_TARGET} ${CLIENT_SOURCES}
	@if [ $$? ]; then echo "create client success."; else echo "create client faild."; fi

clean:
	@if [ -e ${SERVER_OBJES} ];then echo "rm ${SERVER_OBJES}"; rm ${SERVER_OBJES}; fi
	@if [ -e ${CLIENT_OBJES} ];then echo "rm ${CLIENT_OBJES}"; rm ${CLIENT_OBJES}; fi
	@if [ -e ${SERVER_TARGET} ];then echo "rm ${SERVER_TARGET}"; rm ${SERVER_TARGET}; fi
	@if [ -e ${CLIENT_TARGET} ];then echo "rm ${CLIENT_TARGET}"; rm ${CLIENT_TARGET}; fi
	@if [ -e a.out ]; then echo "rm a.out"; rm a.out; fi
