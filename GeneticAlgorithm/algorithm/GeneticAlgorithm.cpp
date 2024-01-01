//
// Created by antek on 31.12.2023.
//

#include <random>
#include "GeneticAlgorithm.h"
#include "map"
#include "iostream"
#include "../utils/Timer.h"

double ALLOW_INTO_NEXT_GENERATION_THRESHOLD = 0.8;
double MINIMAL_REQUIRED_FITNESS = 0.5;

void GeneticAlgorithm::start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph, int target) {
    vector<Node> population;
    /*
     * Initialize population
     */
    population.push_back(Node::greedySolution(graph));
    while(population.size() < populationSize){
        population.push_back(Node::generateRandomNode(graph));
    }
    Node bestSolution;
    bestSolution.cost = INT_MAX;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<double> distribution(0.0, 1.0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomInt(0, graph.vertices / 2 + 1);

    Timer timer;
    timer.start();
    timer.stop();
    timer.start();

    while(timer.mili() < stopCondition * 1000 && bestSolution.cost != target){
        calculateFitness(population);
        if(bestSolution.cost > population[0].cost) {
            bestSolution = population[0];
            cout << "Found better solution -> " << bestSolution.cost << endl;
        }
        vector<Node> nextGeneration;

        for(auto element : population){
            /*
             * Let the best solutions go to the next generation
             */
            if(element.fitness > ALLOW_INTO_NEXT_GENERATION_THRESHOLD) {
                nextGeneration.push_back(element);
            }
            /*
             * If solutions have too small fitness, stop exploring them
             */
            else if(element.fitness < MINIMAL_REQUIRED_FITNESS)
                break;

            double value = distribution(generator);
            /*
            * Check if node should mutate
            */
            if(1 - value <= mutationFactor){
                Node mutatedNode = element.scrambleMutate();
                mutatedNode.calculateCost(graph);
                nextGeneration.push_back(mutatedNode);
            }
            value = distribution(generator);
            /*
             * Check if crossover should be performed
             */
            if(1 - value <= crossoverFactor){
                /*
                 * Select second parent
                 */
                Node parent = element.selectParent(population);
                /*
                 * If returned parent is the same node, skip crossover
                 */
                if(equal(parent.chromosome.begin(), parent.chromosome.end(), element.chromosome.begin()))
                    continue;

                int start = randomInt(gen);
                randomInt = uniform_int_distribution<>(4, graph.vertices / 2);
                int segmentLength = randomInt(gen);
                randomInt = uniform_int_distribution<>(0, graph.vertices / 2 + 1);
                /*
                 * Generate first child
                 */
                //Node offspring = pmx(element, parent, start, segmentLength, graph);
                Node offspring = orderCrossover(element, parent, start, segmentLength, graph);
                nextGeneration.push_back(offspring);
                /*
                 * Generate second child
                 */
                //offspring = pmx(parent, element, start, segmentLength, graph);
                offspring = orderCrossover(parent, element, start, segmentLength, graph);
                nextGeneration.push_back(offspring);
            }
        }
        population = nextGeneration;
        timer.stop();
    }
    bestSolution.printNode();
}

Node GeneticAlgorithm::orderCrossover(const Node& parent1, const Node& parent2, int start, int segmentLength, const Graph& graph) {

    Node offspring;
    int size = (int) parent1.chromosome.size();

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

    offspring.calculateCost(graph);

    return offspring;
}
//TODO fix - gets stuck
Node GeneticAlgorithm::pmx(const Node &parent1, const Node &parent2, int start, int segmentLength, const Graph &graph) {
    Node offspring;
    int size = (int) parent1.chromosome.size();

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

    for(int i = start; i < start + segmentLength; i++){
        /*
         * If value from parent2 is already in offspring, continue
         */
        if(find(offspring.chromosome.begin(), offspring.chromosome.end(), parent2.chromosome[i]) != offspring.chromosome.end())
            continue;
        /*
         * Get index at which value from parent2 is at in parent1
         */
        int index = getIndex(parent1.chromosome, parent2.chromosome[i]);
        /*
         * If this position in offspring is free, fill it with this value
         */
        if(offspring.chromosome[index] == -1) {
            offspring.chromosome[index] = parent2.chromosome[i];
            values[parent2.chromosome[i]] = true;
        }
        /*
         * Else check what element was copied at the place of value currently being explored
         */
        else {
            int value = parent1.chromosome[i];
            index = getIndex(parent1.chromosome, value);
            offspring.chromosome[index] = parent2.chromosome[i];
            values[parent2.chromosome[i]] = true;
        }
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

    offspring.calculateCost(graph);
    return offspring;
}

void GeneticAlgorithm::calculateFitness(vector<Node> &population) {
    sort(population.begin(), population.end(), compareByCost);
    double minCost = population.front().cost;
    double maxCost = population.back().cost;

    double costRange = maxCost - minCost;

    for (auto &node : population) {
        if (costRange > 0) {
            double score = (node.cost - minCost) / costRange;
            node.fitness = 1 - score;
        } else {
            node.fitness = 1;
        }
    }
}

bool GeneticAlgorithm::compareByCost(const Node &a, const Node &b) {
    return a.cost < b.cost;
}

int GeneticAlgorithm::getIndex(const vector<int>& v, int K)
{
    auto it = find(v.begin(), v.end(), K);

    if (it != v.end())
    {
        int index = it - v.begin();
        return index;
    }
    else {
        return -1;
    }
}