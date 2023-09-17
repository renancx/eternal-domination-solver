#ifndef CONFIGURATIONGRAPH_H
#define CONFIGURATIONGRAPH_H

#include "Edge.h"
#include <vector>
#include <list>

class ConfigurationGraph {
public:
    ConfigurationGraph(int num_vertices, int original_num_vertices, std::vector<std::vector<int>> &configurations);

    std::vector<bool> findSafeDominatingSets(const std::vector<std::vector<int>> &dominating_sets);

    void printSafeDominatingSets(const std::vector<std::vector<int>> &dominating_sets, const std::vector<bool> &is_safe);

    int numVertices();
    int numEdges();

    bool hasEdge(Edge e);

    void insertEdge(Edge e);

    void removeEdge(Edge e);

    void print();

    void markCurrentVertices(const std::vector<int> &dominating_set, std::vector<bool> &current_vertices_);

    void markDefendedVertices(const std::vector<int> &dominating_set, std::vector<bool> &defended_vertices_);

    bool canDefendAllVertices(const std::vector<int> &dominating_set1, const std::vector<int> &dominating_set2, std::vector<bool> &defended_vertices_);
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