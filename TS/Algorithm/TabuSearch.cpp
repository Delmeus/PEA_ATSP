//
// Created by Antek on 21.11.2023.
//

#include <random>
#include "TabuSearch.h"
#include "iostream"
#include "../utils/Timer.h"
#include "TabuManager.h"

int INTERVAL = 1;
//int TABU_TIME_INCREASE_INTERVAL = 100;
//int TABU_TIME_LIMIT = 10;
//int ALLOW_WORSE_SOLUTION_INTERVAL = 300;
//int ALLOW_WORSE_SOLUTION_ITERATION = 20;
//int DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL = 5000;
//int RANDOM_SOLUTION_INTERVAL = 10000;

//struct TabuElement{
//    TabuElement(const pair<int, int> &move, int time);
//
//    pair<int, int> move;
//    int time;
//};

//TabuElement::TabuElement(const pair<int, int> &move, int time) : move(move), time(time) {}

//vector<TabuElement> tabuList;
TabuManager params(100, 10, 300, 20, 5000, 10000);
long TabuSearch::TSSolver(Graph &graph, int numberOfIterations, bool print) {
    params.updateParameters(0);
    Timer timer;
    timer.start();
    Node currentSolution = randomSolution(graph);
    Node bestSolution = currentSolution;
    int timeSinceChange = 0, iteration = 0, bestCost = bestSolution.cost;
    printNode(currentSolution);
    vector<Node> neighbours;
    while(INTERVAL < numberOfIterations && timer.mili() < 60 * 1000 * 10){
        /*
         * if there was an improvement
         */
        if(bestCost > bestSolution.cost){
            cout << "FOUND BETTER SOLUTION -> " << bestSolution.cost << endl;
            bestCost = bestSolution.cost;
            timeSinceChange = 0;
        }
        /*
         * Make adjustments to search parameters if stuck on the same result
         * or if algorithm just found a better solution
         */
        if((timeSinceChange > 100 && timeSinceChange %  params.TABU_TIME_INCREASE_INTERVAL == 0) || timeSinceChange == 0)
            params.updateParameters(timeSinceChange);
        /*
         * check if it's time to
         */
        vector<Node> newNeighbours;
        if(INTERVAL %  params.DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL == 0) {
            params.tabuList.clear();
            if(print)
                cout << "BROUGHT BACK BEST SOLUTION" << endl;
            newNeighbours = defineNeighbours(graph, bestSolution, iteration,  params.TABU_TIME_LIMIT, bestSolution, print);
        }
        else
            newNeighbours = defineNeighbours(graph, currentSolution, iteration,  params.TABU_TIME_LIMIT, bestSolution, print);
        neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());
//        cout << "===================" << endl <<"Possible neighbours = " << neighbours.size() <<
//                "   Tabu size = " <<   params.tabuList.size() << endl;
        currentSolution = findSolution(currentSolution, neighbours, bestSolution);

        if(currentSolution.cost < bestSolution.cost)
            bestSolution = currentSolution;

        params.decreaseTime();

        iteration++;
        INTERVAL++;
        timeSinceChange++;
        if(INTERVAL % 100 == 0 && print)
            cout << INTERVAL << " best sol cost = " << bestSolution.cost << " current cost = " << currentSolution.cost << endl;

        timer.stop();
    }
    timer.stop();
    cout << "Solution found in " << timer.mili() << " ms" << endl;
    printNode(bestSolution);
    return timer.micro();
}

TabuSearch::Node TabuSearch::findSolution(const TabuSearch::Node& currentSolution, vector<Node>& neighbours, Node &bestSolution) {
    int position = -2;
    int bestCost = INT_MAX;
    Node localSolution = currentSolution;
    Node solution;
    bool inTabu;
    do{
        bool aspiration = false, betterThanCurrent = false;
        for (int i = neighbours.size() - 1; i >= 0; i--) {
            if (neighbours[i].cost < bestCost){
                localSolution = neighbours[i];
                bestCost = localSolution.cost;
                position = i;
                if (bestCost < currentSolution.cost)
                    betterThanCurrent = true;
            }
        }

        inTabu =  params.isForbidden(localSolution.move);

        if(inTabu){
            if(bestCost < bestSolution.cost) // -100
                aspiration = true;
        }

        if((aspiration || !inTabu) && betterThanCurrent){
            if(localSolution.cost < bestSolution.cost)
                bestSolution = localSolution;
            return localSolution;
        }
        if(position != -2) {
            neighbours.erase(neighbours.begin() + position);
        }

    }while(!neighbours.empty());
    return localSolution;
}

