//
// Created by Antek on 02.12.2023.
//

#include <random>
#include "Node.h"
#include "iostream"
#include "TabuManager.h"

int Node::calculateCost(Graph &graph, Node node) {
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

void Node::printNode(Node node) {
    for(auto vertex : node.path){
        cout << vertex << " -> ";
    }
    cout << node.path[0] << endl << "cost = " << node.cost << endl;
}

Node Node::randomSolution(Graph &graph) {
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
/*
 * generate initial solution using greedy method
 */
Node Node::firstSolution(Graph &graph) {
    Node solution;
    vector<bool> visited(graph.vertices, false);
    int currentVertex = 0;
    solution.path.push_back(currentVertex);
    visited[currentVertex] = true;

    for (int i = 1; i < graph.vertices; ++i) {
        int nearestNeighbor = findNearestNeighbour(graph, currentVertex, visited);
        solution.path.push_back(nearestNeighbor);
        visited[nearestNeighbor] = true;
        currentVertex = nearestNeighbor;
    }
    solution.cost = Node::calculateCost(graph, solution);
    return solution;
}

Node Node::findSolution(const Node &currentSolution, vector<Node> &neighbours, Node &bestSolution, TabuManager params) {
    int position = -2;
    int bestCost = INT_MAX;
    Node localSolution = currentSolution;
    Node solution;
    solution.cost = INT_MAX;
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
        /*
         * if current solution is better than best, ignore tabu restriction
         */
        if(bestCost < bestSolution.cost)
                aspiration = true;

        if((aspiration || !inTabu) && betterThanCurrent){
            if(localSolution.cost < bestSolution.cost)
                bestSolution = localSolution;
            return localSolution;
        }

//        if(inTabu && localSolution.cost < solution.cost)
//            solution = localSolution;

        if(position != -2) {
            neighbours.erase(neighbours.begin() + position);
        }

    }while(!neighbours.empty());
//
//    //zastanow sie/potestuj
//    if(solution.cost < localSolution.cost)
//        return solution;
    return localSolution;
}

int Node::findNearestNeighbour(const Graph &graph, int currentVertex, const vector<bool> &visited) {
    int nearestNeighbor = -1;
    int minDistance = std::numeric_limits<int>::max();

    for (int i = 0; i < graph.vertices; ++i) {
        if (!visited[i] && graph.edges[currentVertex][i] < minDistance) {
            nearestNeighbor = i;
            minDistance = graph.edges[currentVertex][i];
        }
    }

    return nearestNeighbor;
}
