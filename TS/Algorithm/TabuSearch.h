//
// Created by Antek on 21.11.2023.
//

#ifndef TABUSEARCH_TABUSEARCH_H
#define TABUSEARCH_TABUSEARCH_H

#include <vector>
#include "../structures/Graph.h"
#include "list"

using namespace std;

class TabuSearch {
public:
    static long TSSolver(Graph &graph, int numberOfIterations);
private:
    struct Node{
        vector<int> path;
        int cost = 0;
        pair<int, int> move;
    };

    static Node randomSolution(Graph &graph);
    static vector<Node> defineNeighbours(Graph &graph, const Node& currentSolution, int iteration, int tabuTime, Node &bestSolution);
    static int calculateCost(Graph &graph, Node node);
    static bool isForbidden(pair<int, int> p);
    static int stopCondition(Graph &graph);
    static void printNode(Node node);
    static Node findSolution(const Node& currentSolution, vector<Node>& neighbours, Node &bestSolution);
};


#endif //TABUSEARCH_TABUSEARCH_H
