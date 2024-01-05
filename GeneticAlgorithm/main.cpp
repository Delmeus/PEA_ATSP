#include <iostream>
#include <filesystem>
#include "structures/Graph.h"
#include "algorithm/GeneticAlgorithm.h"

using namespace std;

int main(int argc, char *argv[]) {
    /*
     * To use with mlflow to automate
     */
    if(argc > 1){
        int size = stoi(argv[2]);
        int time = stoi(argv[4]);
        double mutationFactor = stod(argv[6]);
        double crossoverFactor = stod(argv[8]);
        // fileNumber = stoi(argv[10]);
        //int crossoverMethod = stoi(argv[12]);
        int mutationMethod = stoi(argv[14]);
        int target;
        string fileName = argv[10];

        bool orderCrossover;
        if(stoi(argv[12]) == 0)
            orderCrossover = true;
        else
            orderCrossover = false;

        if(crossoverFactor < 0 || crossoverFactor > 1 || mutationFactor < 0 || mutationFactor > 1 || time < 0 || size < 0)
            return 0;

        Graph graph(1);

        if(graph.readGraphDirectedAbsolute(fileName) !=0){
            cerr << "Couldn't open the given file" << endl;
            return -1;
        }

        GeneticAlgorithm::start(size, time, mutationFactor, crossoverFactor, graph, target, orderCrossover, false);

        return 0;
    }
    /*
     * manual tester
     */
    int answer, time = 60, target, size = 100;
    double crossoverFactor = 0.8;
    double mutationFactor = 0.01;
    bool orderCrossover = true;
    string fileName = "ftv47.atsp";
    target = 1776;
    Graph graph(1);
    graph.readGraphDirected(fileName);
    do{
        string method;
        if(orderCrossover)
            method = "order crossover";
        else
            method = "pmx";

        system("CLS");
        cout << "--------------Genetic algorithm--------------" << endl
             << "Current settings: " << endl
             << "Time = " << time << " s, " << "target cost = " << target << ", file = " << fileName << endl
             << "Population size = " << size << ", mutation factor = " << mutationFactor << endl
             << "Crossover factor = " << crossoverFactor << ", crossover method = " << method << endl
             << "------------------MENU------------------" << endl
             << "1. Load graph" << endl
             << "2. Display graph" << endl
             << "3. Change stop condition" << endl
             << "4. Change initial population size" << endl
             << "5. Change mutation factor" << endl
             << "6. Change crossover factor" << endl
             << "7. Change mutation method"  << endl
             << "8. Change crossover method" << endl
             << "9. Start algorithm" << endl
             << "10. Quit program" << endl;
        cin >> answer;
        switch(answer){
            case 1:
                do{
                    cout << "What file do you wish to load?\n1. ftv47.atsp\n2. ftv170.atsp\n3. rbg403.atsp" << endl;
                    cin >> answer;
                } while(answer < 1 || answer > 4);
                if(answer == 1) {
                    fileName = "ftv47.atsp";
                    target = 1776;
                }
                else if(answer == 2) {
                    fileName = "ftv170.atsp";
                    target = 2755;
                }
                else if(answer == 3) {
                    fileName = "rbg403.atsp";
                    target = 2465;
                }
                else
                    fileName = "test.atsp";
                graph.readGraphDirected(fileName);
                system("PAUSE");
                system("CLS");
                break;
            case 2:
                graph.display();
                system("PAUSE");
                system("CLS");
                break;
            case 3:
                do {
                    system("CLS");
                    cout << "How long do you want the algorithm to go on for? (seconds): ";
                    cin >> time;
                    system("PAUSE");
                }while(time < 0 || time > 1200);
                break;
            case 4:
                do{
                    system("CLS");
                    cout << "How large do you want the starting population to be: ";
                    cin >> size;
                    system("PAUSE");
                }while(size < 2);
                break;
            case 5:
                do {
                    system("CLS");
                    cout << "What do you want the mutation factor to be: ";
                    cin >> mutationFactor;
                    system("PAUSE");
                }while(mutationFactor <= 0 || mutationFactor > 1);
                break;
            case 6:
                do {
                    system("CLS");
                    cout << "What do you want the crossover factor to be: ";
                    cin >> crossoverFactor;
                    system("PAUSE");
                }while(crossoverFactor <= 0 || crossoverFactor > 1);
                break;
            case 8:
                orderCrossover = !orderCrossover;
                break;
            case 9:
                cout << endl;
                GeneticAlgorithm::start(size, time, mutationFactor, crossoverFactor, graph, target, orderCrossover, true);
                system("PAUSE");
                system("CLS");
                break;
            default:
                break;
        }
    }while(answer != 10);
}
