//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <string>
#include <math.h>
#include <iomanip>
#include <algorithm>

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
//    nr_edges_in.resize(graph.get()->getNoVertices());
//    nr_edges_out.resize(graph.get()->getNoVertices());
    nr_label_occurences.resize(graph.get()->getNoLabels());
    array.resize(graph.get()->getNoLabels());
    tabels.resize(graph.get()->getNoLabels());
    uniqueIN.resize(graph.get()->getNoLabels());
    uniqueOUT.resize(graph.get()->getNoLabels());
    AttributeCountIN.resize(graph.get()->getNoLabels());
    AttributeCountOUT.resize(graph.get()->getNoLabels());
    thresholdsIN.resize(graph.get()->getNoLabels());
    thresholdsOUT.resize(graph.get()->getNoLabels());
    sampleSize = ((graph.get()->getNoEdges() * 2) / graph->getNoLabels()) / 5;
}

void SimpleEstimator::prepare() {

//    for(int i = 0; i < graph.get()->getNoLabels(); i++) {
//        AttributeCountIN[i].resize(graph.get()->getNoVertices());
//        AttributeCountOUT[i].resize(graph.get()->getNoVertices());
//    }


    for(int k =0; k< array.size(); k++){
        array[k] = 0;
    }

    for(uint32_t source = 0; source < graph->getNoVertices(); source++) {
        for (auto labelTarget : graph->adj[source]) {
            auto label = labelTarget.first;
            auto target = labelTarget.second;
            tabels[label].emplace_back(std::make_tuple(source, target));
            array[label] +=1;
            //AttributeCountIN[label][source] +=1;
            //AttributeCountOUT[label][target] +=1;
            if(!(std::find(std::begin(uniqueIN[label]), std::end(uniqueIN[label]), source) != std::end(uniqueIN[label]))) {
                uniqueIN[label].push_back(source);
            }
            if(!(std::find(std::begin(uniqueOUT[label]), std::end(uniqueOUT[label]), target) != std::end(uniqueOUT[label]))) {
                uniqueOUT[label].push_back(target);
            }
        }
    }

//    // sort our unique nodes per label so we can merge later
//    for (int label = 0; label < graph.get()->getNoLabels(); label++) {
//        std::sort(uniqueIN[label].begin(), uniqueIN[label].end());
//        std::sort(uniqueOUT[label].begin(), uniqueOUT[label].end());
//    }

    //find thresholds for in
//    for (int label = 0; label < graph.get()->getNoLabels(); label++) {
//        int threshold = 1;
//        double count = graph.get()->getNoVertices();
//        // if the unique number of nodes is smaller then sample size we add them all.
//        if (uniqueIN[label].size() <= sampleSize) {
//            thresholdsIN[label] = 1;
//        } else {
//            while(count >= sampleSize) {
//                count = 0;
//                //std::cout << "count1: " << count << "\n";
//                for(int i = 0; i < uniqueIN[label].size(); i++) {
//                    if (AttributeCountIN[label][uniqueIN[label][i]] > threshold) {
//                        count++;
//                        //std::cout << "count2: " << count << "\n";
//                    } else {
//                        count += ((double)AttributeCountIN[label][uniqueIN[label][i]] / (double)threshold);
//                        //std::cout << "count3: " << std::setprecision (17) << count << "\n";
//                    }
//                }
//                threshold++;
//                //std::cout << "threshold: " << threshold << "\n";
//                //std::cout << "count4: " << count << "\n";
//
//            }
//            thresholdsIN[label] = threshold - 1;
//        }
//        //std::cout << "label" << label << "\n";
//    }

     //find thresholds for out
//    for (int label = 0; label < graph.get()->getNoLabels(); label++) {
//        int threshold = 1;
//        double count = graph.get()->getNoVertices();
//        // if the unique number of nodes is smaller then sample size we add them all.
//        if (uniqueOUT[label].size() <= sampleSize) {
//            thresholdsOUT[label] = 1;
//        } else {
//            while(count >= sampleSize) {
//                count = 0;
//                //std::cout << "count1: " << count << "\n";
//                for(int i = 0; i < uniqueOUT[label].size(); i++) {
//                    if (AttributeCountOUT[label][uniqueOUT[label][i]] > threshold) {
//                        count++;
//                        //std::cout << "count2: " << count << "\n";
//                    } else {
//                        count += ((double)AttributeCountOUT[label][uniqueOUT[label][i]] / (double)threshold);
//                        //std::cout << "count3: " << std::setprecision (17) << count << "\n";
//                    }
//                }
//                threshold++;
//                //std::cout << "threshold: " << threshold << "\n";
//                //std::cout << "count4: " << count << "\n";
//
//            }
//            thresholdsOUT[label] = threshold - 1;
//        }
//        //std::cout << "label" << label << "\n";
//    }

//    std::cout << array[17] << " " << array[1] << " " << array[8] << " " << array[9] << "\n";
//    std::cout << "HERE: " << uniqueIN[17].size() << " " << uniqueOUT[17].size() << " " << thresholdsIN[17] << " " << thresholdsOUT[17] << "\n";
//    std::cout << "HERE: " << uniqueIN[1].size() << " " << uniqueOUT[1].size() << " " << thresholdsIN[1] << " " << thresholdsOUT[1] << "\n";
//    std::cout << "HERE: " << uniqueIN[8].size() << " " << uniqueOUT[8].size() << " " << thresholdsIN[8] << " " << thresholdsOUT[8] << "\n";
//    std::cout << "HERE: " << uniqueIN[9].size() << " " << uniqueOUT[9].size() << " " << thresholdsIN[9] << " " << thresholdsOUT[9] << "\n";

}

