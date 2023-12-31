#include <iostream>
#include "structures/Graph.h"
#include "algorithm/GeneticAlgorithm.h"

using namespace std;

int main() {
    int answer, time = 1, target;
    string fileName = "ftv47.atsp";
    target = 1776;
    Graph graph(1);
    graph.readGraphDirected(fileName);
    do{
        system("CLS");
        cout << "--------------Genetic algorithm--------------" << endl
             << "Current settings: " << endl
             << "time = " << time << " s, " << "target cost = " << target << ", file = " << fileName << endl
             << "population size = " << ", mutation factor = " << ", crossover factor = " << endl
             << "------------------MENU------------------" << endl
             << "1. Load graph" << endl
             << "2. Display graph" << endl
             << "3. Change stop condition" << endl
             << "4. Change mutation factor" << endl
             << "5. Change crossover factor" << endl
             << "6. Change mutation method"  << endl
             << "7. Change crossover method" << endl
             << "8. Start algorithm" << endl
             << "9. Quit program" << endl;
        cin >> answer;
        switch(answer){
            case 1:
                do{
                    cout << "What file do you wish to load?\n1. ftv47.atsp\n2. ftv170.atsp\n3. rbg403.atsp" << endl;
                    cin >> answer;
                } while(answer < 1 || answer > 3);
                if(answer == 1)
                    fileName = "ftv47.atsp";
                else if(answer == 2)
                    fileName = "ftv170.atsp";
                else
                    fileName = "rbg403.atsp";
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
                break;
            case 4:
                break;
            case 5:
                break;
            case 8:
                GeneticAlgorithm::start(0,0,graph);
                system("PAUSE");
                system("CLS");
                break;
            default:
                break;
        }
    }while(answer != 9);
}
