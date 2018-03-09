//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <string>
#include <math.h>
#include <w32api/ntdef.h>

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
    nr_edges_in.resize(graph.get()->getNoVertices());
    nr_edges_out.resize(graph.get()->getNoVertices());
    nr_label_occurences.resize(graph.get()->getNoLabels());
    array.resize(graph.get()->getNoLabels());
    tabels.resize(graph.get()->getNoLabels());
    uniqueIN.resize(graph.get()->getNoLabels());
    uniqueOUT.resize(graph.get()->getNoLabels());
}

void SimpleEstimator::prepare() {

    for(int k =0; k< array.size(); k++){
        array[k] = 0;
    }

    for(uint32_t source = 0; source < graph->getNoVertices(); source++) {
        for (auto labelTarget : graph->adj[source]) {
            auto label = labelTarget.first;
            auto target = labelTarget.second;
            tabels[label].emplace_back(std::make_tuple(source, target));
            array[label] +=1;
            if(!(std::find(std::begin(uniqueIN[label]), std::end(uniqueIN[label]), source) != std::end(uniqueIN[label]))) {
                uniqueIN[label].push_back(source);
            }
            if(!(std::find(std::begin(uniqueOUT[label]), std::end(uniqueOUT[label]), target) != std::end(uniqueOUT[label]))) {
                uniqueOUT[label].push_back(target);
            }
        }
    }

    std::cout << array[0] << " " << array[1] << " " << array[2] << " " << array[3] << "\n";
    std::cout << "HERE: " << uniqueIN[0].size() << " " << uniqueOUT[0].size();
    std::cout << "HERE: " << uniqueIN[1].size() << " " << uniqueOUT[1].size();
    std::cout << "HERE: " << uniqueIN[2].size() << " " << uniqueOUT[2].size();
    std::cout << "HERE: " << uniqueIN[3].size() << " " << uniqueOUT[3].size();



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

std::vector<int> SimpleEstimator::estimatePath(RPQTree *q) {
    if(q->isLeaf()){
        std::regex directLabel (R"((\d+)\+)");
        std::regex inverseLabel (R"((\d+)\-)");
        std::smatch matches;
        if(std::regex_search(q->data, matches, directLabel)) {
            auto label = (uint32_t) std::stoul(matches[1]);
            std::vector<int> test {static_cast<int>(uniqueIN[label].size()), static_cast<int>(uniqueOUT[label].size()), array[label]};
            return test;
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            auto label = (uint32_t) std::stoul(matches[1]);
            std::vector<int> test {static_cast<int>(uniqueOUT[label].size()), static_cast<int>(uniqueIN[label].size()), array[label]};
            return test;
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
            std::vector<int> test;
            return test;
        }
    }
    if(q->isConcat()) {
        auto left = estimatePath(q->left);
        auto right = estimatePath(q->right);
        std::vector<int> test {left[0], right[1], (int) min((left[2] * (((double)right[2])/((double)right[0]))), right[2] * ((double)left[2]/(double)left[1]))};
        return test;
    }
    return {};
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
    std::vector<int> testvar = estimatePath(q);
    //noPaths = uint32_t(estimatePath(q)[2]);
    return cardStat {testvar[0],testvar[2],testvar[1]};
}