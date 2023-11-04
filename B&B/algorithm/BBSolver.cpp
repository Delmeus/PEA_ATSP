//
// Created by antek on 02.11.2023.
//
#include <vector>
#include <queue>
#include "iostream"
#include "BBSolver.h"

using namespace std;

void BBSolver::solve(Graph& graph, int city) {
    int lowerBound = 0;
    int bestCost = INT_MAX;
    timer.start();
    relax(graph, lowerBound);
    /*
     * Queue to store possible paths
     */
    std::priority_queue<Node, std::vector<Node>, NodeComparator> nodeQueue;
    /*
     * initialize first Node containing starting city and push it
     * onto the priority queue
     */
    vector<int> initialPath, bestPath;
    initialPath.push_back(city);
    Node initialNode(initialPath, 0);
    nodeQueue.push(initialNode);

    while(!nodeQueue.empty()) {
        timer.stop();
        if(timer.mili() > 5000*60) {
            cout << "Time limit reached!" << endl;
            break;
        }
        /*
         * explore the node from top of the priority queue
         */
        Node currentNode = nodeQueue.top();
        nodeQueue.pop();
        /*
         * check if the path is complete and if this solution is better than
         * the previous ones
         */
        if (currentNode.path.size() == graph.vertices) {
            /*
             * return to origin
             */
            currentNode.cost += graph.edges[currentNode.path.back()][city];
            //currentNode.path.push_back(city);
            if (currentNode.cost < bestCost) {
                bestCost = currentNode.cost;
                bestPath = currentNode.path;
            }
            continue;
        }
        /*
         * explore possible paths
         */
        for (int i = 0; i < graph.vertices; i++) {
            if (canAddVertex(currentNode, i)) {
                /*
                 * add unvisited city to the current path
                 */
                std::vector<int> newPath = currentNode.path;
                newPath.push_back(i);
                int newCost = calculateCost(graph, currentNode, i);
                /*
                 * add node to queue only when it has a chance to be a better solution
                 */
                if(newCost < bestCost) {
                    Node newNode(newPath, newCost);
                    nodeQueue.push(newNode);
                }
            }
        }
    }
    timer.stop();
    cout << "Time needed to complete " << timer.mili() << " ms " << timer.micro() << " microseconds" << endl;
    cout << "Best cost: " << bestCost + lowerBound << endl;
    for(auto element : bestPath)
        cout << element << " -> ";
    cout << city << endl;
}

void BBSolver::relax(Graph& graph, int& lowerBound) {

    int highest = 0, column = 0;
    int x, y;
    /*
     * find lowest value in every row
    */
    for(int i = 0; i < graph.vertices; i++){
        int smallest = INT_MAX;
        for(int j = 0; j < graph.vertices; j++){
            if(graph.edges[i][j] < smallest && i != j) {
                smallest = graph.edges[i][j];
                column = j;
            }

        }
/*
        if(smallest > highest) {
            highest = smallest;
            x = i;
            y = column;
        }
        */

        lowerBound += smallest;
        /*
         * subtract smallest cost from every element in a row except the ones being zeros
         */
        for(int j = 0; j < graph.vertices; j++){

            if(graph.edges[i][j] == 0)
                continue;

            graph.edges[i][j] -= smallest;
        }

    }
    /*
     * find lowest value in every column that doesnt contain a 0
     */
    for(int i = 0; i < graph.vertices; i++){
        int smallest = INT_MAX;
        int row;
        for(int j = 0; j < graph.vertices; j++){
            /*
             * if a column contains a 0 don't check for smallest cost
             */
            if(graph.edges[j][i] == 0 && j != i){
                row = -1;
                break;
            }

            if(graph.edges[j][i] < smallest && j != i){
                smallest = graph.edges[j][i];
                row = j;
            }
        }

        if(row != -1) {
/*
            if(smallest > highest) {
                highest = smallest;
                x = row;
                y = i;
            }
            /*
            * subtract smallest cost from every element in a column except the ones being zeros
            */
            lowerBound += smallest;
            for(int j = 0; j < graph.vertices; j++){

                if(graph.edges[j][i] == 0)
                    continue;

                graph.edges[j][i] -= smallest;
            }
        }

    }

    Graph reducedMatrix(graph.vertices - 1);
    /*
     *  create matrix without row x and column y

    for(int i = 0; i < graph.vertices; i++){

        if(i == x)
            continue;

        for(int j = 0; j < graph.vertices; j++){

            if(j == y || j == i)
                continue;

            if(i < x){
                if(j < y)
                    reducedMatrix.edges[i][j] = graph.edges[i][j];
                else
                    reducedMatrix.edges[i][j - 1] = graph.edges[i][j];
            }
            else{
                if(j < y)
                    reducedMatrix.edges[i - 1][j] = graph.edges[i][j];
                else
                    reducedMatrix.edges[i - 1][j - 1] = graph.edges[i][j];
            }
        }
    }
    */
}

bool BBSolver::canAddVertex(const BBSolver::Node &currentNode, int i) {
    for(int vertex : currentNode.path) {
        if (vertex == i) {
            return false;
        }
    }
    return true;
}

int BBSolver::calculateCost(Graph& graph, const BBSolver::Node &currentNode, int i) {
    int lastVertex = currentNode.path.back();
    int cost = graph.edges[lastVertex][i];
    return currentNode.cost + cost;
}
