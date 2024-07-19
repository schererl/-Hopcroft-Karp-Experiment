#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>


#include "BND.cpp"
int main() {
    BND *g = new BND(6);
    g->addEdge(0, 0, 0.03);
    g->addEdge(0, 1, 0.05);
    g->addEdge(0, 2, 0.003);
    g->addEdge(1, 1, 0.02);
    g->addEdge(1, 2, 0.05);
    g->addEdge(2, 0, 0.012);
    g->addEdge(2, 2, 0.0122);
    g->BNDecomposition();
    
    // double B = g->findBottleneck();
    // cout << "bottleneck: " << B << endl;
    // g->readMatchings();
    // g->decreaseMatchings(B);
    // g->readMatchings();
    

    // double B2 = g->findBottleneck();
    // cout << "bottleneck: " << B2 << endl;
    // g->readMatchings();
    // g->decreaseMatchings(B2);
    // g->readMatchings();

    // double B3 = g->findBottleneck();
    // cout << "bottleneck: " << B3 << endl;
    // g->readMatchings();
    // g->decreaseMatchings(B3);
    // g->readMatchings();

    // int n1, n2, num_edges;
    // std::string line;

    // std::getline(std::cin, line);
    // std::istringstream first_line(line);
    // char p, edge[5];
    // first_line >> p >> edge >> n1 >> n2 >> num_edges;
    // //std::cout << n1 << " " << n2 << std::endl;
    // Graph *g = new Graph(n1 + n2);

    // while (std::getline(std::cin, line)) {
    //     std::istringstream edge_line(line);
    //     char e;
    //     int u, v;
    //     edge_line >> e >> u >> v;
    //     g->addEdge(u, v);
    //     //std::cout << "edge added> " << u << " " << v << std::endl;
    // }

    // g->maximumMatching();
    return 0;
}
