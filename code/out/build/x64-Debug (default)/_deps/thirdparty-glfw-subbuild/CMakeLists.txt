# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.18.20081302-MSVC_2)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(thirdparty-glfw-populate NONE)

include(ExternalProject)
ExternalProject_Add(thirdparty-glfw-populate
                     "UPDATE_DISCONNECTED" "True" "URL" "G:/My Documents/COMP371/code/thirdparty/glfw-3.3.2.zip"
                    SOURCE_DIR          "G:/My Documents/COMP371/code/out/build/x64-Debug (default)/_deps/thirdparty-glfw-src"
                    BINARY_DIR          "G:/My Documents/COMP371/code/out/build/x64-Debug (default)/_deps/thirdparty-glfw-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


