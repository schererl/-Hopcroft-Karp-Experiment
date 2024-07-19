#include "graph.hpp"
#include <list>
#include <queue>
#include <climits>
#include <tuple>
#include <complex>
#include <vector>

#include <fstream>
#include <iostream>
#include<unistd.h> 
Graph::Graph(int V):V(V), M(0), adj_lst(V), matched(V,-1), bfs_calls(0), dfs_calls(0), bfs_operations(0), dfs_operations(0), aug_paths(0), max_aug_path(0), sum_aug_paths(0) {

}

void Graph::addEdge(int u, int v){
    M+=1;
    adj_lst[u].push_back(v);
    adj_lst[v].push_back(u);
}

void Graph::maximumMatching(){
    auto start_global = std::chrono::high_resolution_clock::now();
    int it = 0;
    while(true){
        auto start = std::chrono::high_resolution_clock::now();
        aug_paths+=1;
        if(!findMultipleAugmentingPath()){
            break;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        int match_count = 0;
        for(int n=0; n < V/2; n++){
            if(matched[n]!=-1){
                match_count+=1;
            }
        }
        cout << "iteration " << it << " ended after " << elapsed.count() << " seconds with " << match_count << " matchings." << endl;
        it++;
    }
    auto end_global = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_global - start_global;
    
    cout << "Algorithm ended after: " << it << " iterations. " << endl;
    cout << "Elapsed time: " <<  elapsed.count() << " seconds. " << endl; 
    cout << "Total matchings: " << countMatchings() << " matchings." << endl;
    cout << "Total augmenting paths: " << sum_aug_paths << endl;
    cout << "Maximum size augmenting path: " << max_aug_path << endl;
    cout << "BFS calls: " << bfs_calls << endl;
    cout << "BFS operations: " << bfs_operations << endl;
    cout << "DFS calls: " << dfs_calls << endl;
    cout << "DFS operations: " << dfs_operations << endl;
}


int Graph::countMatchings(){
    int match_count = 0;
    for(int n=0; n < V/2; n++){
        if(matched[n]!=-1){
            match_count+=1;
        }
    }
    return match_count;
}

bool Graph::findMultipleAugmentingPath(){
    // my original code had some flaw I couldn't fix, I got some ideas with https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
    vector<int>  dist(V+1, 10000000);
    queue<int>   q;
    for(int n = 0; n < V/2;n++){
        if(matched[n] == -1){
            q.push(n);
            dist[n] = 0;
        }
    }
    bfs_calls += 1;
    int cutt_off = 10000001;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        if(dist[u] == dist[V] || dist[u] > cutt_off){ 
            continue;
        }
        
        for(int v : adj_lst[u]){
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
        dfs_calls+=1;
        int path_len = 0;
        found_path |= extractPath(leaf, dist, active, path_len);
        if(path_len > 0){
            sum_aug_paths+=1;
            if(path_len > max_aug_path){
                max_aug_path=path_len;
            }
        }
        
    }
    return found_path;
}

bool Graph::extractPath(int n, vector<int>& depth, vector<bool>& active, int& path_len) {
    // my original code had some flaw I couldn't fix, I got some ideas with https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
    dfs_operations+=1;
    active[n] = false;
    
    //search for reachable edges to v
    for(int m:adj_lst[n]){
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
            if(extractPath(matched[m], depth, active, path_len)){
                matched[m] = n;
                matched[n] = m;
                path_len+=2;
                return true;
            }
        }
    }
    return false;
}

void Graph::readMatchings(){
    cout << "matchings are: ";
    for(int n=0; n < V/2; n++){
        if(matched[n]!=-1){
            cout << "("<< matched[matched[n]] <<","<< matched[n] <<")";
        }
    }
    cout << endl;
}

void Graph::readAugpath(list<int> path){
    cout << "Augmenting path is: ";
    for(int v : path){
        cout << v << " ";
        
    }
    cout << endl;
}

Graph::~Graph() {
    for (auto &list : adj_lst) {
        list.clear(); 
    }
}

