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
    static int findNearestNeighbour(const Graph &graph, int currentVertex, const vector<bool> &visited);
public:
    int cost = -1;
    vector<int> chromosome;
    double fitness;

    Node mutate(bool mutationMethod);
    Node scrambleMutate();
    Node inversionMutate();
    Node selectParent(const vector<Node>& population);
    static Node greedySolution(const Graph& graph);
    static Node generateRandomNode(const Graph& graph);
    void printNode();
    void calculateCost(const Graph& graph);

    bool operator==(const Node& other) const {
        return chromosome == other.chromosome;
    }

};


#endif //GENETICALGORITHM_NODE_H
