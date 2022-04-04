cmake_minimum_required(VERSION 3.2)
project(GRAPHICS)

# useful variables
set(GRAPHICS_EXTERNAL "${CMAKE_SOURCE_DIR}/external")

message("Find conan packages......")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add sub directories
add_subdirectory(src)

