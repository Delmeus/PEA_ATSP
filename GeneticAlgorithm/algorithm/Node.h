//
// Created by antek on 31.12.2023.
//

#ifndef GENETICALGORITHM_NODE_H
#define GENETICALGORITHM_NODE_H

#include <vector>
#include "../structures/Graph.h"

using namespace std;

class Node {
private:
    int cost = 0;
public:
    vector<int> chromosome;

    Node scrambleMutate();
    static Node generateRandomNode(const Graph& graph);
    void printNode();
    void calculateCost(const Graph& graph);

};


#endif //GENETICALGORITHM_NODE_H
