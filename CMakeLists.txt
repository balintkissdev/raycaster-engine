# Project definition
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
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
set(SRC
    src/Camera.cpp
    src/Game.cpp
    src/Main.cpp
    src/RayCaster.cpp
    src/SDLRenderer.cpp
)

# Target bin folder
if (WIN32)
    message("Target OS is Windows")
    set(SDL2_PATH "${CMAKE_FIND_ROOT_PATH};${SDL2_PATH}")
    message("SDL2_PATH: ${SDL2_PATH}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/windows)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message("Target OS is Mac")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/mac)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message("Target OS is Linux")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/linux)
endif()

# Dependencies
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake/modules)
find_package(SDL2 REQUIRED)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

# Copy resource files
if (NOT PROJECT_SOURCE_DIR STREQUAL CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(RESOURCES
        README.md
        resources/textures/dusk_sky_texture.bmp
        resources/map/map.dat
    )

    foreach(f IN LISTS RESOURCES)
        message(STATUS ${f})
        add_custom_command(
            OUTPUT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${f}"
            COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/${f}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${f}"
            DEPENDS "${PROJECT_SOURCE_DIR}/${f}"
        )
    endforeach()
endif()

add_custom_target(resources-target ALL DEPENDS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESOURCES}")

# Create executable
add_executable(${PROJECT_NAME} ${SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY})
