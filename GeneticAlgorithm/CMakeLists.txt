cmake_minimum_required(VERSION 3.27)
project(GeneticAlgorithm)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

add_executable(GeneticAlgorithm main.cpp
        algorithm/Node.cpp
        algorithm/Node.h
        algorithm/GeneticAlgorithm.cpp
        algorithm/GeneticAlgorithm.h
        structures/Graph.cpp
        structures/Graph.h
        utils/Timer.cpp
        utils/Timer.h)
