cmake_minimum_required(VERSION 3.12)
project(Epilobe)

set(CMAKE_CXX_STANDARD 14)
SET(GCC_COVERAGE_COMPILE_FLAGS "-O3")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS -static)

add_executable(Epilobe ../Utils/Configuration.h ../Utils/Configuration.cpp NoConfigException.hpp Epilobe.h Epilobe.cpp main.cpp)