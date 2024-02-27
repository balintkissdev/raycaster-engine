cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

option(WEBBUILD_CI "Specify if an Emscripten build is running in CI" OFF)

# Emscripten uses its own SDL2 package which conflicts with Hunter in CI.
# Disable Hunter during Emscripten CI otherwise it cannot find SDL2 header files.
if(NOT WEBBUILD_CI)
    set(HUNTER_PACKAGES SDL2)

    include(FetchContent)
    FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
    FetchContent_MakeAvailable(SetupHunter)
endif()

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' default.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

set(PROJECT_NAME Raycaster)
set(PROJECT_AUTHOR "Balint Kiss")
set(PROJECT_DESCRIPTION "Pseudo-3D raycasting engine in C++17.")
set(PROJECT_URL "https://github.com/balintkissdev/raycaster-engine")

# TODO: Get version from git tag
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 5)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project(${PROJECT_NAME} LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

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

# Copy asset files
add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_LINK    # Needed for Emscripten preload
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
)

# TODO: Remove exceptions
if(EMSCRIPTEN)
    # Output will be Raycaster.html. Intentional to keep names of Raycaster.js,
    # Raycaster.data and Raycaster.wasm.
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
        --preload-file "assets"
        --shell-file "${PROJECT_SOURCE_DIR}/src/shell_minimal.html"
    )
else()
    find_package(SDL2 CONFIG REQUIRED)

    # Hunter SDL2 package uses static linking
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2)
endif()

# Create distributable ZIP file for Windows releases
if(WIN32)
    set(CPACK_PACKAGE_NAME "Raycaster")
    set(CPACK_PACKAGE_DESCRIPTION "C++ raycasting pseudo-3D engine")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")

    install(FILES "${CPACK_RESOURCE_FILE_LICENSE}" "${CPACK_RESOURCE_FILE_README}" DESTINATION ".")
    install(TARGETS ${PROJECT_NAME} RUNTIME CONFIGURATIONS Release DESTINATION ".")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets/" DESTINATION "assets")
    set(CPACK_GENERATOR ZIP)
    include(CPack)
elseif(EMSCRIPTEN)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.data" DESTINATION ".")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.js" DESTINATION ".")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.wasm" DESTINATION ".")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.html" RENAME "index.html" DESTINATION ".")
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets/" DESTINATION "assets")
# Linux releases are distributed as AppImage files instead for compatibility
else()
    # Keep "assets" and "share" folder separate to avoid packaging icon as asset for the app
    set(ICON "${PROJECT_SOURCE_DIR}/share/icons/appimage.svg")
    install(CODE
    "include(${PROJECT_SOURCE_DIR}/cmake/modules/appimage.cmake)
    make_appimage(
        EXE \"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}\"
        NAME \"${PROJECT_NAME}\"
        ICON \"${ICON}\"
        DIR_ICON \"${ICON}\"
        OUTPUT_NAME \"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}-linux-${CMAKE_SYSTEM_PROCESSOR}.AppImage\"
        ASSETS \"${CMAKE_CURRENT_BINARY_DIR}/assets\"
    )
    "
    COMPONENT Runtime
    )
endif()
