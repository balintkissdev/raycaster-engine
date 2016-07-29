.PHONY: all default clean

CC=g++

CFLAGS=-g -Wall -std=c++0x
SRC=src/Main.cpp src/Game.cpp src/Camera.cpp src/RayCaster.cpp
LIB=-lSDL2
OBJ=raycaster

default:
	${CC} ${CFLAGS} ${SRC} -o ${OBJ} ${LIB}

clean:
	rm -rf ${OBJ}
