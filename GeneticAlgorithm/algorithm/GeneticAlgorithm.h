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
    void start(int populationSize, long stopCondition);
    Node crossover(Node parent1, Node parent2);
};


#endif //GENETICALGORITHM_GENETICALGORITHM_H
