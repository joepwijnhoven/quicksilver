//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <string>

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
    nr_edges_in.resize(graph.get()->getNoVertices());
    nr_edges_out.resize(graph.get()->getNoVertices());
    nr_label_occurences.resize(graph.get()->getNoLabels());


}

void SimpleEstimator::prepare() {
    // do your prep here

    // For each node count the number of outgoing and incoming edges
    for(int source = 0; source < graph->getNoVertices(); source++) {
        nr_edges_out[source] = graph->adj[source].size();
        std::cout << "size of: " << source << " number edges: " << nr_edges_out[source] << "\n";
    }

    for(int target = 0; target < graph->getNoVertices(); target++) {
        nr_edges_in[target] = graph->reverse_adj[target].size();
    }

//    //for each unique label count the number of occurences
//    for(int label = 0; label < graph->getNoLabels(); label++) {
//        nr_label_occurences[label] =
//    }


}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    // perform your estimation here

    // variables to be returned later
    uint32_t noIn;
    uint32_t noOut;
    uint32_t noPaths;

    /* check what the first and last node are in query, because we can assume s and t can be anything for
     * noOut we look at all the edges corresponding to the first label in the query. Similar thing is done for noIn */

    RPQTree *q_in = q;
    RPQTree *q_out = q;
    // retrieve the begin of the query
    while (q_in->isConcat() == 1) {
         q_in = q_in->left;
    }
    // get the int value of the node and the direction, direction not needed yet.
    std::string query_begin = q_in->data;
    int query_begin_edge = atoi(query_begin.substr(0, query_begin.length() - 1).c_str());
    char query_begin_direction = query_begin.at(query_begin.length() - 1);

    while (q_out->isConcat() == 1) {
        q_out = q_out->right;
    }
    std::string query_end = q_out->data;
    int query_end_edge = atoi(query_end.substr(0, query_end.length() - 1).c_str());
    char query_end_direction = query_end.at(query_end.length() - 1);

//    noOut = nr_edges_in[query_begin_edge];
//
//    // for the final node direction does matter.
//    if(query_end_direction == '+') {
//        noIn = nr_edges_out[query_end_edge];
//    } else {
//        noIn = nr_edges_in[query_end_edge];
//    }

    noOut = graph.get()->getNoEdges()/graph.get()->getNoLabels();
    noIn = graph.get()->getNoEdges()/graph.get()->getNoLabels();
    noPaths = graph.get()->getNoVertices()/graph.get()->getNoLabels();

    return cardStat {noOut,noPaths,noIn};
}