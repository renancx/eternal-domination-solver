#ifndef BIPARTITEGRAPH_H

#define BIPARTITEGRAPH_H

#include "Edge.h"
#include <vector>
#include <list>

class BipartiteGraph {
public:
    BipartiteGraph(int num_vertices_set_1, int num_vertices_set_2, std::list<Edge> &edges);

    int numVerticesSet1();
    int numVerticesSet2();
    int numEdges();

    bool hasEdge(Edge e);

    int maxMatching(std::vector<int> &match);

    bool findAugmentingPath(int v, std::vector<int> &match, std::vector<bool> &marked);

    void print();
private:
    int num_vertices_set_1_;
    int num_vertices_set_2_;
    int num_edges_;
    std::vector<std::list<int>> adjacency_lists_;  

    //insert an edge in the graph if the edge does not exist in the graph
    void insertEdge(Edge e);
    
    void validateVertexSet1(int v);
    void validateVertexSet2(int v);
    void validateEdge(Edge e);
};

#endif /* BIPARTITEGRAPH_H */