#include "Edge.h"
#include <string>

Edge::Edge(int v1, int v2) : v1(v1), v2(v2) {
}

std::string Edge::to_string() {
    return std::to_string(v1) + " " + std::to_string(v2);
}