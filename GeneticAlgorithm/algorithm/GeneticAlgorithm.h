//
// Created by antek on 31.12.2023.
//

#ifndef GENETICALGORITHM_GENETICALGORITHM_H
#define GENETICALGORITHM_GENETICALGORITHM_H

#include "Node.h"

using namespace std;

class GeneticAlgorithm {
private:
    //static vector<Node> population;

    static Node orderCrossover(const Node& parent1, const Node& parent2, int start, const Graph& graph);
    static void calculateFitness(vector<Node> &population);
    static bool compareByCost(const Node& a, const Node& b);
public:
    static void start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph, int target);
};


#endif //GENETICALGORITHM_GENETICALGORITHM_H
