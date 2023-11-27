#include <iostream>
#include "structures/Graph.h"
#include "utils/FileRandomizer.h"
#include "Algorithm/TabuSearch.h"

int main() {
    /*
    string name;
    cout << "Podaj nazwe pliku: ";
    cin >> name;
    FileRandomizer::randomize(10);
    Graph graph(10);
    //graph.readGraphDirected("test.atsp"); //min cost = 2033 0 - 1 - 2 - 3 - 8 - 7 - 5 - 9 - 6 - 4 - 0
    graph.readGraphDirected(name);
    int answer;
    cout << "Podaj liczbe iteracji: ";
    cin >> answer;
    TabuSearch::TSSolver(graph, answer, true);
     */
    cout << " " << endl;
    Graph graph(10);

//    cout << "TEST 1 -> 2033" << endl;
//    graph.readGraphDirected("test.atsp");
//    TabuSearch::TSSolver(graph, 100000, false);

    cout << "TEST 2 -> 1776" << endl;
    graph.readGraphDirected("test2.atsp");
    //for(int i = 0; i < 5; i++)
    TabuSearch::TSSolver(graph, 10000000, false);

//    cout << "TEST 3 -> 2755" << endl;
//    graph.readGraphDirected("test3.atsp");
//    TabuSearch::TSSolver(graph, 10000000, false);
//
//    cout << "TEST 4 -> 2465" << endl;
//    graph.readGraphDirected("test4.atsp");
//    TabuSearch::TSSolver(graph, 10000000, false);

    std::cout << std::endl << std::endl << "press ENTER to continue..." << std::endl;
    std::cin.ignore(10000, '\n');
    getchar();

    return 0;
}
