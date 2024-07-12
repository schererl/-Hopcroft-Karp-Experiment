#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    int n1, n2, num_edges;
    std::string line;

    std::getline(std::cin, line);
    std::istringstream first_line(line);
    char p, edge[5];
    first_line >> p >> edge >> n1 >> n2 >> num_edges;
    std::cout << n1 << " " << n2 << std::endl;
    Graph *g = new Graph(n1 + n2);

    while (std::getline(std::cin, line)) {
        std::istringstream edge_line(line);
        char e;
        int u, v;
        edge_line >> e >> u >> v;
        g->addEdge(u, v);
        std::cout << "edge added> " << u << " " << v << std::endl;
    }

    auto maximal_augpaths = g->findMultipleAugmentingPath();
    for(auto aug_path: maximal_augpaths){
        g->symmetricDifference(aug_path);
    }
    g -> readMatchings();
    
    return 0;
}
