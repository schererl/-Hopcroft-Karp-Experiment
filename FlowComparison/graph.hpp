#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <list>
#include <chrono>
 
using namespace std; 
class Graph {
private:
    vector<list<int>> adj_lst;
    vector<int> matched;
    int M;
    int V;
    
    // experimental stuff
    chrono::duration<double> elapsed_time;
    int bfs_calls;
    int dfs_calls;
    int bfs_operations;
    int dfs_operations;
    int aug_paths;
    int max_augpath_size;
    int max_augpath_set;

public:
    Graph(int V);
    void addEdge(int u, int v);
    bool findMultipleAugmentingPath();
    bool extractPath(int v, vector<int> &depth, vector<bool> &active, int &path_len);
    void maximumMatching();
    int  countMatchings();
    void readMatchings();
    void readAugpath(list<int> path);
    ~Graph();
};


#endif