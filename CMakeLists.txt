cmake_minimum_required(VERSION 3.2)
project(GRAPHICS)

message("Find conan packages......")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add sub directories
add_subdirectory(Graphics)
