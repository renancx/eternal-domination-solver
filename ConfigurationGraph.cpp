#include "ConfigurationGraph.h"
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

ConfigurationGraph::ConfigurationGraph(int num_vertices, int original_num_vertices, vector<vector<int>> &configurations) 
    : num_vertices_(num_vertices), original_num_vertices_(original_num_vertices), configurations_(configurations) {
    if (num_vertices < 0) {
        throw invalid_argument("Invalid number of vertices: " + to_string(num_vertices));
    }

    num_edges_ = 0;
    adjacency_lists_.resize(num_vertices_);
}

//function will recieve the dominating sets of size k
vector<bool> ConfigurationGraph::findSafeDominatingSets(const vector<vector<int>>& dominating_sets) {
    
    //vector of bools to check if a dominating set is safe
    //initially all sets are safe
    vector<bool> is_safe(dominating_sets.size(), true);

    //for each dominating set
    bool any_changes = true;

    //while the number of safe dominating sets is > 0
    while(any_changes){
        any_changes = false;

        //for each dominating set
        for(size_t i = 0; i < dominating_sets.size(); i++){
            //verify if the dominating set is safe
            if(is_safe[i]){

                //vector of bools to check if a vertex is defended
                //initially all vertices are not defended
                vector<bool> defended_vertices_(num_vertices_, false);

                //all vertices from dominating_set[i] are defended
                for(int v : dominating_sets[i]){
                    defended_vertices_[v] = true;
                    //cout << v + 1 << " "; THIS SEEMS TO BE WORKING
                }

                for(size_t j = 0; j < dominating_sets.size(); j++){
                    //if the dominating set is safe and is not the same as the dominating set we are checking
                    if (i != j && is_safe[j]) {
                        //for each vertex in dominating_set[j]
                        for (int v : dominating_sets[j]) {
                            //if the vertex is not defended
                            defended_vertices_[v] = true;
                            //cout << v + 1 << " ";
                        }
                    }
                }

                //check if all configurations are defended
                bool all_configurations_are_defended = true;
                for (bool covered : defended_vertices_) {
                    if (!covered) {
                        all_configurations_are_defended = false;
                        break;
                    }
                }

                if(!all_configurations_are_defended){
                    is_safe[i] = false;
                    any_changes = true;
                }
            }
        }
    }
    return is_safe;
}

//------------------------------------------------------------------------------

void ConfigurationGraph::printSafeDominatingSets(const vector<vector<int>> &dominating_sets, const vector<bool> &is_safe) {
    cout << "Safe Dominating Sets of size " << dominating_sets[0].size() << ":\n";

    for (size_t i = 0; i < dominating_sets.size(); i++) {
        if (is_safe[i]) {
            cout << "Set " << (i + 1) << ": ";
            for (int vertex : dominating_sets[i]) {
                cout << vertex + 1 << " ";
            }
            cout << endl;
        }
    }
}

int ConfigurationGraph::numVertices() {
    return num_vertices_;
}

int ConfigurationGraph::numEdges() {
    return num_edges_;
}

bool ConfigurationGraph::hasEdge(Edge e) {
    try {
        validateEdge(e);
    } catch (...) {
        throw_with_nested(runtime_error("Error in operation hasEdge(Edge):"
            " the edge " + e.to_string() + " is invalid!"));
    }

    for (auto u : adjacency_lists_[e.v1]) {
        if (u == e.v2) {
            return true;
        }
    }
    return false;
}

void ConfigurationGraph::insertEdge(Edge e) {
    try {
        validateEdge(e);
    } catch (...) {
        throw_with_nested(runtime_error("Error in operation "
            "insertEdge(Edge): the edge " + e.to_string() + " is "
            "invalid!"));
    }
    if (!hasEdge(e) && (e.v1 != e.v2)) {
        adjacency_lists_[e.v1].push_front(e.v2);
        adjacency_lists_[e.v2].push_front(e.v1);

        num_edges_++;
    }
}

void ConfigurationGraph::removeEdge(Edge e) {
    try {
        validateEdge(e);
    } catch (...) {
        throw_with_nested(runtime_error("Error in operation "
            "removeEdge(Edge): the edge " + e.to_string() + " is "
            "invalid!"));
    }
    bool graph_has_edge = false;

    auto it = adjacency_lists_[e.v1].begin();
    while (it != adjacency_lists_[e.v1].end()) {
        if (*it == e.v2) {
            adjacency_lists_[e.v1].erase(it);
            graph_has_edge = true;
            break;
        }
        it++;
    }

    if (graph_has_edge) {
        auto it = adjacency_lists_[e.v2].begin();
        while (it != adjacency_lists_[e.v2].end()) {
            if (*it == e.v1) {
                adjacency_lists_[e.v2].erase(it);
                break;
            }
            it++;
        }
        num_edges_--;
    }
}

void ConfigurationGraph::print() {
    for (auto v = 0; v < num_vertices_; v++) {
        cout << v  + 1 << ":"; // vertices are numbered from 1 to n in the file format
        for (auto u : adjacency_lists_[v]) {
            cout << " " << u + 1;
        }
        cout << "\n";
    }
}

void ConfigurationGraph::validateVertex(int v) {
    if ((v < 0) || (v >= num_vertices_)) {
        throw out_of_range("Invalid vertex index: " + to_string(v));
    }
}

void ConfigurationGraph::validateEdge(Edge e) {
    validateVertex(e.v1);
    validateVertex(e.v2);
}