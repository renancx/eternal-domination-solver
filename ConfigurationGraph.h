#ifndef CONFIGURATIONGRAPH_H
#define CONFIGURATIONGRAPH_H

#include "Edge.h"
#include <vector> 
#include <list>

class ConfigurationGraph {
public:
    ConfigurationGraph(int num_vertices, int original_num_vertices, std::vector<std::vector<int>> &configurations);

    std::vector<bool> findSafeDominatingSets();

    void printSafeDominatingSets(const std::vector<std::vector<int>> &dominating_sets, const std::vector<bool> &is_safe);

    int numVertices();
    int numEdges();

    bool hasEdge(Edge e);

    void insertEdge(Edge e);

    void removeEdge(Edge e);

    void print();

private:
    int num_vertices_;
    int num_edges_;
    std::vector<std::list<int>> adjacency_lists_;

    int original_num_vertices_;
    std::vector<std::vector<int>> configurations_;

    void validateVertex(int v);
    void validateEdge(Edge e);
};

#endif /* CONFIGURATIONGRAPH_H */