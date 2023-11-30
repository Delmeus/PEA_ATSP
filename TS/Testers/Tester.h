//
// Created by Antek on 29.11.2023.
//

#ifndef TABUSEARCH_TESTER_H
#define TABUSEARCH_TESTER_H

using namespace std;

class Tester {
public:
    static void test();
    static void automaticTest(const string& name, int time, int target, int numberOfTests);
    static void waitForResponse();
};


#endif //TABUSEARCH_TESTER_H
