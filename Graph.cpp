#include "Graph.h"
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include <string>
#include <iostream>

using namespace std;

Graph::Graph(int num_vertices) {
    if (num_vertices <= 0) {
        throw(invalid_argument("Error in constructor Graph(int): the number of "
            "vertices " + to_string(num_vertices) + " is invalid!"));
    }
    num_vertices_ = num_vertices;
    num_edges_ = 0;

    adjacency_lists_.resize(num_vertices);
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
        cout << v << ":";
        for (auto u : adjacency_lists_[v]) {
            cout << " " << u;
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