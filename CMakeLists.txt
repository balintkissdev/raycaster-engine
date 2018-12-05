# Project definition
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

include("cmake/modules/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.64.tar.gz"
    SHA1 "a5f3c4999e03173d28b8469c4da4545dea740a41"
)

set(PROJECT_NAME Raycaster)
set(PROJECT_VERSION "1.0-SNAPSHOT")
project(${PROJECT_NAME} LANGUAGES CXX)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ ")
set(CMAKE_CXX_STANDARD 14)
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_EXE_LINKER_FLAG} -mwindows ")
endif()

# Sources
set(INCLUDE_DIR src)
include_directories(${INCLUDE_DIR})
set(HEADERS
  src/Camera.h
  src/Game.h
  src/IRenderer.h
  src/MathLib.h
  src/RayCaster.h
  src/SDLDeleter.h
  src/SDLRenderer.h
  src/WallTypes.h
)
set(SRC
    src/Camera.cpp
    src/Game.cpp
    src/Main.cpp
    src/RayCaster.cpp
    src/SDLRenderer.cpp
)

# Dependencies
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${HEADERS} ${SRC})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)

# Copy resource files
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)
