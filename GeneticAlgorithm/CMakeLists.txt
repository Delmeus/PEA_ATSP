cmake_minimum_required(VERSION 3.27)
project(GeneticAlgorithm)

set(CMAKE_CXX_STANDARD 23)

add_executable(GeneticAlgorithm main.cpp
        algorithm/Node.cpp
        algorithm/Node.h
        algorithm/GeneticAlgorithm.cpp
        algorithm/GeneticAlgorithm.h
        structures/Graph.cpp
        structures/Graph.h
        utils/Timer.cpp
        utils/Timer.h)