vector<TabuSearch::Node> TabuSearch::defineNeighbours(Graph &graph, const TabuSearch::Node& currentSolution, int timeSinceChange, int tabuTime, Node &bestSolution, bool print) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);
    vector<Node> neighbours;
    int neighboursQuantity = graph.vertices;

    for(int i = 1; i < 5; i++){
        if(timeSinceChange > i * 2000)
            timeSinceChange *= 2;
        else break;
    }

    for(int i = 0; i < neighboursQuantity; i++){
        Node neighbour = currentSolution;
        int vertex1 = randomVertex(gen);
        int vertex2;
        do{
            vertex2 = randomVertex(gen);
        } while (vertex1 == vertex2);
        pair<int, int> p(vertex1, vertex2);
//
//        if(timeSinceChange % 250 == 0 && i == 0 && timeSinceChange != 0){
//            int vertex3;
//            do{
//                vertex3 = randomVertex(gen);
//            } while (vertex3 == vertex1 || vertex3 == vertex2);
//            neighbour.path.at(vertex1) = currentSolution.path.at(vertex2);
//            neighbour.path.at(vertex2) = currentSolution.path.at(vertex3);
//            neighbour.path.at(vertex3) = currentSolution.path.at(vertex1);
//            neighbour.cost = calculateCost(graph, neighbour);
//            neighbours.push_back(neighbour);
//            tabuList.emplace_back(p, tabuTime);
//            tabuList.emplace_back(make_pair(vertex2,vertex3), tabuTime);
//            if(print)
//                cout << "2 moves made" << endl;
//        }
        if(timeSinceChange % 500 == 0 && timeSinceChange != 0){
            int vertex3, vertex4;
            do{
                vertex3 = randomVertex(gen);
                vertex4 = randomVertex(gen);
            } while (vertex3 == vertex1 || vertex3 == vertex2 || vertex4 == vertex1 || vertex4 == vertex2 || vertex3 == vertex4);
            neighbour.path.at(vertex4) = currentSolution.path.at(vertex1);
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex4);
            neighbour.path.at(vertex3) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex3);
            neighbour.cost = calculateCost(graph, neighbour);
            params.tabuList.emplace_back(make_pair(vertex2,vertex3), tabuTime);
            params.tabuList.emplace_back(make_pair(vertex1,vertex4), tabuTime);
            neighbours.push_back(neighbour);
            if(print)
                cout << "2 independent moves made" << endl;
        }
        else if(timeSinceChange % params.RANDOM_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
            if(print)
                cout << "GENERATED RANDOM SOLUTION" << endl;
            neighbours.push_back(randomSolution(graph));
            params.tabuList.clear();
        }

        else if(! params.isForbidden(p)){
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex1);
            neighbour.cost = calculateCost(graph, neighbour);
            //if (neighbour.cost < currentSolution.cost) {
            neighbour.move.first = vertex1;
            neighbour.move.second = vertex2;
            if(neighbour.cost < bestSolution.cost) {
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime * 2);
            }
            else if(neighbour.cost < currentSolution.cost) {
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime);
            }
            else if(timeSinceChange %   params.ALLOW_WORSE_SOLUTION_INTERVAL == 0 && i %   params.ALLOW_WORSE_SOLUTION_ITERATION == 0 && timeSinceChange != 0){
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime/2);
            }
        }
        else if(neighbour.cost < bestSolution.cost){ // -100
            neighbours.push_back(neighbour);
            params.tabuList.emplace_back(p, tabuTime * 3);
        }
    }
    return neighbours;
}


