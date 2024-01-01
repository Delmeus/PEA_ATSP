//
// Created by antek on 31.12.2023.
//

#include <random>
#include "GeneticAlgorithm.h"
#include "map"
#include "iostream"
#include "../utils/Timer.h"

void GeneticAlgorithm::start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph) {
    vector<Node> population;
    /*
     * Initialize population
     */
    while(population.size() < populationSize){
        population.push_back(Node::generateRandomNode(graph));
    }
    Node bestSolution;
    bestSolution.cost = INT_MAX;
    Timer timer;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, 1.0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomVertex(0, graph.vertices / 2 + 1);
    timer.start();
    timer.stop();
    timer.start();
    while(timer.mili() < stopCondition * 1000){
        calculateFitness(population);
        if(bestSolution.cost > population[0].cost) {
            bestSolution = population[0];
            cout << "Found better solution -> " << bestSolution.cost << endl;
        }
        vector<Node> nextGeneration;
        Node previousElement = population[0];

        for(auto element : population){
            /*
             * Let the best solutions go to the next generation
             */
            if(element.fitness > 0.8) {
                nextGeneration.push_back(element);
            }
            else if(element.fitness < 0.5)
                break;
            //if(element.fitness > 0.5){
            double value = distribution(generator);
            /*
            * Check if node should mutate
            */
            if(1 - value < mutationFactor){
                //cout << "mutation occured" << endl;
                Node mutatedNode = element.scrambleMutate();
                mutatedNode.calculateCost(graph);
                nextGeneration.push_back(mutatedNode);
            }
            value = distribution(generator);
            if(1 - value < crossoverFactor && element.fitness != previousElement.fitness){
                //TODO change how nodes are chosen for crossover
                int start = randomVertex(gen);
                Node offspring = orderCrossover(element, previousElement, start);
                offspring.calculateCost(graph);
                nextGeneration.push_back(offspring);
                offspring = orderCrossover(previousElement, element, start);
                offspring.calculateCost(graph);
                nextGeneration.push_back(offspring);
            }
            previousElement = element;
        }
        population = nextGeneration;
        timer.stop();
    }
    bestSolution.printNode();
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
        if(find(offspring.chromosome.begin(), offspring.chromosome.end(), i) != offspring.chromosome.end()){
            values[i] = true;
        }
        else
            values[i] = false;
    }
    /*
     * Use parent2 to fill in the gaps is offspring
     */
    int i = start + segmentLength;
    int j = i;
    do{
        if(i >= size)
            i = 0;
        if(j >= size)
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

void GeneticAlgorithm::calculateFitness(vector<Node> &population) {
    sort(population.begin(), population.end(), compareByCost);
    double score = 1;
    for(auto &element : population){
        element.fitness = score;;
        score -= 0.01;
    }
}

bool GeneticAlgorithm::compareByCost(const Node &a, const Node &b) {
    return a.cost < b.cost;
}
