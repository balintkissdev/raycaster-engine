#!/bin/bash

# Usage with toolchain: TOOLCHAIN=CMake/toolchain/<your toolchain>.cmake ./install.sh 

if [ ! -d CMakeBuild ]
then
	mkdir CMakeBuild
else
	rm -rf CMakeBuild/*
fi
cd CMakeBuild && cmake -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN} .. && make && cd ..
