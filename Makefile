.PHONY: all default clean

CC=g++

CFLAGS=-g -Wall -std=c++0x
SRC=Main.cpp Game.cpp Camera.cpp RayCaster.cpp
LIB=-lSDL2
OBJ=raycaster

default:
	${CC} ${CFLAGS} ${SRC} -o ${OBJ} ${LIB}

clean:
	rm -rf ${OBJ}
