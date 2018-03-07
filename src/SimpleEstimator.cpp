//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <string>
#include <math.h>

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
    nr_edges_in.resize(graph.get()->getNoVertices());
    nr_edges_out.resize(graph.get()->getNoVertices());
    nr_label_occurences.resize(graph.get()->getNoLabels());
    array.resize(graph.get()->getNoLabels());
}

void SimpleEstimator::prepare() {

    for(int k =0; k< array.size(); k++){
        array[k] = 0;
    }

    for(uint32_t source = 0; source < graph->getNoVertices(); source++) {
        for (auto labelTarget : graph->adj[source]) {
            auto label = labelTarget.first;
            auto target = labelTarget.second;
            array[label] +=1;
        }
    }

    std::cout << array[0] << " " << array[1] << " " << array[2] << " " << array[3] << "\n";

    // For each node count the number of outgoing and incoming edges
    for(int source = 0; source < graph->getNoVertices(); source++) {
        nr_edges_out[source] = graph->adj[source].size();
        std::cout << "size of: " << graph->adj[source].data() << " number edges: " << nr_edges_out[source] << "\n";
    }

    for(int target = 0; target < graph->getNoVertices(); target++) {
        nr_edges_in[target] = graph->reverse_adj[target].size();
    }

//    //for each unique label count the number of occurences
//    for(int label = 0; label < graph->getNoLabels(); label++) {
//        nr_label_occurences[label] =
//    }


}

int SimpleEstimator::estimatePath(RPQTree *q , int level) {
    if(q->isLeaf()){
        std::regex directLabel (R"((\d+)\+)");
        std::regex inverseLabel (R"((\d+)\-)");
        std::smatch matches;
        if(std::regex_search(q->data, matches, directLabel)) {
            auto label = (uint32_t) std::stoul(matches[1]);
            return array[label] + int((pow(array[label], array[label] / graph.get()->getNoEdges()) * level));
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            auto label = (uint32_t) std::stoul(matches[1]);
            return array[label] + int((pow(array[label], array[label] / graph.get()->getNoEdges()) * level));
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
            return 0;
        }
    }
    if(q->isConcat()) {
        float nr_edges_per_vertices = float(graph.get()->getNoEdges()) / float(graph.get()->getNoVertices());
        auto left = estimatePath(q->left, level++);
        auto right = estimatePath(q->right, level++);
        return left+right;
    }
    return 0;
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
    while (q_in->isConcat()) {
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
    noPaths = uint32_t(estimatePath(q, 0));
    return cardStat {noOut,a,noIn};
}