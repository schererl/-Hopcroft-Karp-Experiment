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
    int max_aug_path;
    int sum_aug_paths;

public:
    Graph(int V);
    void addEdge(int u, int v);
    list<int> findAugmentingPath();
    list<list<int>> findMultipleAugmentingPath();
    void generateDisjointPaths(vector<int>& parent, list<list<int>>& disjoint_paths);
    void extractPath(int u, list<int> &path, vector<int> &depth, vector<bool> &active, bool &found_path);
    void maximumMatching();
    void symmetricDifference(list<int> path);
    void readMatchings();
    ~Graph();
};


#endif