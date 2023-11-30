//
// Created by Antek on 27.11.2023.
//

#ifndef TABUSEARCH_TABUMANAGER_H
#define TABUSEARCH_TABUMANAGER_H

#include <utility>
#include <vector>

using namespace std;

class TabuManager {
public:
    struct TabuElement{
        TabuElement(const pair<int, int> &move, int time);

        pair<int, int> move;
        int time;
    };

    vector<TabuElement> tabuList;

    int TABU_TIME_INCREASE_INTERVAL;
    int TABU_TIME_LIMIT;
    int ALLOW_WORSE_SOLUTION_INTERVAL;
    int ALLOW_WORSE_SOLUTION_ITERATION;
    int DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL;
    int RANDOM_SOLUTION_INTERVAL;

    TabuManager(int tabuTimeIncreaseInterval, int tabuTimeLimit, int allowWorseSolutionInterval,
                int allowWorseSolutionIteration, int defineBestSolutionNeighboursInterval, int randomSolutionInterval);

    void updateParameters(int timeSinceChange);
    bool isForbidden(pair<int, int> p);
    void decreaseTime();
    void emplaceInTabu(int v1, int v2, int v3, int v4, int tabuTime);

};


#endif //TABUSEARCH_TABUMANAGER_H
