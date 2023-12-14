//
// Created by Antek on 21.11.2023.
//

#ifndef TABUSEARCH_TABUSEARCH_H
#define TABUSEARCH_TABUSEARCH_H

#include <vector>
#include "../structures/Graph.h"
#include "list"
#include "Node.h"

using namespace std;

class TabuSearch {
public:
    static pair<Node, long> TSSolver(Graph &graph, int algorithmTime, bool print, int neighbourhoodMethod, int target);
private:

    static vector<Node> swapTwoCities(Graph &graph, const Node& currentSolution, int timeSinceChange);
    static vector<Node> subPaths(Graph &graph, const Node& currentSolution, int timeSinceChange);
    static vector<Node> permuteFragment(Graph &graph, const Node& currentSolution, int timeSinceChange);
    static vector<Node> defineNeighbours(Graph &graph, const Node& currentSolution, int timeSinceChange, Node &bestSolution,
                                         int method, long iteration);

};


#endif //TABUSEARCH_TABUSEARCH_H
