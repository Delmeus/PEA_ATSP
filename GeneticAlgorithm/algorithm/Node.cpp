//
// Created by antek on 31.12.2023.
//

#include <random>
#include <algorithm>
#include "Node.h"
#include "iostream"

Node Node::scrambleMutate() {
    //TODO consider dynamic number of changes
    int numberOfChanges = (int) chromosome.size() / 10 + 2;
    vector<int> positions;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomVertex(0, (int) chromosome.size() - 1);
    /*
     * Choose positions which will be changed
     */
    do{
        int position = randomVertex(gen);
        bool valid = true;
        for (int element : positions){
            if (element == position) {
                valid = false;
                break;
            }
        }
        if (valid)
            positions.push_back(position);
    } while ((int)positions.size() < numberOfChanges);
    /*
     * Swap positions randomly
     */
    vector<int> shuffled;
    shuffled = positions;
    uniform_int_distribution<> randomPosition(0, numberOfChanges - 1);
    Node mutatedNode;
    mutatedNode.chromosome = this->chromosome;
    do {
        shuffle(shuffled.begin(), shuffled.end(), std::mt19937(std::random_device()()));
    }while(equal(positions.begin(), positions.end(), shuffled.begin()));
    for(int i = 0; i < numberOfChanges; i++){
        mutatedNode.chromosome[positions[i]] = this->chromosome[shuffled[i]];
    }
    return mutatedNode;
}

Node Node::greedySolution(const Graph &graph){
    Node solution;
    vector<bool> visited(graph.vertices, false);
    int currentVertex = 0;
    solution.chromosome.push_back(currentVertex);
    visited[currentVertex] = true;

    for (int i = 1; i < graph.vertices; ++i) {
        int nearestNeighbor = findNearestNeighbour(graph, currentVertex, visited);
        solution.chromosome.push_back(nearestNeighbor);
        visited[nearestNeighbor] = true;
        currentVertex = nearestNeighbor;
    }
    solution.calculateCost(graph);
    return solution;
}

Node Node::generateRandomNode(const Graph& graph) {
    vector<bool> visited;
    visited.resize(graph.vertices);
    Node currentNode;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);

    int previousVertex = randomVertex(gen);
    visited[previousVertex] = true;
    currentNode.chromosome.push_back(previousVertex);
    while(currentNode.chromosome.size() < graph.vertices){
        int currentVertex = randomVertex(gen);
        if(visited[currentVertex] || currentVertex == previousVertex)
            continue;
        currentNode.chromosome.push_back(currentVertex);
        visited[currentVertex] = true;
        previousVertex = currentVertex;
    }
    currentNode.calculateCost(graph);
    return currentNode;
}

void Node::printNode() {
    for(auto vertex : chromosome){
        cout << vertex << " -> ";
    }
    cout << chromosome[0] << endl << "cost = " << cost << endl;
}

void Node::calculateCost(const Graph& graph) {
    cost = 0;
    int previousVertex = chromosome[0];
    for(auto vertex : chromosome){
        if(vertex == previousVertex)
            continue;
        cost += graph.edges[previousVertex][vertex];
        previousVertex = vertex;
    }
    cost += graph.edges[chromosome.back()][chromosome[0]];
}

Node Node::selectParent(const vector<Node>& population) {
    if(population.empty())
        return *this;

    std::random_device rd;
    std::mt19937 gen(rd());

    double totalFitness = 0.0;
    for (const auto& node : population) {
        totalFitness += node.fitness;
    }

    std::uniform_real_distribution<> distribution(0.0, totalFitness);
    double randomValue = distribution(gen);
    double cumulativeFitness = 0.0;

    for (const auto& node : population) {
        cumulativeFitness += node.fitness;
        if (cumulativeFitness >= randomValue && equal(node.chromosome.begin(), node.chromosome.end(), chromosome.begin())) {
            return node;
        }
    }

    return population.front();
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

Node Node::mutate(bool mutationMethod) {
    if(mutationMethod)
        return scrambleMutate();
    return inversionMutate();
}

Node Node::inversionMutate() {
    int fragmentSize = (int) chromosome.size() / 10 + 2;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomVertex(0, (int)chromosome.size() - fragmentSize - 1);
    int start = randomVertex(gen);
    Node mutatedNode;
    mutatedNode.chromosome = this->chromosome;
    reverse(mutatedNode.chromosome.begin() + start, mutatedNode.chromosome.begin() + start + fragmentSize);
    return mutatedNode;
}

