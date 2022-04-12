cmake_minimum_required(VERSION 3.2)
project(cse125-sp22-group4)

# useful variables
set(LIB_INCLUDE "${CMAKE_SOURCE_DIR}/lib")

message("Find conan packages......")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add sub directories
add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(lib/Network)
add_subdirectory(lib/Graphics)
