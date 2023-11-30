//
// Created by Antek on 29.11.2023.
//

#include <iostream>
#include <fstream>
#include "Tester.h"
#include "../structures/Graph.h"
#include "../Algorithm/TabuSearch.h"

void Tester::test() {
    int answer, time = 1, target;
    string fileName = "ftv47.atsp";
    target = 1776;
    Graph graph(1);
    do{
        system("CLS");
        cout << "--------------Tabu Search--------------" << endl
            << "Current settings: " << endl
            << "time = " << time << " min, " << "file = " << fileName << endl
            << "1. Test algorithm" << endl
            << "2. Load file" << endl
            << "3. Change stop condition" << endl
            << "4. Change how neighbours are defined" << endl
            << "5. Automatic test" << endl
            << "6. Exit program"  << endl;
        cin >> answer;
        switch(answer){
            case 1:
                TabuSearch::TSSolver(graph, time, true);
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
                cout << " ";
                break;
            case 5:
                int numberOfTests;
                do{
                    system("CLS");
                    cout << "How many tests do you want to run?" << endl;
                    cin >> numberOfTests;
                }while(numberOfTests < 0);
                automaticTest(fileName, time, target, numberOfTests);
                waitForResponse();
            default:
                break;
       }
    }while(answer != 6);
}

void Tester::automaticTest(const string& name, int time, int target, int numberOfTests) {
    ofstream file((R"(..\output\results_)" + name + ".csv"));
    Graph graph(1);
    graph.readGraphDirected(name);
    file << "cost" << ";" << "from target" << ";" << target << endl;
    for(int i = 0; i < numberOfTests; i++){
        int result = TabuSearch::TSSolver(graph, time, false);
        file << result << ";" << result - target << ";" << target << endl;
    }
}

void Tester::waitForResponse() {
    cout << endl << endl << "press ENTER to continue..." << endl;
    cin.ignore(10000, '\n');
    getchar();
}
