//
// Created by Antek on 27.11.2023.
//

#include "TabuManager.h"

TabuManager::TabuElement::TabuElement(const pair<int, int> &move, int time) {
    this->move = move;
    this->time = time;
}

void TabuManager::updateParameters(int timeSinceChange) {

    if (timeSinceChange == 0) {
        TABU_TIME_LIMIT = INITIAL_TABU_TIME_LIMIT;
        RANDOM_SOLUTION_INTERVAL = INITIAL_RANDOM_SOLUTION_INTERVAL;
        DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL = INITIAL_DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL;
        ALLOW_WORSE_SOLUTION_INTERVAL = INITIAL_ALLOW_WORSE_SOLUTION_INTERVAL;
        ALLOW_WORSE_SOLUTION_ITERATION = INITIAL_ALLOW_WORSE_SOLUTION_ITERATION;
    } else {
        if (TABU_TIME_LIMIT < 100) {
            TABU_TIME_LIMIT += 5;
            for (int j = tabuList.size() - 1; j >= 0; j--) {
                tabuList[j].time += 5;
            }
        }
        if(timeSinceChange > 1000 && timeSinceChange < 20000 && timeSinceChange % 1000 == 0) {
            if (timeSinceChange > 2000 && RANDOM_SOLUTION_INTERVAL < 10000)
                RANDOM_SOLUTION_INTERVAL += 50;
            if (timeSinceChange > 2000 && DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL > 600)
                DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL -= 50;
            if (ALLOW_WORSE_SOLUTION_INTERVAL > 50) {
                ALLOW_WORSE_SOLUTION_INTERVAL -= 50;
                if (ALLOW_WORSE_SOLUTION_ITERATION > 5) {
                    ALLOW_WORSE_SOLUTION_ITERATION--;
                }
            }
        }
        else if(timeSinceChange % 1000 == 0){
            if(RANDOM_SOLUTION_INTERVAL > 700)
                RANDOM_SOLUTION_INTERVAL -= 50;
            if (timeSinceChange > 2000 && DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL < 10000)
                DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL += 50;
            if (ALLOW_WORSE_SOLUTION_INTERVAL > 10) {
                ALLOW_WORSE_SOLUTION_INTERVAL -= 10;
                if (ALLOW_WORSE_SOLUTION_ITERATION > 5) {
                    ALLOW_WORSE_SOLUTION_ITERATION--;
                }
            }
        }

    }
}


bool TabuManager::isForbidden(pair<int, int> p) {
    for(auto element : tabuList){
        if(element.move.first == p.first && element.move.second == p.second)
            return true;
    }
    return false;
}

void TabuManager::decreaseTime() {
    for (int j = tabuList.size() - 1; j >= 0; j--) {
        tabuList[j].time--;
        if(tabuList[j].time <= 0)
            tabuList.erase(tabuList.begin() + j);
    }
}



void TabuManager::emplaceInTabu(int v1, int v2, int v3, int v4, int tabuTime) {

    tabuList.emplace_back(make_pair(v1, v2), tabuTime);
    tabuList.emplace_back(make_pair(v2, v1), tabuTime);

    if(v3 != -1 && v4 != -1){
        tabuList.emplace_back(make_pair(v3, v4), tabuTime);
        tabuList.emplace_back(make_pair(v4, v3), tabuTime);
    }
}

TabuManager::TabuManager(int tabuTimeIncreaseInterval, int tabuTimeLimit, int allowWorseSolutionInterval,
                         int allowWorseSolutionIteration, int defineBestSolutionNeighboursInterval,
                         int randomSolutionInterval) : TABU_TIME_INCREASE_INTERVAL(tabuTimeIncreaseInterval),
                                                       TABU_TIME_LIMIT(tabuTimeLimit),
                                                       ALLOW_WORSE_SOLUTION_INTERVAL(allowWorseSolutionInterval),
                                                       ALLOW_WORSE_SOLUTION_ITERATION(allowWorseSolutionIteration),
                                                       DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL(
                                                               defineBestSolutionNeighboursInterval),
                                                       RANDOM_SOLUTION_INTERVAL(randomSolutionInterval),
                                                       INITIAL_TABU_TIME_LIMIT(tabuTimeLimit),
                                                       INITIAL_ALLOW_WORSE_SOLUTION_INTERVAL(allowWorseSolutionInterval),
                                                       INITIAL_ALLOW_WORSE_SOLUTION_ITERATION(allowWorseSolutionIteration),
                                                       INITIAL_DEFINE_BEST_SOLUTION_NEIGHBOURS_INTERVAL(
                                                                defineBestSolutionNeighboursInterval),
                                                       INITIAL_RANDOM_SOLUTION_INTERVAL(randomSolutionInterval){}



