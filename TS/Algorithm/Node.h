//
// Created by Antek on 02.12.2023.
//

#ifndef TABUSEARCH_NODE_H
#define TABUSEARCH_NODE_H

#include <vector>
#include "../structures/Graph.h"
#include "TabuManager.h"

using namespace std;

class Node {
public:
    int cost = 0;
    vector<int> path;
    pair<int, int> move;

    static int calculateCost(Graph &graph, Node node);
    static int findNearestNeighbour(const Graph& graph, int currentVertex, const std::vector<bool>& visited);
    static void printNode(Node node);
    static Node randomSolution(Graph &graph);
    static Node firstSolution(Graph &graph);
    static Node findSolution(const Node& currentSolution, vector<Node>& neighbours, Node &bestSolution,  TabuManager params);
};


#endif //TABUSEARCH_NODE_H
