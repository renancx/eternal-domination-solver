#include "Graph.h"
#include "BipartiteGraph.h"
#include "ConfigurationGraph.h"
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Graph::Graph(int num_vertices) {
    if (num_vertices < 0) {
        throw std::invalid_argument("Invalid number of vertices: " + to_string(num_vertices));
    }

    num_vertices_ = num_vertices;
    num_edges_ = 0;

    adjacency_lists_.resize(num_vertices_);
}

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

bool Graph::isGuardTransition(vector<int> &dominating_set_1, vector<int> &dominating_set_2, bool print_transition) {
    if (dominating_set_1.size() != dominating_set_2.size()) {
        return false;
    }

    int dominating_set_size = ((int) dominating_set_1.size());

    vector<int> order_in_dominating_set_2(num_vertices_, -1);
    int order = -1;
    for (auto &v : dominating_set_2) {
        order++;
        order_in_dominating_set_2[v] = order;
    }

    list<Edge> edges;
    for (int v = 0; v < ((int) dominating_set_1.size()); v++) {
        for (auto &u : adjacency_lists_[dominating_set_1[v]]) {
            if (order_in_dominating_set_2[u] >= 0) {
                edges.push_back(Edge(v, (dominating_set_size + order_in_dominating_set_2[u])));
            }
        }
        if (order_in_dominating_set_2[dominating_set_1[v]] >= 0) {
            edges.push_back(Edge(v, (dominating_set_size + order_in_dominating_set_2[dominating_set_1[v]])));
        }
    }

    BipartiteGraph bipartite_graph(dominating_set_size, dominating_set_size, edges);

    // TODO Remove
    // bipartite_graph.print();

    vector<int> match(2 * dominating_set_size);
    int max_matching_size = bipartite_graph.maxMatching(match);

    if (max_matching_size == dominating_set_size) {
        if (print_transition) {
            cout << "Guard transition:" << endl;
            for (int v = dominating_set_size; v < (2 * dominating_set_size); v++) {
                if (match[v] >= 0) {
                    cout << "Guard on " << (dominating_set_1[match[v]] + 1) <<
                        " moves to " <<
                        (dominating_set_2[v - dominating_set_size] + 1) <<
                        endl;
                }
            }
        }

        return true;
    }

    return false;
}

ConfigurationGraph Graph::generateConfigurationGraph(int k) {
    vector<vector<int>> dominating_configs = generateDominatingSets(k);
    ConfigurationGraph configuration_graph(dominating_configs.size(), num_vertices_, dominating_configs);

    // iterating over all pairs of dominating sets
    for (int i = 0; i < dominating_configs.size(); i++) {
        for (int j = i + 1; j < dominating_configs.size(); j++) {
            // verify if there is a guard transition between the dominating sets
            if (isGuardTransition(dominating_configs[i], dominating_configs[j], false)) {
                configuration_graph.insertEdge(Edge(i, j));
            }
        }
    }
    return std::move(configuration_graph);
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