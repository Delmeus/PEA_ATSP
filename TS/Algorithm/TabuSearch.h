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
    static int TSSolver(Graph &graph, int algorithmTime, bool print);
private:
    struct Node{
        vector<int> path;
        int cost = 0;
        pair<int, int> move;
    };

    static Node firstSolution(Graph &graph);
    static Node randomSolution(Graph &graph);
    static vector<Node> defineNeighbours(Graph &graph, const Node& currentSolution, int timeSinceChange, int tabuTime, Node &bestSolution, bool print);
    static int calculateCost(Graph &graph, Node node);
    static void printNode(Node node);
    static Node findSolution(const Node& currentSolution, vector<Node>& neighbours, Node &bestSolution);


};


#endif //TABUSEARCH_TABUSEARCH_H
