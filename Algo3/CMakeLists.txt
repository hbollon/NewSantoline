cmake_minimum_required(VERSION 3.12)
project(Algo3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS -static)


add_executable(Algo3 main.cpp Point.cpp Vecteur.cpp Utils.cpp PointOpt.cpp PointOpt.hpp VitesseOpt.cpp VitesseOpt.hpp AlgoOpt.cpp AlgoOpt.hpp EllipseOpt.cpp EllipseOpt.hpp Choc.cpp Choc.hpp SommetCase.cpp SommetCase.h)