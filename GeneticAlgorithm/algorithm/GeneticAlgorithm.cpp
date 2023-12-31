//
// Created by antek on 31.12.2023.
//

#include <random>
#include "GeneticAlgorithm.h"
#include "map"
#include "iostream"

void GeneticAlgorithm::start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph) {
    /*
     * Initialize population
     */
    vector<Node> population;
    while(population.size() < populationSize){
        population.push_back(Node::generateRandomNode(graph));
    }


}

Node GeneticAlgorithm::orderCrossover(const Node& parent1, const Node& parent2, int start) {
    Node offspring;
    int size = (int) parent1.chromosome.size();
    int segmentLength = size / 2;
    vector<int> newChromosome(size, -1);
    /*
     * copy a segment of parent1 into offspring
     */
    copy(parent1.chromosome.begin() + start, parent1.chromosome.begin() + start + segmentLength, newChromosome.begin() + start);
    offspring.chromosome = newChromosome;
    /*
     * create a map to track which cities are included in chromosome
     */
    map<int, bool> values;
    for(int i = 0; i < size; i++){
        if (offspring.chromosome[i] == -1){
            values[offspring.chromosome[i]] = false;
        }
        else
            values[offspring.chromosome[i]] = true;
    }
    /*
     * Use parent2 to fill in the gaps is offspring
     */
    int i = start + segmentLength;
    int j = i;
    do{
        if(i == size)
            i = 0;
        if(j == size)
            j = 0;
        if(values[parent2.chromosome[i]])
            j--;
        else{
            offspring.chromosome[j] = parent2.chromosome[i];
            values[parent2.chromosome[i]] = true;
        }
        j++;
        i++;
    }while(j != start);
    return offspring;
}
