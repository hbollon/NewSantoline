# cmake_minimum_required(VERSION <specify CMake version here>)
project(algo)

set(CMAKE_CXX_STANDARD 14)
SET(GCC_COVERAGE_COMPILE_FLAGS "-O3")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS -static)
### ATTENTION A L'ORDRE DE LA COMPILATION ###
add_executable(algo Utils.cpp Point.cpp Vecteur.cpp geometry.cpp ellipse.cpp WindMapData.cpp Algo.cpp main.cpp)