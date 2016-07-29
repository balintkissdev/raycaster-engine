.PHONY: all default clean

CFLAGS=-g -Wall -std=c++0x
SRC=src/Main.cpp src/Game.cpp src/Camera.cpp src/RayCaster.cpp
LIB=-lSDL2
OBJ=raycaster

default:
	${CXX} ${CFLAGS} ${SRC} -o ${OBJ} ${LIB}

clean:
	rm -rf ${OBJ}
