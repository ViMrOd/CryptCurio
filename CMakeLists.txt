# Build for main program

cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

# exe_file
add_executable(exe_file)
target_sources(exe_file PRIVATE main.c bn.c)

# Enable -O3 optimization and check all warnings
target_compile_options(exe_file PRIVATE -O3 -Wall)

# testing
include(CTest)
add_subdirectory(testsuite)

