#include "graph.hpp"

class BKN{
private:
    // For the matchings we have V nodes, where [0, V/2) from M group and [V/2, V) from N group
    vector<list<pair<int, double>>> adj_lst;
    vector<int> matched;
    int M;
    int V;

    //BKN vars
    double L = numeric_limits<double>::max();
    double H = 0;

public:
    BKN(int V) : V(V), adj_lst(V), matched(V, -1) {}
    // ######################### Hopcroft-Karp stuff ###########################
    void addEdge(int u, int v, double weight) {
        if(weight < L){
            L = weight;
        }
        if(weight > H){
            H = weight;
        }
        adj_lst[u].emplace_back(make_pair(v, weight));
        adj_lst[v].emplace_back(make_pair(u, weight));
    }

    list<int> extractAugmentingPath();
    bool findMultipleAugmentingPaths(double bottleneck){return true;}
    bool extractPath(int v, vector<int> &depth, vector<bool> &active, int &path_len){}
    
    void maximumMatching(double bottleneck){
        int it = 0;
        fill(matched.begin(), matched.end(), -1);
        while(true){
            if(!findMultipleAugmentingPaths(bottleneck)){
                break;
            }
        }
    }
    

    //  ######################### Birkhoff-von Neumann stuff ##############################
    void BNDecomposition(double precision = 0.0001){
        double bottleneck = findBottleneck();
        while (bottleneck > precision) {
            if (perfectMatchingExists(bottleneck)) {
                decreaseMatchings(bottleneck);
            }
            bottleneck = findBottleneck();
        }
    }
    
    void decreaseMatchings(double const &bottleneck){
        for (int m = 0; m < V/2; m++) {
            if (matched[m] != -1) {
                int n = matched[m];
                for (auto& edge : adj_lst[m]) {
                    if (edge.first == n) {
                        edge.second -= bottleneck;
                        break;
                    }
                }
            }
        }
    }

    double findBottleneck(double precision = 0.0001){
        double low = L;
        double high = H;

        while (high - low > precision) {
            double bottleneck = (low + high) / 2.0;
            if (perfectMatchingExists(bottleneck)) {
                low = bottleneck;
            } else {
                high = bottleneck;
            }
        }

        return low;
    }
    
    bool perfectMatchingExists(int bottleneck) {
        maximumMatching(bottleneck);
        for (int match : matched) {
            if (match == -1) return false;
        }
        return true;
    }
};