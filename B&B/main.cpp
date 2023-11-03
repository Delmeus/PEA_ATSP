#include <iostream>
#include "algorithm/BBSolver.h"

int main() {
    BBSolver solver;
    Graph graph(5);
    graph.readGraphDirected("test.atsp");
    solver.solve(graph);
    return 0;
}
