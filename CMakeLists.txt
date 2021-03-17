cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

include("cmake/modules/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.296.tar.gz"
    SHA1 "232f5022ee1d45955a7e8d3e1720f31bac1bb534"
)

set(PROJECT_NAME Raycaster)
project(${PROJECT_NAME} LANGUAGES CXX)
project(${PROJECT_NAME} VERSION 0.0.2)

add_executable(${PROJECT_NAME} "")

# Must be set after project() command
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if (WIN32)
    if (MINGW)
        target_link_options(${PROJECT_NAME} -static-libgcc -static-libstdc++)
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(src)

# Hunter SDL2 package uses static linking
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)

# Copy resource files
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)
