set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_VER "v0.14.3")

# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS 
        "components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(telescope)
