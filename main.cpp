#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>

int main() {
    Graph *g = new Graph(8);
    g->addEdge(0,4);
    g->addEdge(3,6);
    g->addEdge(0,5);
    g->addEdge(1,4);
    g->addEdge(1,7);
    g->addEdge(2,6);
    g->addEdge(3,7);
    
    
    g->findMultipleAugmentingPath();
    
    return 0;
}
