//
// Created by Antek on 29.11.2023.
//

#include <iostream>
#include <fstream>
#include "Tester.h"
#include "../structures/Graph.h"
#include "../Algorithm/TabuSearch.h"

void Tester::test() {
    int answer, time = 1, target, neighbourhoodMethod = 1;
    string fileName = "ftv47.atsp", methodOfNeighbours = "Swap two cities";
    target = 1776;
    Graph graph(1);
    graph.readGraphDirected(fileName);
    do{
        system("CLS");
        cout << "--------------Tabu Search--------------" << endl
            << "Current settings: " << endl
            << "time = " << time << " min, " << "target cost = " << target << ", file = " << fileName << endl
            << "method of choosing neighbours = " << methodOfNeighbours << endl
            << "------------------MENU------------------" << endl
            << "1. Test algorithm" << endl
            << "2. Load file" << endl
            << "3. Change stop condition" << endl
            << "4. Change how neighbours are defined" << endl
            << "5. Automatic test" << endl
            << "6. Exit program"  << endl;
        cin >> answer;
        switch(answer){
            case 1:
                TabuSearch::TSSolver(graph, time, true, neighbourhoodMethod, target);
                waitForResponse();
                break;
            case 2:
                do {
                   system("CLS");
                   cout << "What file do you wish to test on?" << endl
                        << "1. ftv47.atsp" << endl
                        << "2. ftv170.atsp" << endl
                        << "3. rbg403.atsp" << endl;
                   cin >> answer;
                   waitForResponse();
                }while(answer < 1 || answer > 3);
                if(answer == 1) {
                    fileName = "ftv47.atsp";
                    target = 1776;
                }
                else if(answer == 2) {
                    fileName = "ftv170.atsp";
                    target = 2755;
                }
                else {
                    fileName = "rbg403.atsp";
                    target = 2465;
                }
                graph.readGraphDirected(fileName);
                break;
            case 3:
                do {
                    system("CLS");
                    cout << "How long do you want the algorithm to go on for? (minutes): ";
                    cin >> time;
                    waitForResponse();
                }while(time < 0 || time > 15);
                break;
            case 4:
                do {
                    system("CLS");
                    cout    << "How do you want to define neighbours?" << endl
                            << "1. Swap two cities" << endl
                            << "2. Swap two node sub-paths" << endl
                            << "3. Permute a fragment" << endl
                            << "4. Mixed method" << endl;
                    cin >> answer;
                }while(answer < 0 || answer > 4);
                neighbourhoodMethod = answer;
                if(answer == 1)
                    methodOfNeighbours = "Swap two cities";
                else if(answer == 2)
                    methodOfNeighbours = "Swap two node sub-paths";
                else if(answer == 3)
                    methodOfNeighbours = "Permute a fragment";
                else
                    methodOfNeighbours = "Mixed method";
                waitForResponse();
                break;
            case 5:
                int numberOfTests;
                do{
                    system("CLS");
                    cout << "How many tests do you want to run?" << endl;
                    cin >> numberOfTests;
                }while(numberOfTests < 0);
                automaticTest(fileName, time, target, numberOfTests, neighbourhoodMethod, methodOfNeighbours);
                waitForResponse();
            default:
                break;
       }
    }while(answer != 6);
}

void Tester::automaticTest(const string& name, int time, int target, int numberOfTests, int neighbourhoodMethod, const string& methodOfNeighbours) {
    ofstream file((R"(..\output\results_)" + name + "_" + methodOfNeighbours + ".csv"));
    if(file.is_open()) {
        Graph graph(1);
        graph.readGraphDirected(name);
        if (neighbourhoodMethod == 1) {
            file << "Swap two cities" << endl;
        } else if (neighbourhoodMethod == 2)
            file << "Swap two node sub-paths" << endl;
        else if (neighbourhoodMethod == 3)
            file << "Permute a fragment" << endl;
        else
            file << "Mixed method" << endl;

        file << "cost" << ";" << "time" << ";" << "from target" << ";" << target << endl;
        Node node;
        node.cost = INT_MAX;
        for (int i = 0; i < numberOfTests; i++) {
            cout << "Test nr " << i << endl;
            pair<Node, long> result = TabuSearch::TSSolver(graph, time, false, neighbourhoodMethod, target);
            file << result.first.cost << ";" << result.second << ";" << result.first.cost - target << ";" << target << endl;
            if(node.cost > result.first.cost) {
                node = result.first;
            }
        }

        file << "path of best solution" << endl;
        for(int item : node.path)
            file << item << endl;
        file << node.path[0] << endl;
        file.close();
    }
}

void Tester::waitForResponse() {
    cout << endl << endl << "press ENTER to continue..." << endl;
    cin.ignore(10000, '\n');
    getchar();
}

