//
// Created by Antek on 21.11.2023.
//

#include <random>
#include "TabuSearch.h"
#include "iostream"
#include "../utils/Timer.h"

int INTERVAL = 1;
int TABU_INCREASE_INTERVAL = 100;
int RANDOM_SOLUTION_INTERVAL = 3000;
int DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL = 5000;

struct TabuElement{
    TabuElement(const pair<int, int> &move, int time);

    pair<int, int> move;
    int time;
};

TabuElement::TabuElement(const pair<int, int> &move, int time) : move(move), time(time) {}

vector<TabuElement> tabuList;

long TabuSearch::TSSolver(Graph &graph, int numberOfIterations) {

    Timer timer;
    timer.start();
    Node currentSolution = randomSolution(graph);
    Node bestSolution = currentSolution;
    int tabuTimeLimit = 10, timeSinceChange = 0, iteration = 0, bestCost = bestSolution.cost;
    printNode(currentSolution);
    vector<Node> neighbours;
    while(iteration < numberOfIterations && timer.mili() < 60 * 1000 * 5){
        if(bestCost != bestSolution.cost){
            bestCost = bestSolution.cost;
            INTERVAL = 1;
            if(tabuTimeLimit > 10) {
                cout << "Decreasing tabu time" << endl;
                tabuTimeLimit -= 10;
            }
        }
        if(INTERVAL % TABU_INCREASE_INTERVAL == 0 && bestCost == bestSolution.cost){
            tabuTimeLimit += 10;
            cout << "Increasing tabu time" << endl;
            for (int j = tabuList.size() - 1; j >= 0; j--) {
                tabuList[j].time += 10;
            }
            INTERVAL = 1;
        }
        vector<Node> newNeighbours;
        if(INTERVAL % DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL == 0) {
            tabuList.clear();
            newNeighbours = defineNeighbours(graph, bestSolution, iteration, tabuTimeLimit, bestSolution);
        }
        else
            newNeighbours = defineNeighbours(graph, currentSolution, iteration, tabuTimeLimit, bestSolution);
        neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());
        currentSolution = findSolution(currentSolution, neighbours, bestSolution);
        if(currentSolution.cost < bestSolution.cost)
            bestSolution = currentSolution;

        for (int j = tabuList.size() - 1; j >= 0; j--) {
            tabuList[j].time--;
            if(tabuList[j].time <= 0)
                tabuList.erase(tabuList.begin() + j);
        }

        iteration++;
        INTERVAL++;
        cout << iteration << " best sol cost = " << bestSolution.cost << " current cost = " << currentSolution.cost << endl;

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

        inTabu = isForbidden(localSolution.move);

        if(inTabu){
            if(bestCost < bestSolution.cost - 100)
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

vector<TabuSearch::Node> TabuSearch::defineNeighbours(Graph &graph, const TabuSearch::Node& currentSolution, int iteration, int tabuTime, Node &bestSolution) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);
    vector<Node> neighbours;
    for(int i = 0; i < graph.vertices; i++){
        Node neighbour = currentSolution;
        int vertex1 = randomVertex(gen);
        int vertex2;
        do{
            vertex2 = randomVertex(gen);
        } while (vertex1 == vertex2);
        pair<int, int> p(vertex1, vertex2);
        /*
         * every 250 iterations make two moves
         */
        if(INTERVAL % 250 == 0){
            int vertex3;
            do{
                vertex3 = randomVertex(gen);
            } while (vertex3 == vertex1 || vertex3 == vertex2);
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex3);
            neighbour.path.at(vertex3) = currentSolution.path.at(vertex1);
            neighbour.cost = calculateCost(graph, neighbour);
            neighbours.push_back(neighbour);
            tabuList.emplace_back(p, tabuTime);
            tabuList.emplace_back(make_pair(vertex2,vertex3), tabuTime);
            //tabuList.clear();
        }
        /*
        * every 500 iterations make two independent moves
        */
        else if(INTERVAL % 500 == 0){
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
            tabuList.emplace_back(make_pair(vertex2,vertex3), tabuTime);
            tabuList.emplace_back(make_pair(vertex1,vertex4), tabuTime);
            neighbours.push_back(neighbour);
            //tabuList.clear();
        }
        else if(INTERVAL % RANDOM_SOLUTION_INTERVAL == 0){
            neighbours.push_back(randomSolution(graph));
            tabuList.clear();
        }
        else if(!isForbidden(p)){
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex1);
            neighbour.cost = calculateCost(graph, neighbour);
            //if (neighbour.cost < currentSolution.cost) {
            neighbour.move.first = vertex1;
            neighbour.move.second = vertex2;
            if(neighbour.cost > currentSolution.cost + 500)
                continue;
            neighbours.push_back(neighbour);
            if(neighbour.cost < bestSolution.cost)
                tabuList.emplace_back(p, tabuTime * 4);
            else if(neighbour.cost < currentSolution.cost)
                tabuList.emplace_back(p, tabuTime * 2);
            else
                tabuList.emplace_back(p, tabuTime / 2);
            //}
        }
        else if(neighbour.cost < bestSolution.cost - 100){
            neighbours.push_back(neighbour);
            tabuList.emplace_back(p, tabuTime * 3);
        }
    }
    return neighbours;
}

// done
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

// done
void TabuSearch::printNode(TabuSearch::Node node) {
    for(auto vertex : node.path){
        cout << vertex << " -> ";
    }
    cout << node.path[0] << endl << "cost = " << node.cost << endl;
}

// done
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

//done
bool TabuSearch::isForbidden(pair<int, int> p) {

    for(auto element : tabuList){
        if(element.move.first == p.first && element.move.second == p.second)
            return true;
    }
    return false;
}

int TabuSearch::stopCondition(Graph &graph) {
    int value = 0;
    for(int i = 0; i < graph.vertices; i++){
        int smallest = INT_MAX;
        for(int j = 0; j < graph.vertices; j++){
            if(graph.edges[i][j] < smallest && i != j) {
                smallest = graph.edges[i][j];
            }

        }
        value += smallest;
    }

    for(int i = 0; i < graph.vertices; i++){
        int smallest = INT_MAX;
        int row;
        for(int j = 0; j < graph.vertices; j++){

            if(graph.edges[j][i] == 0 && j != i){
                row = -1;
                break;
            }

            if(graph.edges[j][i] < smallest && j != i){
                smallest = graph.edges[j][i];
                row = j;
            }
        }

        if(row != -1)
            value += smallest;
    }

    return value;
}

