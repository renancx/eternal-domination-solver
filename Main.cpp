#include "Edge.h"
#include "Graph.h"
#include <exception>
#include <string>
#include <iostream>

using namespace std;

void print_exception(const exception &e, int level = 0) {
    cerr << "exception: " << string(level, ' ') << e.what() << "\n";
    try {
        rethrow_if_nested(e);
    } catch(const std::exception& nested_exception) {
        print_exception(nested_exception, (level + 2));
    }
}

int main (int argc, char* argv[]) {
    try {
        Graph g("graph.txt");
        g.print();
    } catch (const exception& e) {
        print_exception(e);
    }
    return 0;
}