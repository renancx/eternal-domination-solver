#include "Edge.h"
#include "Graph.h"
#include "ConfigurationGraph.h"
#include <exception>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

void print_exception(const exception &e, int level = 0) {
    cerr << "exception: " << string(level, ' ') << e.what() << "\n";
    try {
        rethrow_if_nested(e);
    } catch(const std::exception& nested_exception) {
        print_exception(nested_exception, (level + 2));
    }
}

int main(int argc, char* argv[]) {

    auto start_time = std::chrono::high_resolution_clock::now();

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_filename" << std::endl;
        return 1;
    }

    try {
        Graph g(argv[1]);
        
        g.findMinimumGuardSet();

    } catch (const std::exception& e) {
        print_exception(e);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    cout << "Running time: " << duration << " ms" << endl;

    return 0;
}