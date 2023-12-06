//
// Created by Antek on 21.11.2023.
//

#include <random>
#include "TabuSearch.h"
#include "iostream"
#include "../utils/Timer.h"
#include "TabuManager.h"

TabuManager params(1000, 5, 300, 20, 5000, 10000);

pair<Node, long> TabuSearch::TSSolver(Graph &graph, int algorithmTime, bool print, int neighbourhoodMethod) {
    if(neighbourhoodMethod < 0 || neighbourhoodMethod > 4 || algorithmTime < 0 || graph.vertices < 4)
        throw invalid_argument("Received wrong arguments");

    params.updateParameters(0);

    Node currentSolution = Node::firstSolution(graph);
    Node bestSolution = currentSolution;
    vector<Node> neighbours;

    Node::printNode(currentSolution);

    long INTERVAL = 1;
    long bestSolutionTime;
    int timeSinceChange = 0, bestCost = bestSolution.cost;

    Timer timer;
    timer.start();

    while(timer.mili() < algorithmTime * 60 * 1000){
        /*
         * if there was an improvement
         */
        if(bestCost > bestSolution.cost){
            cout << "FOUND BETTER SOLUTION -> " << bestSolution.cost << endl;
            bestCost = bestSolution.cost;
            timeSinceChange = 0;
            timer.stop();
            bestSolutionTime = timer.mili();
            params.updateParameters(timeSinceChange);
        }
        /*
         * Make adjustments to search parameters if stuck on the same result
         * or if algorithm just found a better solution
         */
        if(timeSinceChange % params.TABU_TIME_INCREASE_INTERVAL == 0 || timeSinceChange == 0)
            params.updateParameters(timeSinceChange);
        /*
         * define neighbours
         */
        neighbours = defineNeighbours(graph, currentSolution, timeSinceChange, bestSolution, neighbourhoodMethod, INTERVAL);

        if(INTERVAL % 10000 == 0 && print) {
            cout    << INTERVAL << "\tbest cost = " << bestSolution.cost << " current cost = " << currentSolution.cost << endl
                    << "\ttabu size = " << params.tabuList.size() << " possible neighbours = " << neighbours.size() << endl;
        }
        /*
         * find best solution among neighbours
         */
        int previousCost = currentSolution.cost;
        currentSolution = Node::findSolution(currentSolution, neighbours, bestSolution, params);

        if(currentSolution.cost < bestSolution.cost)
            bestSolution = currentSolution;
        else if(currentSolution.cost < previousCost){
            params.emplaceInTabu(currentSolution.move.first, currentSolution.move.second, -1, -1, params.TABU_TIME_LIMIT);
            params.emplaceInTabu(currentSolution.move.second, currentSolution.move.first, -1, -1, params.TABU_TIME_LIMIT);
        }
        else{
            params.emplaceInTabu(currentSolution.move.first, currentSolution.move.second, -1, -1, params.TABU_TIME_LIMIT * 2);
            params.emplaceInTabu(currentSolution.move.second, currentSolution.move.first, -1, -1, params.TABU_TIME_LIMIT * 2);
        }

        params.decreaseTime();
        INTERVAL++;
        timeSinceChange++;

        timer.stop();
    }
    timer.stop();
    cout << "Solution found in " << bestSolutionTime << " ms" << endl;
    Node::printNode(bestSolution);
    return make_pair(bestSolution, bestSolutionTime);
}

vector<Node> TabuSearch::swapTwoCities(Graph &graph, const Node& currentSolution, int timeSinceChange) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);

    vector<Node> neighbours;
    int neighboursQuantity = graph.vertices;

    if(timeSinceChange > 20000)
        neighboursQuantity *= 2;

    Node bestLegalNeighbour;
    bestLegalNeighbour.cost = INT_MAX;
    for(int i = 0; i < neighboursQuantity; i++){
        /*
         * generate a pair of vertices
         */
        Node neighbour = currentSolution;
        int vertex1 = randomVertex(gen);
        int vertex2;
        do{
            vertex2 = randomVertex(gen);
        } while (vertex1 == vertex2);
        /*
         * make two independent moves if time since change was long enough
         */
        if(timeSinceChange % 500 == 0 && timeSinceChange != 0 && i % 10 == 0){
            int vertex3, vertex4;
            do{
                vertex3 = randomVertex(gen);
                vertex4 = randomVertex(gen);
            } while (vertex3 == vertex1 || vertex3 == vertex2 || vertex4 == vertex1 || vertex4 == vertex2 || vertex3 == vertex4);
            neighbour.path.at(vertex4) = currentSolution.path.at(vertex1);
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex4);
            neighbour.path.at(vertex3) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex3);
            neighbour.move.first = vertex1;
            neighbour.move.second = vertex4;
            neighbour.cost = Node::calculateCost(graph, neighbour);
            neighbours.push_back(neighbour);
        }
        /*
        * generate random solution if time since change was long enough
        */
        else if(timeSinceChange % params.RANDOM_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
            neighbours.push_back(Node::randomSolution(graph));
            params.tabuList.clear();
        }
        /*
        * add neighbour to list
        */
        else{
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex1);
            neighbour.cost = Node::calculateCost(graph, neighbour);
            neighbour.move.first = vertex1;
            neighbour.move.second = vertex2;
            neighbours.push_back(neighbour);

        }
    }

    return neighbours;
}

