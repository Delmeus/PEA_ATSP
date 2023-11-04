//
// Created by Antek on 04.11.2023.
//

#include <fstream>
#include <iostream>
#include <random>
#include "AutomaticTester.h"
#include "../utils/Timer.h"
#include "../structures/Graph.h"
#include "../utils/FileRandomizer.h"
#include "../algorithm/BBSolver.h"

using namespace std;

void AutomaticTester::test() {
    std::ofstream file(R"(..\output\results.csv)");
    if (!file.is_open()) {
        cout << "Couldn't open the file, ending test" << endl;
        return;
    }

    long long resultBFS = 0, resultDFS = 0;
    Timer timer;
    BBSolver solver;

    for (int j = 3; j < 24; j++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, j - 1);
        for (int i = 0; i < 100; i++) {
            cout << "Test " << j - 2 << "." << i << " has been started" << endl;

            Graph graph(j);
            FileRandomizer::randomize(j);
            /*
             * both search methods are tested on the same graph
             */
            int start = dist(gen);
            graph.readGraphDirected("random.atsp");
            resultBFS += solver.solveBFS(graph, start);
            graph.readGraphDirected("random.atsp");
            resultDFS += solver.solveDFS(graph, start);

            cout << "Test " << j - 2 << "." << i << " has been completed" << endl;
        }
        file    << "BFS" << ";" <<"rozmiar" << ";" << j << ";" << "sredni czas wykonania" << ";" << resultBFS / 100 << ";" << "mikrosekund" << ";"
                << "DFS" << ";" <<"rozmiar" << ";" << j << ";" << "sredni czas wykonania" << ";" << resultDFS / 100 << ";" << "mikrosekund" << endl;
        resultBFS = 0, resultDFS = 0;
    }

    file.close();
}
