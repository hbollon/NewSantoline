cmake_minimum_required(VERSION 3.12)
project(algo)

set(CMAKE_CXX_STANDARD 14)

### ATTENTION A L'ORDRE DE LA COMPILATION ###
add_executable(algo ConvexHull.cpp Utils.cpp Point.cpp Vecteur.cpp geometry.cpp ellipse.cpp WindMapData.cpp Algo.cpp main.cpp)