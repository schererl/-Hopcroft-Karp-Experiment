#include "graph.hpp"
#include <queue>
#include <iostream>
#include <iomanip>

class BND{
private:
    // For the matchings we have V nodes, where [0, V/2) from M group and [V/2, V) from N group
    vector<list<pair<int, double>>> adj_lst;
    vector<int> matched;
    vector<int> MATCHED;
    int M;
    int V;

    //BKN vars
    double L = numeric_limits<double>::max();
    double H = 0;
    double h = 0;

public:
    BND(int V) : V(V), adj_lst(V), matched(V, -1), MATCHED(V, -1) {}
    // ######################### Hopcroft-Karp stuff ###########################
    void addEdge(int u, int v, double weight) {
        if(v < V/2){ // small change to fit the graph
            v = v+V/2;
        }

        if(weight < L){
            L = weight;
        }
        if(weight > H){
            H = weight;
            h = weight;
        }
        adj_lst[u].emplace_back(make_pair(v, weight));
        adj_lst[v].emplace_back(make_pair(u, weight));
    }

    bool findMultipleAugmentingPaths(const double &bottleneck){
        vector<int>  dist(V+1, 10000000);
        queue<int>   q;
        for(int n = 0; n < V/2;n++){
            if(matched[n] == -1){
                q.push(n);
                dist[n] = 0;
            }
        }
        int cutt_off = 10000001;
        while(!q.empty()){
            int u = q.front();
            q.pop();
            if(dist[u] == dist[V] || dist[u] > cutt_off){ 
                continue;
            }
            
            for(auto &v_tuple : adj_lst[u]){
                int v = v_tuple.first;
                double w = v_tuple.second;
                if(w < bottleneck) continue; // considering bottleneck lowerbound

                if(dist[v] == dist[V]){ // in case matched[v] not visited yet
                    dist[v] = dist[u]+1;
                    if(matched[v]==-1){ // found free v, assign the cuttoff
                        cutt_off = dist[v];
                        continue;
                    }
                    dist[matched[v]] = dist[u]+2;  //move bach to U with matched v
                    q.push(matched[v]);
                }
            }
        }

        vector<bool> active(V, true);
        bool found_path = false;
        for(int n=V/2; n < V;n++){
            if(matched[n]!= -1 || dist[n]==-1) continue;
            int leaf = n;
            int path_len = 0;
            found_path |= extractPath(leaf, dist, active, path_len, bottleneck);
        }
        return found_path;
    }

    bool extractPath(int n, vector<int> &depth, vector<bool> &active, int &path_len, const double &bottleneck){
        // my original code had some flaw I couldn't fix, I got some ideas with https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
        active[n] = false;
        
        //search for reachable edges to v
        for(auto &m_tuple:adj_lst[n]){
            int m   = m_tuple.first;
            double m_w = m_tuple.second;
            if(m_w < bottleneck) continue;

            //found a free variable from u, its over
            if(active[m] && depth[m] == 0){
                active[m] = false;
                matched[m] = n;
                matched[n] = m;
                path_len+=2;
                return true;
            }
            // check for active u
            if(active[m] && depth[n]-1 == depth[m]){
                active[m] = false;
                if(extractPath(matched[m], depth, active, path_len, bottleneck)){
                    matched[m] = n;
                    matched[n] = m;
                    path_len+=2;
                    return true;
                }
            }
        }
        return false;
    }
    
    void maximumMatching(double bottleneck){
        int it = 0;
        fill(matched.begin(), matched.end(), -1);
        while(true){
            if(!findMultipleAugmentingPaths(bottleneck)){
                break;
            }
        }
    }
    

    // ######################### Birkhoff-von Neumann stuff ##############################
    void BNDecomposition(double precision = 0.0001){
        while (h > precision) {
            double bottleneck = findBottleneck();
            cout << "bottleneck: " << bottleneck << endl;
            readMatchings();
            if (bottleneck > 0) {
                decreaseMatchings(bottleneck);
                readMatchings();
            }
            // Wait for the user to press enter to continue to the next iteration
            cout << "Press Enter to continue to the next iteration..." << endl;
            std::cin.get();  // This will pause the program waiting for input
        }
    }
    
    void decreaseMatchings(const double &bottleneck){
        h = 0;
        for (int m = 0; m < V; m++) {
            if (MATCHED[m] == -1) continue;
            int n = MATCHED[m];
            for (auto& edge : adj_lst[m]) {
                if (edge.first == n) {
                    edge.second -= bottleneck;
                    if(h < edge.second){ //update new discounted highest value
                        h = edge.second;
                    }
                    break;
                }
            }
        }
    }

    double findBottleneck(const double precision = 0.00001){
        double low = L;
        double high = H;
        int it = 0;
        double B = 0;
        while (high - low > precision) {
            double b = (low + high) / 2.0;
            cout << "\niteration: " << it << " h:" << high << " l:" << low << " b:" << b << endl;
            if (perfectMatchingExists(b)) {
                cout<< "\tfound perfect matching" << endl;
                low = b;
                B = b;

                //reset perfect matchings
                for(int m = 0; m < V/2;m++){
                    if(MATCHED[m] != -1){ 
                        MATCHED[MATCHED[m]] = -1;
                        MATCHED[m] = -1;
                    }
                }
                // update perfect matchings
                for(int m = 0; m < V/2;m++){
                    if(matched[m] != -1){
                        MATCHED[m] = matched[m];
                        MATCHED[matched[m]] = m;
                    }
                }

            } else {
                cout<< "\tfailed perfect matching" << endl;
                high = b;
            }
            
            cout << '\t';
            readMatchings();
            it+=1;
        }

        return B;
    }
    
    bool perfectMatchingExists(const double &bottleneck) {
        maximumMatching(bottleneck);
        for (int match : matched) {
            if (match == -1) return false;
        }
        return true;
    }

    void readMatchings(){
        cout << "matchings are: ";
        for(int n=0; n < V/2; n++){
            if(MATCHED[n]!=-1){
                //locate weight
                double w =  -1;
                for(auto e:adj_lst[n]){
                    if(e.first == MATCHED[n]){
                        w = e.second;
                        break;
                    }
                }
                cout << "("<< MATCHED[MATCHED[n]] <<","<< MATCHED[n] <<"): " << w << " ";
                
            }
        }
        cout << endl;
    }
};