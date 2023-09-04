#include "Edge.h"
#include "Graph.h"
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

        int k = 3; // dominating set size
        vector<vector<int>> dominating_configs = g.generateDominatingSets(k);

        cout << "Dominating Sets of size " << k << ":\n";
        for (const vector<int>& config : dominating_configs) {
            for (int v : config) {
                cout << v + 1 << " "; 
            }
            cout << "\n";
        }

        for (int i = 0; i < (((int) dominating_configs.size()) - 1); i++) {
            for (int j = (i + 1); j < ((int) dominating_configs.size()); j++) {
                cout << "Is there a guard transition between dominating sets" << endl;
                for (auto &v : dominating_configs[i]) {
                    cout << " " << (v + 1);
                }
                cout << " and";
                for (auto &v : dominating_configs[j]) {
                    cout << " " << (v + 1);
                }
                cout << "?" << endl;
                cout << (g.isGuardTransition(dominating_configs[i], dominating_configs[j], true) ? "Yes" : "No") << endl;
            }
        }
    } catch (const std::exception& e) {
        print_exception(e);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    cout << "Running time: " << duration << " ms\n";

    return 0;
}