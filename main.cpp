#include "graph.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "BND.cpp"

int main(int argc, char* argv[]) {
    if(argc < 2){
        std::cout << "Operation type required: \n(1) Birkhoff-von Neumann Decomposition.\n(2) Efficiency comparison with Edmonds-Karp." << std::endl;
        return -1;
    }

    int operationType = std::stoi(argv[1]); 
    
    if(operationType == 1){
        // BND *g = new BND(6);
        // g->addEdge(0, 0, 0.5);
        // g->addEdge(0, 1, 0.25);
        // g->addEdge(0, 2, 0.25);
        // g->addEdge(1, 1, 0.75);
        // g->addEdge(2, 2, 1.0);
        
        int numRows, numCols, numEntries;
        std::string line;

        std::getline(std::cin, line);
        std::istringstream header(line);
        std::string matrixType;
        header >> matrixType;
        header >> matrixType;
        header >> numRows >> numCols >> numEntries;
        
        BND* g = new BND(numRows+numCols);
        while (std::getline(std::cin, line)) {
            std::istringstream edgeLine(line);
            int m_vertex, n_vertex;
            double weight;
            edgeLine >> m_vertex >> n_vertex >> weight;
            m_vertex--; n_vertex--; // convert from 1-based to 0-based indexing if necessary
            g->addEdge(m_vertex, n_vertex, weight);
        }
        g->BNDecomposition();
    }
    else{
        int n1, n2, num_edges;
        std::string line;

        std::getline(std::cin, line);
        std::istringstream first_line(line);
        char p, edge[5];
        first_line >> p >> edge >> n1 >> n2 >> num_edges;
        //std::cout << n1 << " " << n2 << std::endl;
        Graph *g = new Graph(n1 + n2);

        while (std::getline(std::cin, line)) {
            std::istringstream edge_line(line);
            char e;
            int u, v;
            edge_line >> e >> u >> v;
            g->addEdge(u, v);
            //std::cout << "edge added> " << u << " " << v << std::endl;
        }

        g->maximumMatching();
    }
    return 0;
}