vector<Node> TabuSearch::subPaths(Graph &graph, const Node &currentSolution, int timeSinceChange) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);

    vector<Node> neighbours;
    int neighboursQuantity = graph.vertices;

    if(timeSinceChange > 20000)
        neighboursQuantity *= 2;

    Node bestLegalNeighbour;
    bestLegalNeighbour.cost = INT_MAX;
    for(int i = 0; i < neighboursQuantity; i++){
        /*
         * generate two random vertices and create sub-paths including them
         */
        Node neighbour = currentSolution;
        int vertex1 = randomVertex(gen);
        int vertex2;
        do{
            vertex2 = randomVertex(gen);
        } while (vertex1 == vertex2 + 1 || vertex1 == vertex2 - 1 || vertex1 == vertex2 || vertex1 == vertex2 + 2 || vertex1 == vertex2 - 2);
        pair<int, int> p;
        if(vertex1 > vertex2)
            p = make_pair(vertex1 - 1, vertex2);
        else
            p = make_pair(vertex2 - 1, vertex1);
        /*
        * generate random solution if time since change was long enough
        */
        if(timeSinceChange % params.RANDOM_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
            neighbours.push_back(Node::randomSolution(graph));
            params.tabuList.clear();
        }
        /*
        * add neighbour to list
        */
        else {
            if (vertex1 > vertex2) {
                neighbour.path.at(vertex2) = currentSolution.path.at(vertex1 - 1);
                neighbour.path.at(vertex2 + 1) = currentSolution.path.at(vertex1);
                neighbour.path.at(vertex1) = currentSolution.path.at(vertex2 + 1);
                neighbour.path.at(vertex1 - 1) = currentSolution.path.at(vertex2);
            } else {
                neighbour.path.at(vertex1) = currentSolution.path.at(vertex2 - 1);
                neighbour.path.at(vertex1 + 1) = currentSolution.path.at(vertex2);
                neighbour.path.at(vertex2) = currentSolution.path.at(vertex1 + 1);
                neighbour.path.at(vertex2 - 1) = currentSolution.path.at(vertex1);
            }

            neighbour.cost = Node::calculateCost(graph, neighbour);
            neighbour.move.first = p.first;
            neighbour.move.second = p.second;
            neighbours.push_back(neighbour);
        }
    }
    return neighbours;
}

vector<Node> TabuSearch::permuteFragment(Graph &graph, const Node &currentSolution, int timeSinceChange) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 6);
    vector<Node> neighbours;
    /*
     * generate a random vertex and get a fragment of the current solution
     */
    Node bestLegalNeighbour, neighbour = currentSolution;
    bestLegalNeighbour.cost = INT_MAX;
    int vertex;
    int tries = 0;
    pair<int, int> p;
    do {
        vertex = randomVertex(gen);
        tries++;
        p = make_pair(vertex, vertex + tries % 4 + 2);
    }while(params.isForbidden(p) && tries < 100);
    if(!params.isForbidden(p)) {
        vector<int> fragment(currentSolution.path.begin() + vertex, currentSolution.path.begin() + vertex + tries % 4 + 2 + 1);
        sort(fragment.begin(), fragment.end());
        do {
            copy(fragment.begin(), fragment.end(), neighbour.path.begin() + vertex);
            neighbour.move = p;
            neighbour.cost = Node::calculateCost(graph, neighbour);
            /*
             * to avoid adding too many neighbours
             */
            neighbours.push_back(neighbour);
        } while (next_permutation(fragment.begin(), fragment.end()));
    }
    return neighbours;
}
/*
 * choose proper function depending on choice of method
 */
vector<Node> TabuSearch::defineNeighbours(Graph &graph, const Node &currentSolution, int timeSinceChange,
                                          Node &bestSolution, int method, long iteration) {
    vector<Node> newNeighbours;
    if(timeSinceChange %  params.DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL == 0) {
        params.tabuList.clear();
        if(method == 1)
            newNeighbours = swapTwoCities(graph, bestSolution, timeSinceChange);
        else if(method == 2)
            newNeighbours = subPaths(graph, bestSolution, timeSinceChange);
        else if(method == 3)
            newNeighbours = permuteFragment(graph, bestSolution, timeSinceChange);
        else{
            if(iteration % 100000 == 0) {
                params.tabuList.clear();
                params.updateParameters(0);
            }
            if(iteration % 300000 < 100000)
                newNeighbours = swapTwoCities(graph, bestSolution, timeSinceChange);
            else if(iteration % 300000 > 200000)
                newNeighbours = subPaths(graph, bestSolution, timeSinceChange);
            else
                newNeighbours = permuteFragment(graph, bestSolution, timeSinceChange);
        }
    }
    else {
        if(method == 1)
            newNeighbours = swapTwoCities(graph, currentSolution, timeSinceChange);
        else if(method == 2)
            newNeighbours = subPaths(graph, currentSolution, timeSinceChange);
        else if(method == 3)
            newNeighbours = permuteFragment(graph, currentSolution, timeSinceChange);
        else{
            if(iteration % 100000 == 0) {
                params.tabuList.clear();
                params.updateParameters(0);
            }
            if(iteration % 300000 < 100000)
                newNeighbours = swapTwoCities(graph, currentSolution, timeSinceChange);
            else if(iteration % 300000 > 200000)
                newNeighbours = subPaths(graph, currentSolution, timeSinceChange);
            else
                newNeighbours = permuteFragment(graph, currentSolution, timeSinceChange);
        }
    }

    return newNeighbours;
}



