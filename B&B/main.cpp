#include <iostream>
#include "algorithm/BBSolver.h"
#include "utils/FileRandomizer.h"

using namespace std;

void waitForResponse();

int main() {
    /*
     * graph automatically re-assigns it's size based on file
     */
    Graph graph(1);
    int answer;
    string name;
    do{
        cout    << "ATSP Branch and Bound algorithm test" << endl
                << "1. Load graph from file" << endl
                << "2. Generate random graph" << endl
                << "3. Solve" << endl
                << "4. Display graph" << endl
                << "5. Exit program" << endl;
        cin >> answer;
        switch (answer) {
            case 1:{
                cout << "Provide filename: ";
                cin >> name;
                graph.readGraphDirected(name);
                waitForResponse();
                system("CLS");
                break;
            }
            case 2:{
                cout << "Provide size: ";
                int size;
                cin >> size;
                cout << "New graph generated" << endl;
                name = "random.atsp";
                FileRandomizer::randomize(size);
                graph.readGraphDirected(name);
                waitForResponse();
                system("CLS");
                break;
            }
            case 3:{
                BBSolver solver;
                int city;
                do{
                    cout << "Provide starting city (remember about size): ";
                    cin >> city;
                }while(city >= graph.vertices);
                solver.solve(graph, city);
                /*
                 * solver changes graph so it is re-read it form file
                 */
                graph.readGraphDirected(name);
                waitForResponse();
                system("CLS");
                break;
            }
            case 4:{
                graph.display();
                waitForResponse();
                system("CLS");
                break;
            }
            default:
                break;
        }
    }while(answer != 5);

    return 0;
}

void waitForResponse(){
    std::cout << std::endl << std::endl << "press ENTER to continue..." << std::endl;
    std::cin.ignore(10000, '\n');
    getchar();
}