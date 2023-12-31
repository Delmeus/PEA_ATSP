//
// Created by antek on 31.12.2023.
//

#include <random>
#include <algorithm>
#include "Node.h"
#include "iostream"

Node Node::scrambleMutate() {
    //TODO consider dynamic number of changes
    int numberOfChanges = (int) chromosome.size() / 10;
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
    shuffle (shuffled.begin(), shuffled.end(), std::mt19937(std::random_device()()));
    for(int i = 0; i < numberOfChanges; i++){
        mutatedNode.chromosome[positions[i]] = this->chromosome[shuffled[i]];
        //cout << positions[i] << "\t" << shuffled[i] << endl;
    }
    return mutatedNode;
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
        currentNode.cost += graph.edges[previousVertex][currentVertex];
        currentNode.chromosome.push_back(currentVertex);
        visited[currentVertex] = true;
        previousVertex = currentVertex;
    }
    return currentNode;
}

void Node::printNode() {
    for(auto vertex : chromosome){
        cout << vertex << " -> ";
    }
    cout << chromosome[0] << endl << "cost = " << cost << endl;
}
