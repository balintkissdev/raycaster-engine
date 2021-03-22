# Raycaster engine

![alt tag](https://raw.githubusercontent.com/balintkissdev/raycaster-engine/master/demo.gif)

[Live demo](https://balintkissdev.github.io/raycaster-engine)

My take on making a raycasting pseudo-3D engine in C++, also with my own tiny template linear algebra types. One of the goals was to make raycasting computation equations more explicit and readable.

## Controls

- Up/Down to move and Left/Right to turn around
- WASD to move and strafe
- M to toggle mini-map

## Requirements

- C++14-standard compatible C++ compiler
- CMake 3.16 or newer

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

### Emscripten support

This project can be built with Emscripten in order to be embedded in HTML. After you
[set up the Emscripten environment](https://emscripten.org/docs/getting_started/downloads.html),
issue these commands:

```bash
mkdir build
cd build
emcmake cmake ..
emmake make
```

## References:

- http://permadi.com/1996/05/ray-casting-tutorial-table-of-contents
- http://lodev.org/cgtutor/raycasting.html
- http://www.instructables.com/id/Making-a-Basic-3D-Engine-in-Java
- [Code-It-Yourself! First Person Shooter (Quick and Simple C++)](https://www.youtube.com/watch?v=xW8skO7MFYw) - by OneLoneCoder (javidx9)
  - [Upgraded! First Person Shooter at Command Prompt (C++)](https://www.youtube.com/watch?v=HEb2akswCcw)
  - [Super Fast Ray Casting in Tiled Worlds using DDA](https://www.youtube.com/watch?v=NbSee-XM7WA)
  - [Introducing RayCastWorld](https://www.youtube.com/watch?v=Vij_obgv9h4)
- Free sky texture is from http://www.texturex.com
- Free wall textures are from https://opengameart.org/content/wall-grass-rock-stone-wood-and-dirt-480
