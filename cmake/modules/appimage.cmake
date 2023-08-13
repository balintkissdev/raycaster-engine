# MIT License
#
# Copyright (c) 2021 Ravbug
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
function(make_appimage)
	set(optional)
	set(args EXE NAME DIR_ICON ICON OUTPUT_NAME)
	set(list_args ASSETS)
	cmake_parse_arguments(
		PARSE_ARGV 0
		ARGS
		"${optional}"
		"${args}"
		"${list_args}"
	)

	if(${ARGS_UNPARSED_ARGUMENTS})
		message(WARNING "Unparsed arguments: ${ARGS_UNPARSED_ARGUMENTS}")
	endif()


    # download AppImageTool if needed (TODO: non-x86 build machine?)
    SET(AIT_PATH "${CMAKE_BINARY_DIR}/AppImageTool.AppImage" CACHE INTERNAL "")
    if (NOT EXISTS "${AIT_PATH}")
        file(DOWNLOAD https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage "${AIT_PATH}")
        execute_process(COMMAND chmod +x ${AIT_PATH})
    endif()

    # make the AppDir
    set(APPDIR "${CMAKE_BINARY_DIR}/AppDir")
    file(REMOVE_RECURSE "${APPDIR}")       # remove if leftover
    file(MAKE_DIRECTORY "${APPDIR}")

    # copy executable to appdir
    file(COPY "${ARGS_EXE}" DESTINATION "${APPDIR}" FOLLOW_SYMLINK_CHAIN)
    get_filename_component(EXE_NAME "${ARGS_EXE}" NAME)

    # create the script that will launch the AppImage
file(WRITE "${APPDIR}/AppRun"
"#!/bin/sh
cd \"$(dirname \"$0\")\";
./${EXE_NAME} $@"
    )
    execute_process(COMMAND chmod +x "${APPDIR}/AppRun")

    # copy assets to appdir
    file(COPY ${ARGS_ASSETS} DESTINATION "${APPDIR}")

    # copy icon thumbnail
    file(COPY ${ARGS_DIR_ICON} DESTINATION "${APPDIR}")
    get_filename_component(THUMB_NAME "${ARGS_DIR_ICON}" NAME)
    file(RENAME "${APPDIR}/${THUMB_NAME}" "${APPDIR}/.DirIcon")

    # copy icon highres
    file(COPY ${ARGS_ICON} DESTINATION "${APPDIR}")
    get_filename_component(ICON_NAME "${ARGS_ICON}" NAME)
    get_filename_component(ICON_EXT "${ARGS_ICON}" EXT)
    file(RENAME "${APPDIR}/${ICON_NAME}" "${APPDIR}/${ARGS_NAME}${ICON_EXT}")

    # Create the .desktop file
    file(WRITE "${APPDIR}/${ARGS_NAME}.desktop"
    "[Desktop Entry]
Type=Application
Name=${ARGS_NAME}
Icon=${ARGS_NAME}
Categories=X-None;"
    )

    # Invoke AppImageTool
    execute_process(COMMAND ${AIT_PATH} ${APPDIR} ${ARGS_OUTPUT_NAME})
    file(REMOVE_RECURSE "${APPDIR}")

endfunction()

