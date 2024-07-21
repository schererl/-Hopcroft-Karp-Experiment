#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "BvND.cpp"

int main() {
    std::cout << "Running Birkhoff-von Neumann Decomposition..." << std::endl;

    int numRows, numCols, numEntries;
    std::string line;

    // Read the header line
    std::getline(std::cin, line);
    std::istringstream header(line);
    std::string matrixMarket, matrixType, format, field, symmetry;
    header >> matrixMarket >> matrixType >> format >> field >> symmetry;
    
    std::getline(std::cin, line);
    std::istringstream sizeLine(line);
    sizeLine >> numRows >> numCols >> numEntries;
    std::cout << "Header read: " << line << std::endl;
    std::cout << "numRows: " << numRows << ", numCols: " << numCols << ", numEntries: " << numEntries << std::endl;
    
    BvND* g = new BvND(numRows + numCols);
    while (std::getline(std::cin, line)) {
        std::istringstream edgeLine(line);
        int m_vertex, n_vertex;
        double weight;
        edgeLine >> m_vertex >> n_vertex >> weight;
        m_vertex--; n_vertex--; // convert from 1-based to 0-based indexing if necessary
        g->addEdge(m_vertex, n_vertex, weight);
    }
    g->BNDecomposition();

    return 0;
}
