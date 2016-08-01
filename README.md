# Raycaster engine

![alt tag](https://raw.githubusercontent.com/balintkiss501/raycaster-engine/master/demo.gif)

My take on making a raycasting pseudo-3D engine in C++, also with my own tiny template linear algebra lib. One of the goals was to make raycasting computation equations more explicit and readable.

## Controls

* Up/Down to move and Left/Right to turn around
* WASD to move and strafe
* M to toggle mini-map

## Requirements

* C++11-standard compatible C++ compiler
* CMake
* [SDL2 library](https://www.libsdl.org/download-2.0.php) (apt-get install libsdl2-dev)

## How to build and run

Just run `./install.sh` or

1. `mkdir CMakeBuild && cmake ..`
2. `make`
3. `bin/Raycaster`

For cross-compilation, you can use one of the CMake toolchain files.

`TOOLCHAIN=CMake/toolchain/x86_64-w64-mingw32.cmake ./install.sh`

## References:

* http://permadi.com/1996/05/ray-casting-tutorial-table-of-contents
* http://lodev.org/cgtutor/raycasting.html
* http://www.instructables.com/id/Making-a-Basic-3D-Engine-in-Java
* Free sky texture is from http://www.texturex.com
