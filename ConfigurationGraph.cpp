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

vector<bool> ConfigurationGraph::findSafeDominatingSets() {
    //vector to store the safe dominating sets
    vector<bool> is_safe(configurations_.size(), true);

    //vector to store the vertices of the ORIGINAL graph for each dominating set
    vector<bool> original_vertices(original_num_vertices_, false);

    bool any_changes = true;

    //while the number of safe dominating sets is greater than 0
    while(any_changes){
        any_changes = false;

        for(size_t i = 0; i < configurations_.size(); i++){
            //verify if the dominating set is safe
            if(is_safe[i]){
                
                //in the original graph, mark the vertices of the dominating set as true
                for(int vertex : configurations_[i]){
                    //for each dominating set, create a vector of bools to store the vertices of the original graph
                    original_vertices[vertex] = true;
                }

                for(int neighbor_set : adjacency_lists_[i]){
                    if (is_safe[neighbor_set]) {
                        // Iterate over the vertices in the neighbor_set and mark the corresponding vertices in the original graph as true
                        for(int vertex : configurations_[neighbor_set]){
                            original_vertices[vertex] = true;
                        }
                    }
                }

                // checking if the dominating set is safe
                // if any of the vertices in the original graph is false, then the dominating set is not safe
                for (size_t j = 0; j < original_vertices.size(); j++) {
                    if (!original_vertices[j]) {
                        is_safe[i] = false;
                        any_changes = true;
                        i = 0;
                        break;
                    }
                }

                //reset the vector of vertices of the original graph
                fill(original_vertices.begin(), original_vertices.end(), false);
            }
        }
    }
    return is_safe;
}

void ConfigurationGraph::printSafeDominatingSets(const vector<vector<int>> &dominating_sets, const vector<bool> &is_safe) {
    cout << "\n-- Safe Dominating Sets of size " << dominating_sets[0].size() << ":\n";

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