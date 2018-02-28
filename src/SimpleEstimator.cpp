//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <string>

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {
    // do your prep here

}

cardStat SimpleEstimator::estimate(RPQTree *q) {


    // perform your estimation here

    while (q->isConcat() == 1) {
        q = q->left;
    }
    std::cout << q->data;

    // no of labels = 4
    // no of edges = 22158
    // no of vertices = 9726
    return cardStat {0, 1, 0};
}