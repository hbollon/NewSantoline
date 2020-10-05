cmake_minimum_required(VERSION 3.12)
project(Epilobe)

set(CMAKE_CXX_STANDARD 14)

add_executable(Epilobe ../Utils/Configuration.h ../Utils/Configuration.cpp NoConfigException.hpp Epilobe.h Epilobe.cpp main.cpp)