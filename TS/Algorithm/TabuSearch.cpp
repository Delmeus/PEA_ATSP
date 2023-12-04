//
// Created by Antek on 21.11.2023.
//

#include <random>
#include "TabuSearch.h"
#include "iostream"
#include "../utils/Timer.h"
#include "TabuManager.h"

TabuManager params(1000, 5, 300, 20, 5000, 10000);

pair<Node, long> TabuSearch::TSSolver(Graph &graph, int algorithmTime, bool print, int neighbourhoodMethod) {
    if(neighbourhoodMethod < 0 || neighbourhoodMethod > 4 || algorithmTime < 0 || graph.vertices < 4)
        throw invalid_argument("Received wrong arguments");

    params.updateParameters(0);

    Node currentSolution = Node::firstSolution(graph);
    Node bestSolution = currentSolution;
    vector<Node> neighbours;

    Node::printNode(currentSolution);

    long INTERVAL = 1;
    long bestSolutionTime;
    int timeSinceChange = 0, bestCost = bestSolution.cost;

    Timer timer;
    timer.start();

    while(timer.mili() < algorithmTime * 60 * 1000){
        /*
         * if there was an improvement
         */
        if(bestCost > bestSolution.cost){
            cout << "FOUND BETTER SOLUTION -> " << bestSolution.cost << endl;
            bestCost = bestSolution.cost;
            timeSinceChange = 0;
            timer.stop();
            bestSolutionTime = timer.mili();
            params.updateParameters(timeSinceChange);
        }
        /*
         * Make adjustments to search parameters if stuck on the same result
         * or if algorithm just found a better solution
         */
        if(timeSinceChange % params.TABU_TIME_INCREASE_INTERVAL == 0 || timeSinceChange == 0)
            params.updateParameters(timeSinceChange);
        /*
         * define neighbours and find the best solution among them
         */
        vector<Node> newNeighbours = defineNeighbours(graph, currentSolution, timeSinceChange, bestSolution, neighbourhoodMethod, INTERVAL);
        neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());

        if(INTERVAL % 10000 == 0 && print) {
            cout    << INTERVAL << "\tbest cost = " << bestSolution.cost << " current cost = " << currentSolution.cost << endl
                    << "\ttabu size = " << params.tabuList.size() << " possible neighbours = " << neighbours.size() << endl;
        }
        /*
         * find best solution among neighbours
         */
        currentSolution = Node::findSolution(currentSolution, neighbours, bestSolution, params);

        if(currentSolution.cost < bestSolution.cost)
            bestSolution = currentSolution;

        params.decreaseTime();
        INTERVAL++;
        timeSinceChange++;

        timer.stop();
    }
    timer.stop();
    cout << "Solution found in " << bestSolutionTime << " ms" << endl;
    Node::printNode(bestSolution);
    return make_pair(bestSolution, bestSolutionTime);
}

vector<Node> TabuSearch::swapTwoCities(Graph &graph, const Node& currentSolution, int timeSinceChange, int tabuTime, Node &bestSolution) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);

    vector<Node> neighbours;
    int neighboursQuantity = graph.vertices;

    if(timeSinceChange > 20000)
        neighboursQuantity *= 2;

    Node bestLegalNeighbour;
    bestLegalNeighbour.cost = INT_MAX;
    for(int i = 0; i < neighboursQuantity; i++){
        /*
         * generate a pair of vertices
         */
        Node neighbour = currentSolution;
        int vertex1 = randomVertex(gen);
        int vertex2;
        do{
            vertex2 = randomVertex(gen);
        } while (vertex1 == vertex2);
        pair<int, int> p(vertex1, vertex2);
        pair<int, int> p1(vertex2, vertex1);
        /*
         * make two independent moves if time since change was long enough
         */
        if(timeSinceChange % 500 == 0 && timeSinceChange != 0 && i % 10 == 0){
            int vertex3, vertex4;
            do{
                vertex3 = randomVertex(gen);
                vertex4 = randomVertex(gen);
            } while (vertex3 == vertex1 || vertex3 == vertex2 || vertex4 == vertex1 || vertex4 == vertex2 || vertex3 == vertex4);
            neighbour.path.at(vertex4) = currentSolution.path.at(vertex1);
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex4);
            neighbour.path.at(vertex3) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex3);
            neighbour.cost = Node::calculateCost(graph, neighbour);
            if(neighbour.cost < bestSolution.cost) {
                neighbours.push_back(neighbour);
                params.emplaceInTabu(vertex1, vertex4, vertex2, vertex3, tabuTime/2);
            }

            else if(neighbour.cost < currentSolution.cost) {
                neighbours.push_back(neighbour);
                params.emplaceInTabu(vertex1, vertex4, vertex2, vertex3, tabuTime);
            }

            else if(timeSinceChange > params.ALLOW_WORSE_SOLUTION_INTERVAL){
                neighbours.push_back(neighbour);
                params.emplaceInTabu(vertex1, vertex4, vertex2, vertex3, tabuTime * 2);
            }

        }
        /*
        * generate random solution if time since change was long enough
        */
        else if(timeSinceChange % params.RANDOM_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
            neighbours.push_back(Node::randomSolution(graph));
            params.tabuList.clear();
        }
        /*
         * if generated neighbour isn't forbidden
         */
        else if(!params.isForbidden(p)){
            neighbour.path.at(vertex1) = currentSolution.path.at(vertex2);
            neighbour.path.at(vertex2) = currentSolution.path.at(vertex1);
            neighbour.cost = Node::calculateCost(graph, neighbour);
            neighbour.move.first = vertex1;
            neighbour.move.second = vertex2;
            /*
             * if cost of neighbour surpasses best cost don't forbid the move
             */
            if(neighbour.cost < bestSolution.cost) {
                neighbours.push_back(neighbour);
            }
            else if(neighbour.cost < currentSolution.cost) {
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime);
                params.tabuList.emplace_back(p1, tabuTime);
            }
            /*
             * if it was long enough since adding worse solution, allow solutions
             * that are worse than current solution
             */
            else if(timeSinceChange % params.ALLOW_WORSE_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime * 2);
                params.tabuList.emplace_back(p1, tabuTime * 2);
            }

            if(neighbour.cost < bestLegalNeighbour.cost) {
                bestLegalNeighbour = neighbour;
            }
        }
        else if(neighbour.cost < bestSolution.cost){
            neighbours.push_back(neighbour);
        }
    }
    /*
     * if no solution was added to neighbours list, push back best solution that wasn't tabu
     */
    if(neighbours.empty()) {
        neighbours.push_back(bestLegalNeighbour);
        params.tabuList.emplace_back(bestLegalNeighbour.move, tabuTime * 2);
        params.tabuList.emplace_back(make_pair(bestLegalNeighbour.move.second, bestLegalNeighbour.move.first), tabuTime);
    }
    return neighbours;
}

