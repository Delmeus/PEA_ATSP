#include <iostream>
#include "structures/Graph.h"
#include "utils/FileRandomizer.h"
#include "Algorithm/TabuSearch.h"

int main() {
    FileRandomizer::randomize(10);
    Graph graph(10);
    graph.readGraphDirected("test.atsp"); //min cost = 2033 0 - 1 - 2 - 3 - 8 - 7 - 5 - 9 - 6 - 4 - 0
    int answer;
    cout << "Podaj liczbe iteracji: ";
    cin >> answer;
    TabuSearch::TSSolver(graph, answer);

    std::cout << std::endl << std::endl << "press ENTER to continue..." << std::endl;
    std::cin.ignore(10000, '\n');
    getchar();

    return 0;
}
