#include "BipartiteGraph.h"
#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

BipartiteGraph::BipartiteGraph(int num_vertices_set_1, int num_vertices_set_2, list<Edge> &edges) {
    if (num_vertices_set_1 < 0) {
        throw invalid_argument("Invalid first set number of vertices: " + to_string(num_vertices_set_1));
    }

    if (num_vertices_set_2 < 0) {
        throw invalid_argument("Invalid second set number of vertices: " + to_string(num_vertices_set_2));
    }

    num_vertices_set_1_ = num_vertices_set_1;
    num_vertices_set_2_ = num_vertices_set_2;
    num_edges_ = 0;

    adjacency_lists_.resize(num_vertices_set_1_ + num_vertices_set_2_);

    for (auto &e : edges) {
        insertEdge(e);
    }
}

int BipartiteGraph::numVerticesSet1() {
    return num_vertices_set_1_;
}

int BipartiteGraph::numVerticesSet2() {
    return num_vertices_set_2_;
}

int BipartiteGraph::numEdges() {
    return num_edges_;
}

bool BipartiteGraph::hasEdge(Edge e) {
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

int BipartiteGraph::maxMatching(vector<int> &match) {
    match.assign((num_vertices_set_1_ + num_vertices_set_2_), -1);

    vector<bool> marked(num_vertices_set_1_);

    int matching_size = 0;
    for (int v = 0; v < num_vertices_set_1_; v++) {
        marked.assign(num_vertices_set_1_, false);

        if (findAugmentingPath(v, match, marked)) {
            matching_size++;
        }
    }

    return matching_size;
}

bool BipartiteGraph::findAugmentingPath(int v, vector<int> &match, vector<bool> &marked) {
    if (marked[v]) {
        return false;
    }

    marked[v] = true;

    for (auto &u : adjacency_lists_[v]) {
        if ((match[u] == -1) || findAugmentingPath(match[u], match, marked)) {
            match[u] = v;
            return true;
        }
    }

    return false;
}

void BipartiteGraph::print() {
    for (auto v = 0; v < (num_vertices_set_1_ + num_vertices_set_2_); v++) {
        cout << v + 1 << ":"; // vertices are printed from 1 to n
        for (auto u : adjacency_lists_[v]) {
            cout << " " << u + 1;
        }
        cout << "\n";
    }
}

void BipartiteGraph::insertEdge(Edge e) {
    try {
        validateEdge(e);
    } catch (...) {
        throw_with_nested(runtime_error("Error in operation "
            "insertEdge(Edge): the edge " + e.to_string() + " is "
            "invalid!"));
    }

    // TODO Remove check if edge already exists
    if (!hasEdge(e) && (e.v1 != e.v2)) {
        adjacency_lists_[e.v1].push_front(e.v2);
        adjacency_lists_[e.v2].push_front(e.v1);

        num_edges_++;
    }
}

void BipartiteGraph::validateVertexSet1(int v) {
    if ((v < 0) || (v >= (num_vertices_set_1_))) {
        throw out_of_range("Invalid first set vertex index: " + to_string(v));
    }
}

void BipartiteGraph::validateVertexSet2(int v) {
    if ((v < num_vertices_set_1_) || (v >= (num_vertices_set_1_ + num_vertices_set_2_))) {
        throw out_of_range("Invalid second set vertex index: " + to_string(v));
    }
}

void BipartiteGraph::validateEdge(Edge e) {
    if (e.v1 == e.v2) {
        throw invalid_argument("Invalid edge: " + e.to_string());
    }

    if (e.v1 < e.v2) {
        validateVertexSet1(e.v1);
        validateVertexSet2(e.v2);
    }
    else {
        validateVertexSet1(e.v2);
        validateVertexSet2(e.v1);
    }
}