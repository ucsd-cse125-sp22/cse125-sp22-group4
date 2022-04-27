cmake_minimum_required(VERSION 3.6)
project(cse125-sp22-group4 CXX)

# useful variables
set(LIB_INCLUDE "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_CONFIGURATION_TYPES "Release")

message("Find conan packages......")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add sub directories
add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(lib/Network)
add_subdirectory(lib/Graphics)
add_subdirectory(lib/Constants)
add_subdirectory(lib/Logic)

# set startup project
#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Server)
