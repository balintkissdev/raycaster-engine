# Raycaster engine

![alt tag](https://raw.githubusercontent.com/balintkissdev/raycaster-engine/master/demo.png)

My take on making a raycasting pseudo-3D engine in C++, also with my own tiny template linear algebra lib. One of the goals was to make raycasting computation equations more explicit and readable.

## Downloads

* [Windows 64-bit executable](https://github.com/balintkissdev/raycaster-engine/releases/download/v0.0.1/raycaster-engine_windows_x86_64.zip)
* [Linux 64-bit executable](https://github.com/balintkissdev/raycaster-engine/releases/download/v0.0.1/raycaster-engine_linux_x86_64.tar.gz)

## Controls

* Up/Down to move and Left/Right to turn around
* WASD to move and strafe
* M to toggle mini-map

## Requirements

* C++14-standard compatible C++ compiler
* CMake 3.2 or newer

That's all. The dependencies are automatically downloaded by [Hunter](https://github.com/ruslo/hunter/).

## Build

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

For cross-compilation, you can use one of the CMake toolchain files.

`TOOLCHAIN=cmake/toolchain/x86_64-w64-mingw32.cmake cmake ..`

## References:

* http://permadi.com/1996/05/ray-casting-tutorial-table-of-contents
* http://lodev.org/cgtutor/raycasting.html
* http://www.instructables.com/id/Making-a-Basic-3D-Engine-in-Java
* Free sky texture is from http://www.texturex.com
* Free wall textures are from https://opengameart.org/content/wall-grass-rock-stone-wood-and-dirt-480