vector<Node> TabuSearch::subPaths(Graph &graph, const Node &currentSolution, int timeSinceChange, int tabuTime, Node &bestSolution) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 1);

    vector<Node> neighbours;
    int neighboursQuantity = graph.vertices;

    if(timeSinceChange > 20000)
        neighboursQuantity *= 2;

    Node bestLegalNeighbour;
    bestLegalNeighbour.cost = INT_MAX;
    for(int i = 0; i < neighboursQuantity; i++){
        /*
         * generate two random vertices and create sub-paths including them
         */
        Node neighbour = currentSolution;
        int vertex1 = randomVertex(gen);
        int vertex2;
        do{
            vertex2 = randomVertex(gen);
        } while (vertex1 == vertex2 + 1 || vertex1 == vertex2 - 1 || vertex1 == vertex2 || vertex1 == vertex2 + 2 || vertex1 == vertex2 - 2);
        pair<int, int> p;
        pair<int, int> p1;
        if(vertex1 > vertex2){
            p = make_pair(vertex1 - 1, vertex2);
            p1 = make_pair(vertex2, vertex1 - 1);
        }
        else{
            p = make_pair(vertex2 - 1, vertex1);
            p1 = make_pair(vertex1, vertex2 - 1);
        }
        /*
        * generate random solution if time since change was long enough
        */
        if(timeSinceChange % params.RANDOM_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
            neighbours.push_back(Node::randomSolution(graph));
            params.tabuList.clear();
        }
        /*
        * if generated neighbour isn't forbidden
        */
        else if(!params.isForbidden(p) && !params.isForbidden(p1)){
            if(vertex1 > vertex2){
                neighbour.path.at(vertex2) = currentSolution.path.at(vertex1 - 1);
                neighbour.path.at(vertex2 + 1) = currentSolution.path.at(vertex1);
                neighbour.path.at(vertex1) = currentSolution.path.at(vertex2 + 1);
                neighbour.path.at(vertex1 - 1) = currentSolution.path.at(vertex2);
            }
            else{
                neighbour.path.at(vertex1) = currentSolution.path.at(vertex2 - 1);
                neighbour.path.at(vertex1 + 1) = currentSolution.path.at(vertex2);
                neighbour.path.at(vertex2) = currentSolution.path.at(vertex1 + 1);
                neighbour.path.at(vertex2 - 1) = currentSolution.path.at(vertex1);
            }

            neighbour.cost = Node::calculateCost(graph, neighbour);
            neighbour.move.first = p.first;
            neighbour.move.second = p.second;
            /*
             * if cost of neighbour surpasses best cost don't forbid the move
             */
            if(neighbour.cost < bestSolution.cost) {
                neighbours.push_back(neighbour);
            }
            else if(neighbour.cost < currentSolution.cost) {
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime);
                params.tabuList.emplace_back(p1, tabuTime);
            }
            /*
            * if it was long enough since adding worse solution, allow solutions
            * that are worse than current solution
            */
            else if(timeSinceChange % params.ALLOW_WORSE_SOLUTION_INTERVAL == 0 && timeSinceChange != 0){
                neighbours.push_back(neighbour);
                params.tabuList.emplace_back(p, tabuTime * 2);
                params.tabuList.emplace_back(p1, tabuTime * 2);
            }

            if(neighbour.cost < bestLegalNeighbour.cost)
                bestLegalNeighbour = neighbour;
        }
        else if(neighbour.cost < bestSolution.cost){
            neighbours.push_back(neighbour);
        }
    }
    /*
     * if no solution was added to neighbours list, push back best solution that wasn't tabu
     */
    if(neighbours.empty()) {
        neighbours.push_back(bestLegalNeighbour);
        params.tabuList.emplace_back(bestLegalNeighbour.move, tabuTime * 2);
        params.tabuList.emplace_back(make_pair(bestLegalNeighbour.move.second, bestLegalNeighbour.move.first), tabuTime);
    }
    return neighbours;
}

