#   cmake -DCMAKE_TOOLCAHIN_FILE=../CMake/toolchain/i686-w64-mingw32.cmake ..
#   make
set(CMAKE_SYSTEM_NAME Windows)

set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)

set(CMAKE_FIND_ROOT_PATH /usr/i686-w64-mingw32)

set($ENV{SDL2DIR} "$ENV{SDL2DIR} ${CMAKE_FIND_ROOT_PATH}" )
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY) 

# Needed, so CMake should not search PkgConfig files in /usr/local/lib
set($ENV{PKG_CONFIG_LIBDIR} ${CMAKE_FIND_ROOT_PATH}/lib/pkgconfig)
set($ENV{PKG_CONFIG_PATH} ${CMAKE_FIND_ROOT_PATH}/lib/pkgconfig)
