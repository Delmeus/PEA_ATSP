//
// Created by antek on 31.12.2023.
//

#ifndef GENETICALGORITHM_NODE_H
#define GENETICALGORITHM_NODE_H

#include <vector>

using namespace std;

class Node {
private:
    vector<int> path;
    int cost;
public:
    Node mutate();
    Node generateRandomNode();
};


#endif //GENETICALGORITHM_NODE_H
