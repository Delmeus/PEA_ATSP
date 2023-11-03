//
// Created by antek on 02.11.2023.
//
#include "iostream"
#include "BBSolver.h"

using namespace std;

void BBSolver::solve(Graph& graph) {
    int x, y;
    graph.display();
    Graph g1 = relax(graph, x, y);
    cout << "most costly " << x << " " << y << endl;
    graph.display();
    g1.display();
}

Graph BBSolver::relax(Graph &graph, int& x, int& y) {
    int lowerBound = 0, highest = 0, column = 0;
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

        if(smallest > highest) {
            highest = smallest;
            x = i;
            y = column;
        }

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
    //cout << "lower bound is: " << lowerBound << endl;

    Graph reducedMatrix(graph.vertices - 1);
    /*
     *  create matrix without row x and column y
     */
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
    return reducedMatrix;
}
