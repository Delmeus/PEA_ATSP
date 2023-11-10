#include <iostream>
#include "algorithm/BBSolver.h"
#include "utils/FileRandomizer.h"
#include "Tester/AutomaticTester.h"

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
                << "3. Solve with BFS" << endl
                << "4. Solve with DFS" << endl
                << "5. Display graph" << endl
                << "6. Automatic test for given size" << endl
                << "7. Automatic test through 3 to 25" << endl
                << "8. Exit program" << endl;
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

                if(size < 3){
                    cout << "Size too small" << endl;
                    waitForResponse();
                    system("CLS");
                    break;
                }

                cout << "New graph generated" << endl;
                name = "random.atsp";
                FileRandomizer::randomize(size);
                graph.readGraphDirected(name);
                waitForResponse();
                system("CLS");
                break;
            }
            case 3:{
                if(graph.vertices < 2){
                    cout << "Graph doesn't exist, try to load it again or generate a random graph" << endl;
                    waitForResponse();
                    system("CLS");
                    break;
                }
                BBSolver solver;
                int city;
                do{
                    cout << "Provide starting city (remember about size): ";
                    cin >> city;
                }while(city >= graph.vertices);
                solver.solveBFS(graph, city);
                /*
                 * solver changes graph, so it is re-read it form file
                 */
                graph.readGraphDirected(name);
                waitForResponse();
                system("CLS");
                break;
            }
            case 4:{
                if(graph.vertices < 2){
                    cout << "Graph doesn't exist, try to load it again or generate a random graph" << endl;
                    waitForResponse();
                    system("CLS");
                    break;
                }
                BBSolver solver;
                int city;
                do{
                    cout << "Provide starting city (remember about size): ";
                    cin >> city;
                }while(city >= graph.vertices);
                solver.solveDFS(graph, city);
                /*
                 * solver changes graph, so it is re-read it form file
                 */
                graph.readGraphDirected(name);
                waitForResponse();
                system("CLS");
                break;
            }
            case 5:{
                graph.display();
                waitForResponse();
                system("CLS");
                break;
            }
            case 6:{
                cout << "Provide size: ";
                int size;
                cin >> size;

                if(size < 3){
                    cout << "Size too small" << endl;
                    waitForResponse();
                    system("CLS");
                    break;
                }

                cout << "Beginning automatic test for size " << size << endl;
                AutomaticTester::test(size);
                cout << "Test complete" << endl;
                waitForResponse();
                system("CLS");
                break;
            }
            case 7:{
                AutomaticTester::test(0);
                answer = 8;
                break;
            }
            default:
                break;
        }
    }while(answer != 8);

    return 0;
}

void waitForResponse(){
    std::cout << std::endl << std::endl << "press ENTER to continue..." << std::endl;
    std::cin.ignore(10000, '\n');
    getchar();
}