vector<Node> TabuSearch::permuteFragment(Graph &graph, const Node &currentSolution, int timeSinceChange, int tabuTime, Node &bestSolution) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomVertex(0, graph.vertices - 6);
    vector<Node> neighbours;
    /*
     * generate a random vertex and get a fragment of the current solution
     */
    Node bestLegalNeighbour, neighbour = currentSolution;
    bestLegalNeighbour.cost = INT_MAX;
    int vertex;
    int tries = 0;
    pair<int, int> p;
    do {
        vertex = randomVertex(gen);
        tries++;
        p = make_pair(vertex, vertex + 5);
    }while(params.isForbidden(p) && tries < 100);

    if(!params.isForbidden(p)){
        vector<int> fragment(currentSolution.path.begin() + vertex, currentSolution.path.begin() + vertex + 5 + 1);
        sort(fragment.begin(), fragment.end());
        /*
         * check every permutation of given fragment
         */
        do{
            copy(fragment.begin(), fragment.end(),  neighbour.path.begin() + vertex);
            neighbour.move = p;
            neighbour.cost = Node::calculateCost(graph, neighbour);
            if(neighbour.cost < currentSolution.cost || (timeSinceChange % params.ALLOW_WORSE_SOLUTION_INTERVAL == 0 && timeSinceChange != 0))
                neighbours.push_back(neighbour);
            if(neighbour.cost < bestLegalNeighbour.cost && neighbour.cost != currentSolution.cost)
                bestLegalNeighbour = neighbour;
        }while(next_permutation(fragment.begin(), fragment.end()));
    }
    /*
     * if no solution in neighbours, add the best legal neighbour
     */
    if(neighbours.empty()) {
        if(bestLegalNeighbour.cost == INT_MAX)
            bestLegalNeighbour = currentSolution;
        neighbours.push_back(bestLegalNeighbour);
        params.tabuList.emplace_back(bestLegalNeighbour.move, tabuTime * 3);
    }
    else{
        if(bestLegalNeighbour.cost > currentSolution.cost) {
            params.tabuList.emplace_back(bestLegalNeighbour.move, tabuTime);
        }
        else if(bestLegalNeighbour.cost > bestSolution.cost){
            params.tabuList.emplace_back(bestLegalNeighbour.move, tabuTime / 2);
        }
    }

    return neighbours;
}
/*
 * choose proper function depending on choice of method
 */
vector<Node> TabuSearch::defineNeighbours(Graph &graph, const Node &currentSolution, int timeSinceChange,
                                          Node &bestSolution, int method, long iteration) {
    vector<Node> newNeighbours;
    if(timeSinceChange %  params.DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL == 0) {
        params.tabuList.clear();
        if(method == 1)
            newNeighbours = swapTwoCities(graph, bestSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
        else if(method == 2)
            newNeighbours = subPaths(graph, bestSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
        else if(method == 3)
            newNeighbours = permuteFragment(graph, bestSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
        else{
            if(iteration % 300000 == 0) {
                params.tabuList.clear();
                params.updateParameters(0);
            }
            if(iteration % 900000 < 300000)
                newNeighbours = swapTwoCities(graph, bestSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
            else if(iteration % 900000 > 600000)
                newNeighbours = subPaths(graph, bestSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
            else
                newNeighbours = permuteFragment(graph, bestSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
        }
    }
    else {
        if(method == 1)
            newNeighbours = swapTwoCities(graph, currentSolution, timeSinceChange, params.TABU_TIME_LIMIT,bestSolution);
        else if(method == 2)
            newNeighbours = subPaths(graph, currentSolution, timeSinceChange, params.TABU_TIME_LIMIT,bestSolution);
        else if(method == 3)
            newNeighbours = permuteFragment(graph, currentSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
        else{
            if(iteration % 300000 == 0) {
                params.tabuList.clear();
                params.updateParameters(0);
            }
            if(iteration % 900000 < 300000)
                newNeighbours = swapTwoCities(graph, currentSolution, timeSinceChange, params.TABU_TIME_LIMIT,bestSolution);
            else if(iteration % 900000 > 600000)
                newNeighbours = subPaths(graph, currentSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
            else
                newNeighbours = permuteFragment(graph, currentSolution, timeSinceChange, params.TABU_TIME_LIMIT, bestSolution);
        }
    }

    return newNeighbours;
}



