//
// Created by antek on 02.11.2023.
//

#ifndef PEA_B_B_BBSOLVER_H
#define PEA_B_B_BBSOLVER_H


#include "../structures/Graph.h"
#include "vector"
#include "../utils/Timer.h"

class BBSolver {

public:
    long solveBFS(Graph& graph, int city);
    long solveDFS(Graph& graph, int city);

private:
    Timer timer;
    Timer loopTimer;

    struct Node{
        std::vector<int> path;
        int cost;

        Node(const std::vector<int>& p, int c) : path(p), cost(c) {}
    };

    struct NodeComparator {
        bool operator()(const Node& a, const Node& b) {
            return a.cost > b.cost;
        }
    };

    static bool canAddVertex(const Node& currentNode, int i);
    static void relax(Graph& graph, int& lowerBound);
};


#endif //PEA_B_B_BBSOLVER_H
