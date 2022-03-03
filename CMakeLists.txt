cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(HUNTER_PACKAGES SDL2)

include(FetchContent)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

set(PROJECT_NAME Raycaster)
project(${PROJECT_NAME} LANGUAGES CXX)
project(${PROJECT_NAME} VERSION 0.0.4)  # TODO: Get version from git tag

add_executable(${PROJECT_NAME} "")

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

find_program(
    CLANG_TIDY_EXE
    NAMES clang-tidy
)
if (CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY
        "${CLANG_TIDY_EXE}"
        "--header-filter=src/*"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
else()
    message(STATUS "clang-tidy not found.")
endif()

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(WIN32)
    if (MINGW)
        target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
    endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(src)

# Copy resource files
add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_LINK    # Needed for Emscripten preload
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
)

# TODO: Remove exceptions
if(EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".html"
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        -O3
        "SHELL: -s USE_SDL=2"
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        "SHELL: -s USE_SDL=2"
        "SHELL: -s INITIAL_MEMORY=33554432"
        --preload-file "resources"
        --shell-file "${PROJECT_SOURCE_DIR}/src/shell_minimal.html"
    )
else()
    find_package(SDL2 CONFIG REQUIRED)

    # Hunter SDL2 package uses static linking
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)
endif()

set(CPACK_PACKAGE_NAME "Raycaster")
set(CPACK_PACKAGE_DESCRIPTION "C++ raycasting pseudo-3D engine")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

install(FILES "${CPACK_RESOURCE_FILE_LICENSE}" "${CPACK_RESOURCE_FILE_README}" DESTINATION ".")
install(TARGETS ${PROJECT_NAME} RUNTIME CONFIGURATIONS Release DESTINATION ".")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/resources/" DESTINATION "resources")
if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TGZ)
endif()
include(CPack)