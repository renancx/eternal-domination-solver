#ifndef GRAPH_H

#define GRAPH_H

#include "Edge.h"
#include <vector>
#include <list>

class Graph {
public:
    //build a simple graph that has the number of vertices received by parameter and that has no edges
    Graph(int num_vertices);

    //build a graph from a file that has the following format:
    //p edge <number of vertices> <number of edges>
    Graph(const std::string& filename);

    int numVertices();
    int numEdges();

    bool hasEdge(Edge e);

    //insert an edge in the graph if the edge does not exist in the graph and is not a loop
    void insertEdge(Edge e);

    //remove an edge from the graph if the edge exists in the graph
    void removeEdge(Edge e);

    void print();
private:
    //attributes of the class Graph will have the suffix _ (underscore) to differentiate from the parameters
    int num_vertices_;
    int num_edges_;
    std::vector<std::list<int>> adjacency_lists_;  
    
    void validateVertex(int v);
    void validateEdge(Edge e);
};

#endif /* GRAPH_H */