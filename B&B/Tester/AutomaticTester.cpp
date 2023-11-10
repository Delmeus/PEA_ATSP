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

void AutomaticTester::test(int size) {
    long long resultBFS = 0, resultDFS = 0;
    Timer timer;
    BBSolver solver;

    if(size == 0) {

        ofstream file(R"(..\output\results.csv)");
        if (!file.is_open()) {
            cout << "Couldn't open the file, ending test" << endl;
            return;
        }

        for (int j = 3; j <= 25; j++) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, j - 1);
            int divBFS = 100, divDFS = 100;
            for (int i = 0; i < 100; i++) {
                cout << "------------------------------------------" << endl
                     << "Test nr " << i << " for size " << j << " has been started" << endl;

                Graph graph(j);
                FileRandomizer::randomize(j);
                /*
                 * both search methods are tested on the same graph
                 */
                int start = dist(gen);
                long result;
                cout << "BFS" << endl;

                graph.readGraphDirected("random.atsp");
                result = solver.solveBFS(graph, start);
                /*
                 * if function took more that two minutes to complete
                 * dont count the result
                 */
                if (result >= 2000 * 60 * 1000)
                    divBFS--;
                else
                    resultBFS += result;

                cout << "DFS" << endl;
                graph.readGraphDirected("random.atsp");
                result += solver.solveDFS(graph, start);
                if (result >= 2000 * 60 * 1000)
                    divDFS--;
                else
                    resultDFS += result;

                cout << "Test nr" << i << " for size " << j << " has been completed" << endl;
            }
            if (!(divBFS == 0 || divDFS == 0)) {
                file << "BFS" << ";" << "rozmiar" << ";" << j << ";" << "sredni czas wykonania" << ";"
                     << resultBFS / divBFS
                     << ";" << "mikrosekund" << ";" << " nie znaleziono rozwiazania " << ";" << 100 - divBFS << ";"
                     << ";"
                     << "DFS" << ";" << "rozmiar" << ";" << j << ";" << "sredni czas wykonania" << ";"
                     << resultDFS / divDFS
                     << ";" << "mikrosekund" << ";" << " nie znaleziono rozwiazania " << ";" << 100 - divDFS << endl;
            }
            resultBFS = 0, resultDFS = 0;
        }
        file.close();
    }
    else{

        ofstream file(R"(..\output\results)" + to_string(size) + ".csv");
        if (!file.is_open()) {
            cout << "Couldn't open the file, ending test" << endl;
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, size - 1);
        int divBFS = 100, divDFS = 100;
        for (int i = 0; i < 100; i++) {
            cout << "------------------------------------------" << endl
                 << "Test nr " << i << " for size " << size << " has been started" << endl;

            Graph graph(size);
            FileRandomizer::randomize(size);
            /*
             * both search methods are tested on the same graph
             */
            int start = dist(gen);
            long result;
            cout << "BFS" << endl;

            graph.readGraphDirected("random.atsp");
            result = solver.solveBFS(graph, start);
            /*
             * if function took more that two minutes to complete
             * dont count the result
             */
            if (result >= 2000 * 60 * 1000)
                divBFS--;
            else
                resultBFS += result;

            cout << "DFS" << endl;
            graph.readGraphDirected("random.atsp");
            result += solver.solveDFS(graph, start);
            if (result >= 2000 * 60 * 1000)
                divDFS--;
            else
                resultDFS += result;

            cout << "Test nr" << i << " for size " << size << " has been completed" << endl;
        }
        if (!(divBFS == 0 || divDFS == 0)) {
            file << "BFS" << endl   << "Rozmiar" << ";" << "Sredni czas wykonania w mikrosekundach" << ";" << "Nieznaleziono rozwiazania" << endl
                                    << size << ";" << resultBFS / divBFS << ";" << 100 - divBFS << endl;
            file << "DFS" << endl   << "Rozmiar" << ";" << "Sredni czas wykonania w mikrosekundach" << ";" << "Nieznaleziono rozwiazania" << endl
                 << size << ";" << resultDFS / divDFS << ";" << 100 - divDFS << endl;
        }
        file.close();
    }

}