std::vector<int> SimpleEstimator::estimatePath(RPQTree *q) {
    if(q->isLeaf()){
        std::smatch matches;
        std::regex directLabel (R"((\d+)\+)");
        std::regex inverseLabel (R"((\d+)\-)");
        if(std::regex_search(q->data, matches, directLabel)) {
            auto label = (uint32_t) std::stoul(matches[1]);
            std::vector<int> test {static_cast<int>(uniqueIN[label].size()), static_cast<int>(uniqueOUT[label].size()), array[label], label, 1};
            return test;
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            auto label = (uint32_t) std::stoul(matches[1]);
            std::vector<int> test {static_cast<int>(uniqueOUT[label].size()), static_cast<int>(uniqueIN[label].size()), array[label], label, 0};
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
        // join estimate from slides
        std::vector<int> test {left[0], right[1], (int) std::min((left[2] * (((double)right[2])/((double)right[0]))), right[2] * ((double)left[2]/(double)left[1]))};
        // join estimate from end biased samples for join cardinality estimation
//        int S;
//        // merge on the attribute
//        // both go forwards
//        if(left[4] == 1 && right[4] == 1) {
//            joinAttributes.resize(uniqueOUT[left[3]].size()+uniqueIN[right[3]].size());
//            std::vector<int>::iterator it;
//            it = std::set_union(uniqueOUT[left[3]].begin(), uniqueOUT[left[3]].end(), uniqueIN[right[3]].begin(), uniqueIN[right[3]].end(), joinAttributes.begin());
//            joinAttributes.resize(it-joinAttributes.begin());
//        } else if (left[4] == 1 && right[4] == 0) {
//            joinAttributes.resize(uniqueOUT[left[3]].size()+uniqueOUT[right[3]].size());
//            std::vector<int>::iterator it;
//            it = std::set_union(uniqueOUT[left[3]].begin(), uniqueOUT[left[3]].end(), uniqueOUT[right[3]].begin(), uniqueOUT[right[3]].end(), joinAttributes.begin());
//            joinAttributes.resize(it-joinAttributes.begin());
//        } else if (left[4] == 0 && right[4] == 1) {
//            joinAttributes.resize(uniqueIN[left[3]].size()+uniqueIN[right[3]].size());
//            std::vector<int>::iterator it;
//            it = std::set_union(uniqueIN[left[3]].begin(), uniqueIN[left[3]].end(), uniqueIN[right[3]].begin(), uniqueIN[right[3]].end(), joinAttributes.begin());
//            joinAttributes.resize(it-joinAttributes.begin());
//        } else if (left[4] == 0 && right[4] == 0) {
//            joinAttributes.resize(uniqueIN[left[3]].size()+uniqueOUT[right[3]].size());
//            std::vector<int>::iterator it;
//            it = std::set_union(uniqueIN[left[3]].begin(), uniqueIN[left[3]].end(), uniqueOUT[right[3]].begin(), uniqueOUT[right[3]].end(), joinAttributes.begin());
//            joinAttributes.resize(it-joinAttributes.begin());
//        }
//        // all unique values in the joined attribute.
//        for (int i = 0; i < joinAttributes.size(); i++) {
//            if(left[4] == 1 && right[4] == 1) {
//                int T_a = thresholdsOUT[left[3]];
//                int T_b = thresholdsIN[right[3]];
//                int A_v = AttributeCountOUT[left[3]][joinAttributes[i]];
//                int B_v = AttributeCountIN[right[3]][joinAttributes[i]];
//                if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v*B_v);
//                } else if (A_v < T_a && B_v >= T_b) {
//                    S += (T_a * B_v);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * T_b);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * B_v * std::max(T_a/A_v, T_b/B_v));
//                }
//            } else if (left[4] == 1 && right[4] == 0) {
//                int T_a = thresholdsOUT[left[3]];
//                int T_b = thresholdsOUT[right[3]];
//                int A_v = AttributeCountOUT[left[3]][joinAttributes[i]];
//                int B_v = AttributeCountOUT[right[3]][joinAttributes[i]];
//                if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v*B_v);
//                } else if (A_v < T_a && B_v >= T_b) {
//                    S += (T_a * B_v);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * T_b);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * B_v * std::max(T_a/A_v, T_b/B_v));
//                }
//            } else if (left[4] == 0 && right[4] == 1) {
//                int T_a = thresholdsIN[left[3]];
//                int T_b = thresholdsIN[right[3]];
//                int A_v = AttributeCountIN[left[3]][joinAttributes[i]];
//                int B_v = AttributeCountIN[right[3]][joinAttributes[i]];
//                if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v*B_v);
//                } else if (A_v < T_a && B_v >= T_b) {
//                    S += (T_a * B_v);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * T_b);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * B_v * std::max(T_a/A_v, T_b/B_v));
//                }
//            } else if (left[4] == 0 && right[4] == 0) {
//                int T_a = thresholdsIN[left[3]];
//                int T_b = thresholdsOUT[right[3]];
//                int A_v = AttributeCountIN[left[3]][joinAttributes[i]];
//                int B_v = AttributeCountOUT[right[3]][joinAttributes[i]];
//                if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v*B_v);
//                } else if (A_v < T_a && B_v >= T_b) {
//                    S += (T_a * B_v);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * T_b);
//                } else if (A_v >= T_a && B_v >= T_b) {
//                    S += (A_v * B_v * std::max(T_a/A_v, T_b/B_v));
//                }
//            }
//        }
//        std::vector<int> test {left[0], right[1], S};

        //std::cout << "HERE test 2: " << test[2];
        return test;
    }
    return {};
}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    // perform your estimation here
    // variables to be returned later
//    uint32_t noIn;
//    uint32_t noOut;
//    uint32_t noPaths;

    /* check what the first and last node are in query, because we can assume s and t can be anything for
     * noOut we look at all the edges corresponding to the first label in the query. Similar thing is done for noIn */

//    RPQTree *q_in = q;
//    RPQTree *q_out = q;
//    // retrieve the begin of the query
//    while (q_in->isConcat()) {
//         q_in = q_in->left;
//    }
//    // get the int value of the node and the direction, direction not needed yet.
//    std::string query_begin = q_in->data;
//    int query_begin_edge = atoi(query_begin.substr(0, query_begin.length() - 1).c_str());
//    char query_begin_direction = query_begin.at(query_begin.length() - 1);
//
//    while (q_out->isConcat() == 1) {
//        q_out = q_out->right;
//    }
//    std::string query_end = q_out->data;
//    int query_end_edge = atoi(query_end.substr(0, query_end.length() - 1).c_str());
//    char query_end_direction = query_end.at(query_end.length() - 1);

//    noOut = nr_edges_in[query_begin_edge];
//
//    // for the final node direction does matter.
//    if(query_end_direction == '+') {
//        noIn = nr_edges_out[query_end_edge];
//    } else {
//        noIn = nr_edges_in[query_end_edge];
//    }

//    noOut = graph.get()->getNoEdges()/graph.get()->getNoLabels();
//    noIn = graph.get()->getNoEdges()/graph.get()->getNoLabels();
    std::vector<int> testvar = estimatePath(q);
    testvar[0] = std::min(testvar[0], testvar[2]);
    testvar[1] = std::min(testvar[1], testvar[2]);
    return cardStat {testvar[0],testvar[2],testvar[1]};
}