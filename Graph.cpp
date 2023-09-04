#include "Graph.h"
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Graph::Graph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    int num_vertices, num_edges;

    // Read the first line to get the number of vertices and edges
    getline(file, line);
    if (line.substr(0, 2) != "p ") {
        throw std::invalid_argument("Invalid file format: " + filename);
    }
    if (sscanf(line.c_str(), "p edge %d %d", &num_vertices, &num_edges) != 2) {
        throw std::invalid_argument("Invalid file format: " + filename);
    }

    if (num_vertices <= 0 || num_edges < 0) {
        throw std::invalid_argument("Invalid graph data in file: " + filename);
    }

    num_vertices_ = num_vertices;
    num_edges_ = 0;

    adjacency_lists_.resize(num_vertices);

    // Read and insert edges
    for (int i = 0; i < num_edges; ++i) {
        getline(file, line);
        if (line.substr(0, 2) != "e ") {
            throw std::invalid_argument("Invalid file format: " + filename);
        }
        int v1, v2;
        if (sscanf(line.c_str(), "e %d %d", &v1, &v2) != 2) {
            throw std::invalid_argument("Invalid file format: " + filename);
        }
        Edge edge(v1 - 1, v2 - 1);
        insertEdge(edge);
    }

    file.close();
}

vector<vector<int>> Graph::generateDominatingSets(int k) {
    vector<vector<int>> dominating_sets; // stores the generated dominating sets
    vector<int> dcurrent_set; // stores the current dominating set temporarily

    exploreCombinations(0, k, dcurrent_set, dominating_sets);

    // avoid unnecessary copies
    return std::move(dominating_sets); // return all the generated dominating sets
}

bool Graph::isDominatingSet(vector<int>& set) {
    vector<bool> is_dominated(num_vertices_, false); // all vertices are initially not dominated (false)
    for (auto v : set) {  // all vertices in the dominating set are dominated (true)
        is_dominated[v] = true;
        for (auto u : adjacency_lists_[v]) { // all vertices adjacent to a vertex in the dominating set are dominated (true)
            is_dominated[u] = true;
        }
    }

    for (auto v = 0; v < num_vertices_; v++) { // if there is a vertex that is not dominated, the set is not a dominating set
        if (!is_dominated[v]) {
            return false;
        }
    }
    return true;
}

// generate all the dominating sets of size k recursively
void Graph::exploreCombinations(int current_vertex, int k, vector<int>& current_set, vector<vector<int>>& dominating_sets) {
    if (current_set.size() == k) {
        if(isDominatingSet(current_set)) {
            dominating_sets.push_back(current_set);
        }
        return;
    }

    for (int v = current_vertex; v < num_vertices_; v++) {
        current_set.push_back(v);
        exploreCombinations(v + 1, k, current_set, dominating_sets);
        current_set.pop_back();
    }
}

int Graph::numVertices() {
    return num_vertices_;
}

int Graph::numEdges() {
    return num_edges_;
}

bool Graph::hasEdge(Edge e) {
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

void Graph::insertEdge(Edge e) {
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

void Graph::removeEdge(Edge e) {
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

void Graph::print() {
    for (auto v = 0; v < num_vertices_; v++) {
        cout << v + 1 << ":"; // vertices are numbered from 1 to n in the file format
        for (auto u : adjacency_lists_[v]) {
            cout << " " << u + 1;
        }
        cout << "\n";
    }
}

void Graph::validateVertex(int v) {
    if ((v < 0) || (v >= num_vertices_)) {
        throw out_of_range("Invalid vertex index: " + to_string(v));
    }
}

void Graph::validateEdge(Edge e) {
    validateVertex(e.v1);
    validateVertex(e.v2);
}