#include "Edge.h"
#include "Graph.h"
#include "ConfigurationGraph.h"
#include <exception>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;

void print_exception(const exception &e, int level = 0) {
    cerr << "exception: " << string(level, ' ') << e.what() << "\n";
    try {
        rethrow_if_nested(e);
    } catch(const std::exception& nested_exception) {
        print_exception(nested_exception, (level + 2));
    }
}

void runGraphProcessing(const string& inputFilename, atomic<bool>& graphFinished) {
    string instance = inputFilename.substr(inputFilename.find_last_of("/\\") + 1);
    instance = instance.substr(0, instance.find_last_of("."));

    cout << "Instance: " << instance << endl;
    try {
        Graph g(inputFilename);
        g.findMinimumGuardSet();
        graphFinished = true; //If the graph finish the execution
    } catch (const std::exception& e) {
        print_exception(e);
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_filename" << std::endl;
        return 1;
    }

    atomic<bool> graphFinished(false);

    const int max_time_in_seconds = 7200;

    std::thread processingThread(runGraphProcessing, argv[1], ref(graphFinished));

    auto startTime = chrono::steady_clock::now();
    while (true) {
        auto currentTime = chrono::steady_clock::now();
        auto elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(currentTime - startTime);

        if (graphFinished) {
            cout << "Running time: " << elapsedMilliseconds.count() << " ms" << endl;
            break;
        }

        if (elapsedMilliseconds.count() > max_time_in_seconds * 1000) {
            cout << "Time limit exceeded: " <<  elapsedMilliseconds.count() << " ms" << endl;
            return 0;
        }
    }

    //Wait until the thread process ends
    processingThread.join();

    return 0;
}