TabuSearch::Node TabuSearch::randomSolution(Graph &graph) {
    vector<bool> visited;
    visited.resize(graph.vertices);
    Node currentNode;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);

    int previousVertex = randomVertex(gen);
    visited[previousVertex] = true;
    currentNode.path.push_back(previousVertex);
    while(currentNode.path.size() < graph.vertices){
        int currentVertex = randomVertex(gen);
        if(visited[currentVertex] || currentVertex == previousVertex)
            continue;
        currentNode.cost += graph.edges[previousVertex][currentVertex];
        currentNode.path.push_back(currentVertex);
        visited[currentVertex] = true;
        previousVertex = currentVertex;
    }
    currentNode.move.first = -1;
    currentNode.move.second = -1;
    return currentNode;
}


void TabuSearch::printNode(TabuSearch::Node node) {
    for(auto vertex : node.path){
        cout << vertex << " -> ";
    }
    cout << node.path[0] << endl << "cost = " << node.cost << endl;
}


int TabuSearch::calculateCost(Graph &graph, TabuSearch::Node node) {
    int cost = 0;
    int previousVertex = node.path[0];
    for(auto vertex : node.path){
        if(vertex == previousVertex)
            continue;
        cost += graph.edges[previousVertex][vertex];
        previousVertex = vertex;
    }
    cost += graph.edges[node.path.back()][node.path[0]];
    return cost;
}


//bool TabuSearch::isForbidden(pair<int, int> p) {
//
//    for(auto element : tabuList){
//        if(element.move.first == p.first && element.move.second == p.second)
//            return true;
//    }
//    return false;
//}

/*
void TabuSearch::updateParameters(int timeSinceChange) {
    if (timeSinceChange == 0) {
        TABU_TIME_LIMIT = 10;
        RANDOM_SOLUTION_INTERVAL = 10000;
        DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL = 1000;
        ALLOW_WORSE_SOLUTION_INTERVAL = 200;
        ALLOW_WORSE_SOLUTION_ITERATION = 20;


    } else {
        if (TABU_TIME_LIMIT < 300) {
            TABU_TIME_LIMIT += 10;
            for (int j = tabuList.size() - 1; j >= 0; j--) {
                tabuList[j].time += 10;
            }
        }
        if(timeSinceChange > 1000 && timeSinceChange < 20000 && timeSinceChange % 1000 == 0) {
            if (timeSinceChange > 2000 && RANDOM_SOLUTION_INTERVAL < 10000)
                RANDOM_SOLUTION_INTERVAL += 50;
            if (timeSinceChange > 2000 && DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL > 600)
                DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL -= 50;
            if (ALLOW_WORSE_SOLUTION_INTERVAL > 50) {
                ALLOW_WORSE_SOLUTION_INTERVAL -= 50;
                if (ALLOW_WORSE_SOLUTION_ITERATION > 5) {
                    ALLOW_WORSE_SOLUTION_ITERATION--;
                }
            }
        }
        else if(timeSinceChange % 1000 == 0){
            if(RANDOM_SOLUTION_INTERVAL > 700)
                RANDOM_SOLUTION_INTERVAL -= 50;
            if (timeSinceChange > 2000 && DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL < 10000)
                DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL += 50;
            if (ALLOW_WORSE_SOLUTION_INTERVAL > 50) {
                ALLOW_WORSE_SOLUTION_INTERVAL -= 50;
                if (ALLOW_WORSE_SOLUTION_ITERATION > 5) {
                    ALLOW_WORSE_SOLUTION_ITERATION--;
                }
            }
        }

    }
//    cout    << "----Current parameters----" << endl
//            << "Tabu time limit = " << TABU_TIME_LIMIT << endl
//            << "Allow worse solution = " << ALLOW_WORSE_SOLUTION_INTERVAL << endl
//            << "Allow worse sol iteration = " << ALLOW_WORSE_SOLUTION_ITERATION << endl
//            << "Define best sol neighbours = " << DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL << endl
//            << "Random sol = " << RANDOM_SOLUTION_INTERVAL << endl
//            << "Time since change  = " << timeSinceChange << endl
//            << "--------------------------" << endl;
}*/

