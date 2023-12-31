//
// Created by antek on 31.12.2023.
//

#include "GeneticAlgorithm.h"

void GeneticAlgorithm::start(int populationSize, long stopCondition, const Graph& graph) {
    Node node;
    node = Node::generateRandomNode(graph);
    node.printNode();
    Node newNode = node.scrambleMutate();
    newNode.printNode();

}

Node GeneticAlgorithm::crossover(Node parent1, Node parent2) {
    return Node();
}
