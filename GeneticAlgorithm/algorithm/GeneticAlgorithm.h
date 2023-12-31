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
    static void start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph);
    static Node orderCrossover(const Node& parent1, const Node& parent2, int start);
};


#endif //GENETICALGORITHM_GENETICALGORITHM_H
