//
// Created by User on 13.10.2023.
//

#include <vector>
#include "Graph.h"
#include "fstream"
#include "iostream"
#include "iomanip"

Graph::Graph(int v){
    vertices = v;
    edges = new int * [vertices];
    for(int i = 0; i < vertices; ++i) edges[i] = new int[vertices];

    for(int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            edges[i][j] = INT_MAX;
        }
    }
}

Graph::~Graph() {
    if(vertices > 0) {
        for (int i = 0; i < vertices; i++) {
            delete[] edges[i];
            edges[i] = nullptr;
        }
        delete[] edges;
        edges = nullptr;
    }
}


void Graph::changeSize(int size) {
    if(vertices > 0) {
        for (int i = 0; i < vertices; i++) {
            delete[] edges[i];
            edges[i] = nullptr;
        }
        delete[] edges;
        edges = nullptr;
    }
    vertices = size;
    edges = new int * [vertices];
    for(int i = 0; i < vertices; ++i) edges[i] = new int[vertices];

    for(int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            edges[i][j] = INT_MAX;
        }
    }
}

void Graph::readGraphDirected(const std::string& s){
    std::string name = R"(..\input\)" + s;
    std::ifstream file(name);

    if(file.is_open()) {
        std::string line;
        std::vector<int> lineData;

        for(int i = 0; i < 4; i++)
            std::getline(file, line);

        std::string number_str;
        for (char c : line) {
            if (std::isdigit(c)) {
                number_str += c;
            }
        }

        int dimensionValue;
        std::istringstream(number_str) >> dimensionValue;

        if(dimensionValue != vertices)
            Graph::changeSize(dimensionValue);

        int i = 0;
        while(std::getline(file, line)){
            std::stringstream lineStream(line);
            int value;
            while (lineStream >> value) {
                lineData.push_back(value);
                if(lineData.size() == vertices){
                    std::copy(lineData.begin(), lineData.end(), edges[i]);
                    i++;
                    lineData.clear();
                }
            }
        }

        file.close();
    }
    else std::cout << "\nTHERE WAS A PROBLEM WITH OPENING FILE";
}

