cmake_minimum_required(VERSION 2.8.12)

# set project name
project(itax CXX)

# compiler options
add_compile_options(-std=c++11 -g -Wall -Wextra -Wpedantic -Wnon-virtual-dtor -Wold-style-cast)
# enables function names in backtraces
# set(CMAKE_EXE_LINKER_FLAGS -rdynamic)

# run clang-tidy during compilation
# include(cmake/static_analyzers.cmake)

# set include dirs
# include_directories(include)
add_subdirectory(libcore)
add_subdirectory(librules)

# source files
# file(GLOB SOURCES_LIBTAX "libtax/*.cpp")

# targets
# add_executable(itax ${SOURCES_LIBTAX})
