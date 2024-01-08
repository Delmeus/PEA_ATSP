//
// Created by antek on 31.12.2023.
//

#include <random>
#include "GeneticAlgorithm.h"
#include "map"
#include "iostream"
#include "../utils/Timer.h"


double ALLOW_INTO_NEXT_GENERATION_THRESHOLD = 0.95;
double MINIMAL_REQUIRED_FITNESS = 0.7;
int MAXIMAL_POPULATION_SIZE = 10000;

void GeneticAlgorithm::start(int populationSize, long stopCondition, double mutationFactor, double crossoverFactor, const Graph& graph, int target, bool crossoverMethod, bool mutationMethod, bool print) {
    int MINIMAL_AMOUNT_OF_INDIVIDUALS = populationSize / 4;
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
    uniform_int_distribution<> randomInt(0, graph.vertices / 2 );

    Timer timer;
    timer.start();
    timer.stop();
    timer.start();

    long solutionTime = 0;

    while(timer.mili() < stopCondition * 1000 && bestSolution.cost != target){
        calculateFitness(population);
        if(bestSolution.cost > population[0].cost) {
            bestSolution = population[0];
            solutionTime = timer.mili();
            if(print)
                cout << "Found better solution -> " << bestSolution.cost << " current population size = " << population.size() << endl;
        }
        /*
         * Select individuals who can potentially mate or mutate
         */
        for(int i = (int) population.size(); i >= 0; i--){
            if((population[i].fitness < MINIMAL_REQUIRED_FITNESS && i > MINIMAL_AMOUNT_OF_INDIVIDUALS * 2) || i > MAXIMAL_POPULATION_SIZE){
                population.pop_back();
            }
            else
                break;
        }

        vector<Node> nextGeneration;
        for(auto element : population){
            /*
             * Let the best solutions go to the next generation
             */
            if((element.fitness > ALLOW_INTO_NEXT_GENERATION_THRESHOLD || (int) nextGeneration.size() < MINIMAL_AMOUNT_OF_INDIVIDUALS) && !containsNode(nextGeneration, element)) {
                nextGeneration.push_back(element);
            }

            double value = distribution(generator);
            /*
            * Check if node should mutate
            */
            if(1 - value <= mutationFactor){
                Node mutatedNode = element.mutate(mutationMethod);
                mutatedNode.calculateCost(graph);
                if(!containsNode(nextGeneration, mutatedNode)) {
                    nextGeneration.push_back(mutatedNode);
                }
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
                randomInt = uniform_int_distribution<>(4, graph.vertices / 2 - 1);
                int segmentLength = randomInt(gen);
                randomInt = uniform_int_distribution<>(0, graph.vertices / 2 );
                /*
                 * Generate first child
                 */
                Node offspring = crossover(element, parent, start, segmentLength, graph, crossoverMethod);
                if(hasDuplicates(offspring.chromosome)) {
                    throw std::invalid_argument("Duplicate occured");
                }

                if(!containsNode(nextGeneration, offspring)) {
                    nextGeneration.push_back(offspring);
                }
                /*
                 * Generate second child
                 */
                offspring = crossover(parent, element, start, segmentLength, graph, crossoverMethod);
                if(hasDuplicates(offspring.chromosome)) {
                    throw std::invalid_argument("Duplicate occured");
                }

                if(!containsNode(nextGeneration, offspring)) {
                    nextGeneration.push_back(offspring);
                }
            }
        }
        population = nextGeneration;
        timer.stop();
    }
    bestSolution.printNode();
    cout << "Solution found in " << solutionTime << endl;
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

[[maybe_unused]] Node GeneticAlgorithm::pmx(const Node &parent1, const Node &parent2, int start, int segmentLength, const Graph &graph) {
    Node offspring;
    int size = (int) parent1.chromosome.size();
    vector<int> newChromosome(size, -1);

    copy(parent1.chromosome.begin() + start, parent1.chromosome.begin() + start + segmentLength + 1, newChromosome.begin() + start);
    offspring.chromosome = newChromosome;
    /*
     * Find values that are not represented in the swath from parent1
     */
    vector<pair<int,int>> waitingForCopy;
    unordered_set<int> offspringSet(parent1.chromosome.begin() + start, parent1.chromosome.begin() + start + segmentLength + 1);
    for(int i = start; i <= start + segmentLength; i++){
        if(offspringSet.find(parent2.chromosome[i]) != offspringSet.end()) {
            continue;
        }
        waitingForCopy.emplace_back(parent2.chromosome[i], i);
    }
    for(auto gene : waitingForCopy){
        int index = gene.second;
        /*
         * Find what value is at this place in parent1 and get
         * position of this value in parent2, repeat until index is
         * not in the original swath
         */
        while(index >= start && index < start + segmentLength + 1){
            int v = parent1.chromosome[index];
            index = getIndex(parent2.chromosome, v);
        }
        offspring.chromosome[index] = gene.first;
    }
    /*
     * Fill the remaining positions with values from parent2
     */
    for(int i = 0; i < size; i++){
        if(offspring.chromosome[i] == -1) {
            offspring.chromosome[i] = parent2.chromosome[i];
        }
    }
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

/*
 * To avoid duplicates in population
 */
bool GeneticAlgorithm::containsNode(const vector<Node> &nextGeneration, const Node &node) {
    return std::find(nextGeneration.begin(), nextGeneration.end(), node) != nextGeneration.end();
}

Node GeneticAlgorithm::crossover(const Node &parent1, const Node &parent2, int start, int segmentLength, const Graph &graph, bool order) {
    if(order){
        return orderCrossover(parent1, parent2, start, segmentLength, graph);
    }
    else{
        return edgeCrossover(parent1, parent2, start, segmentLength, graph);
    }

}


Node GeneticAlgorithm::edgeCrossover(const Node &parent1, const Node &parent2, int start, int segmentLength, const Graph &graph) {
    /*
     * Create and fill edge table
     */
    vector<int> edgeTable[graph.vertices];
    for(int i = 0; i < graph.vertices; i++){
        int p1index = getIndex(parent1.chromosome, i);
        int p2index = getIndex(parent2.chromosome, i);
        edgeTable[i].push_back(parent1.getNeighbours(p1index).first);
        edgeTable[i].push_back(parent1.getNeighbours(p1index).second);
        edgeTable[i].push_back(parent2.getNeighbours(p2index).first);
        edgeTable[i].push_back(parent2.getNeighbours(p2index).second);
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> randomInt(0, graph.vertices - 1 );
    Node offspring;
    int v = randomInt(gen);
    offspring.chromosome.push_back(v);
    while(offspring.chromosome.size() < parent1.chromosome.size()){
        /*
        * Remove all mentions of v
        */
        for (int i = 0; i < graph.vertices; i++)
            edgeTable[i].erase(remove(edgeTable[i].begin(), edgeTable[i].end(), v), edgeTable[i].end());
        if(edgeTable[v].empty()){

            v = offspring.chromosome[0];

            randomInt = uniform_int_distribution<>(0, graph.vertices - 1);
            while(edgeTable[v].empty()){
                v = randomInt(gen);
            }

        }

        int previous = v;
        map<int, int> occurrences;
        map<int, int> lengths;
        for (int i: edgeTable[v]) {
            lengths[i] = (int)edgeTable[i].size();
            if (occurrences.find(i) != occurrences.end()) {
                occurrences[i] = 2;
                v = i;
                break;
            } else
                occurrences[i] = 1;

        }

        if (v == previous) {
            int minLength = min_element(lengths.begin(), lengths.end(), [](const auto& pair1, const auto& pair2) {
                return pair1.second < pair2.second;
            })->second;
            std::vector<int> minLengthElements;

            for (const auto& pair : lengths) {
                if (pair.second == minLength) {
                    minLengthElements.push_back(pair.first);
                }
            }

            if (!minLengthElements.empty()) {
                randomInt = uniform_int_distribution<> (0, (int)minLengthElements.size() - 1);
                v = minLengthElements[randomInt(gen)];
            }
        }
        offspring.chromosome.push_back(v);
    }
    offspring.calculateCost(graph);
    return offspring;
}
