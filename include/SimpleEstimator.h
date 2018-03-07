//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"
#include <vector>

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;
    std::vector<uint32_t> nr_edges_in;
    std::vector<uint32_t> nr_edges_out;
    std::vector<uint32_t> nr_label_occurences;
    std::vector<uint32_t> array;
    std::vector<std::vector<std::tuple<int, int>>> tabels;
    std::vector<std::vector<uint32_t>>  uniqueIN;
    std::vector<std::vector<uint32_t>>  uniqueOUT;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override ;
    int estimatePath(RPQTree *q, int level);
    cardStat estimate(RPQTree *q) override ;

};


#endif //QS_SIMPLEESTIMATOR_H
