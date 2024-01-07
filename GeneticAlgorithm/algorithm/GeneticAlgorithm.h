//
// Created by antek on 31.12.2023.
//

#ifndef GENETICALGORITHM_GENETICALGORITHM_H
#define GENETICALGORITHM_GENETICALGORITHM_H

#include <unordered_set>
#include "Node.h"
#include "iostream"

using namespace std;

class GeneticAlgorithm {
private:
    //static vector<Node> population;

    static Node crossover(const Node& parent1, const Node& parent2, int start, int segmentLength, const Graph& graph, bool order);
    static Node orderCrossover(const Node& parent1, const Node& parent2, int start, int segmentLength, const Graph& graph);
    static Node pmx(const Node& parent1, const Node& parent2, int start, int segmentLength, const Graph& graph);
    static void calculateFitness(vector<Node> &population);
    static bool compareByCost(const Node& a, const Node& b);
    static int getIndex(const vector<int>& v, int K);

    static bool hasDuplicates(const std::vector<int>& vec) {
        std::unordered_set<int> uniqueValues;

        for (int value : vec) {
            if(value == -1)
                continue;
            // If the value is already in the set, it's a duplicate
            if (!uniqueValues.insert(value).second) {
                cout << "duplicate:" << value << endl;
                return true; // Duplicate found
            }
        }

        return false; // No duplicates found
    }

    static bool containsNode(const vector<Node>& nextGeneration, const Node& node);

public:
    static void start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph, int target, bool crossoverMethod, bool mutationMethod, bool print);
};


#endif //GENETICALGORITHM_GENETICALGORITHM_H
