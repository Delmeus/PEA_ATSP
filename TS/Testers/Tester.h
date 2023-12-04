//
// Created by Antek on 29.11.2023.
//

#ifndef TABUSEARCH_TESTER_H
#define TABUSEARCH_TESTER_H

#include <vector>

using namespace std;

class Tester {
public:
    static void test();
    static void automaticTest(const string& name, int time, int target, int numberOfTests, int neighbourhoodMethod);
    static void waitForResponse();
};


#endif //TABUSEARCH_TESTER_H
