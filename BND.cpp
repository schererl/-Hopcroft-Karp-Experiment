#include "graph.hpp"
#include <queue>
#include <iostream>
#include <iomanip>
#include <algorithm> 
class BND{
struct Edge {
    int m;
    int n;
    double weight;
};
private:
    // For the matchings we have V nodes, where [0, V/2) from M group and [V/2, V) from N group
    vector<vector<Edge*>> adj_lst;
    vector<Edge*> allEdges;
    vector<int> matched;
    vector<int> MATCHED;
    int V; // total vertices
    double H = 0;

public:
    BND(int V) : V(V), adj_lst(V), matched(V, -1), MATCHED(V, -1){}
    ~BND() {
        for (auto& edgeList : adj_lst) {
            for (auto* edge : edgeList) {
                delete edge;
            }
        }
        for (auto* edge : allEdges) {
            delete edge;
        }
    }

    // ######################### Hopcroft-Karp stuff ###########################
    void addEdge(int m, int n, double weight) {
        if (n < V / 2) { // Adjust index range
            n += V / 2;
        }
        if (weight > H) {
            H = weight;
        }

        Edge* edge = new Edge{m, n, weight};
        adj_lst[m].push_back(edge);
        adj_lst[n].push_back(edge);
        allEdges.push_back(edge);
        cout << "edge: " << edge->m << " " << edge->n << " w:" << edge->weight << endl;
    }

    bool findMultipleAugmentingPaths(const double &bottleneck){
        vector<int>  dist(V+1, 10000000);
        queue<int>   q;
        for(int m = 0; m < V/2;m++){
            if(matched[m] == -1){
                q.push(m);
                dist[m] = 0;
            }
        }
        int cutt_off = 10000001;
        while(!q.empty()){
            int m = q.front();
            q.pop();
            if(dist[m] == dist[V] || dist[m] > cutt_off){ 
                continue;
            }
            
            for(auto &edge : adj_lst[m]){
                int n = edge->n;
                double w = edge->weight;
                if(w < bottleneck) continue; // considering bottleneck lowerbound

                if(dist[n] == dist[V]){ // in case matched[n] not visited yet
                    dist[n] = dist[m]+1;
                    if(matched[n]==-1){ // found free n, assign the cuttoff
                        cutt_off = dist[n];
                        continue;
                    }
                    dist[matched[n]] = dist[m]+2;  //move back to M with matched n
                    q.push(matched[n]);
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
        for(auto &edge:adj_lst[n]){
            if(edge->weight < bottleneck) continue;

            //found a free variable from u, its over
            if(active[edge->m] && depth[edge->m] == 0){
                active[edge->m] = false;
                matched[edge->m] = n;
                matched[n] = edge->m;
                path_len+=2;
                return true;
            }
            // check for active u
            if(active[edge->m] && depth[n]-1 == depth[edge->m]){
                active[edge->m] = false;
                if(extractPath(matched[edge->m], depth, active, path_len, bottleneck)){
                    matched[edge->m] = n;
                    matched[n] = edge->m;
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
        while (H > precision) {
            cout << "search for bottleneck " << endl;
            double bottleneck = findBottleneck();
            cout << "BOTTLENECK: " << bottleneck << endl;
            cout << "BEFORE discount\t";
            readMatchings();
            if (bottleneck > 0) {
                decreaseMatchings(bottleneck);
                cout << "AFTER discount\t";
                readMatchings();
            }
            // Wait for the user to press enter to continue to the next iteration
            cout << "Press Enter to continue to the next iteration..." << endl;
            std::cin.get();  // This will pause the program waiting for input
        }
    }
    
    void decreaseMatchings(const double &bottleneck){
        H = 0;
        for (int m = 0; m < V; m++) {
            if (MATCHED[m] == -1) continue;
            int match_n = MATCHED[m];
            for (auto& edge : adj_lst[m]) {
                if (edge->n == match_n) {
                    edge->weight -= bottleneck;
                    if(H < edge->weight){ //update new discounted highest value
                        H = edge->weight;
                    }
                    break;
                }
            }
        }
    }

    double findBottleneck(){
        std::sort(allEdges.begin(), allEdges.end(), [](const Edge* a, const Edge* b) { return a->weight < b->weight;});
        cout << "sorted" << endl;
        for(auto e : allEdges){ cout << " " << e->weight;}
        cout << endl;
        int low = 0;
        for(auto e : allEdges){ 
            if(e->weight != 0){
                break;
            }
            low+=1;
        }
        int high = allEdges.size();
        int it = 0;
        double B = 0;
        while (high > low) {
            int mid = (high+low)/2;
            cout << " NEW MID " << mid << endl;
            double b = allEdges[mid]->weight;
            
            cout << "\niteration " << it << " => h:" << high << " l:" << low << " m:" << mid << " bottleneck: " << b << endl;
            if (perfectMatchingExists(b)) {
                cout<< "\tfound perfect matching" << endl;
                low = mid+1;
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
                high = mid-1;
                cout<< "\tfailed perfect matching" << endl;
            }
            
            cout << '\t';
            cout<< "\tnew bounds: high " << high << " low " << low << endl;
            readMatchings();
            it+=1;
        }
        cout << "returning bottleneck " << B << endl;
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
        for(int m=0; m < V/2; m++){
            if(MATCHED[m]!=-1){
                //locate weight
                double w =  -1;
                for(auto e:adj_lst[m]){
                    if(e->n == MATCHED[m]){
                        w = e->weight;
                        break;
                    }
                }
                cout << "("<< MATCHED[m] <<","<< MATCHED[MATCHED[m]] <<", " << w << ") ";
                
            }
        }
        cout << endl;
    }
};