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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_filename" << std::endl;
        return 1;
    }

    try {
        Graph g(argv[1]);

        int k = 3; // dominating set size
        vector<vector<int>> dominating_configs = g.generateDominatingSets(k);

        cout << "Dominating Sets of size " << k << ":\n";
        for (const vector<int>& config : dominating_configs) {
            for (int v : config) {
                cout << v + 1 << " "; 
            }
            cout << "\n";
        }

    } catch (const std::exception& e) {
        print_exception(e);
    }
    return 0;
}