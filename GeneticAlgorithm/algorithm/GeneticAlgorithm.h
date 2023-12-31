//
// Created by antek on 31.12.2023.
//

#ifndef GENETICALGORITHM_GENETICALGORITHM_H
#define GENETICALGORITHM_GENETICALGORITHM_H

#include "Node.h"

using namespace std;

class GeneticAlgorithm {
private:
    vector<Node> population;

public:
    static void start(int populationSize, long stopCondition,  const Graph& graph);
    Node crossover(Node parent1, Node parent2);
};


#endif //GENETICALGORITHM_GENETICALGORITHM_H
