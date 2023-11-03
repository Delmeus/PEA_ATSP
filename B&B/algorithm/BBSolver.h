//
// Created by antek on 02.11.2023.
//

#ifndef PEA_B_B_BBSOLVER_H
#define PEA_B_B_BBSOLVER_H


#include "../structures/Graph.h"

class BBSolver {
public:
    void solve(Graph& graph);

private:
    void search();
    void bound();
    Graph relax(Graph& graph, int& x, int& y);
};


#endif //PEA_B_B_BBSOLVER_H
