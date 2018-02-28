//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph

    graph = g;
    nr_edges_in[graph.get()->getNoVertices()];
    nr_edges_out[graph.get()->getNoVertices()];

}

void SimpleEstimator::prepare() {
    // do your prep here

    for(int source = 0; source < graph->getNoVertices(); source++) {
        nr_edges_out[source] += graph->adj[source].size();
    }

    for(int target = 0; target < graph->getNoVertices(); target++) {
        nr_edges_in[target] += graph->adj[target].size();
    }
}

cardStat SimpleEstimator::estimate(RPQTree *q) {

    // perform your estimation here

    return cardStat {0, 0, 0};
}