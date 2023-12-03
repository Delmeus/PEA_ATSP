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

Node Node::firstSolution(Graph &graph) {
    Node solution;
    vector<bool> visited(graph.vertices, false);
    for(int i = 0; i < graph.vertices; i++){
        pair<int, int> bestCost;
        bestCost.first = INT_MAX;
        for(int j = 0; j < graph.vertices; j++){
            if(graph.edges[i][j] < bestCost.first && !visited[j] && i != j){
                bestCost.first = graph.edges[i][j];
                bestCost.second = j;
            }
        }
        solution.path.push_back(bestCost.second);
        visited[bestCost.second] = true;
    }
    solution.cost = Node::calculateCost(graph, solution);
    solution.move.first = -1;
    solution.move.second = -1;
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

        if(inTabu && bestCost < bestSolution.cost)
                aspiration = true;

        if((aspiration || !inTabu) && betterThanCurrent){
            if(localSolution.cost < bestSolution.cost)
                bestSolution = localSolution;
            return localSolution;
        }

        if(inTabu && localSolution.cost < solution.cost)
            solution = localSolution;

        if(position != -2) {
            neighbours.erase(neighbours.begin() + position);
        }

    }while(!neighbours.empty());

    if(solution.cost < localSolution.cost)
        return solution;
    return localSolution;
}
