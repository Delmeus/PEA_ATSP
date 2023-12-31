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
    vector<int> chromosome;
    int cost;
public:
    Node scrambleMutate();
    static Node generateRandomNode(const Graph& graph);
    void printNode();
};


#endif //GENETICALGORITHM_NODE_H
