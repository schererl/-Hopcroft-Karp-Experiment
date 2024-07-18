#include "graph.hpp"
#include <iostream>
#include <list>
#include<queue>
#include <climits>
#include <tuple>
#include <complex>
#include <iostream>
#include <vector>
#include <fstream>


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
        auto maximal_augpaths = findMultipleAugmentingPath();
        
        if(maximal_augpaths.size() == 0) break;
        sum_aug_paths += maximal_augpaths.size();
        //cout << "###### new iteration ######" << endl;
        for(auto aug_path: maximal_augpaths){
            //cout << "new aug path: ";
            //cout << "\t";
            //readAugpath(aug_path);
            //readMatchings();
            symmetricDifference(aug_path);
            //readMatchings();

            if(max_aug_path < aug_path.size()){
                max_aug_path = aug_path.size();
            }

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
    int match_count = 0;
    for(int n=0; n < V/2; n++){
        if(matched[n]!=-1){
            match_count+=1;
        }
    }
    cout << "Algorithm ended after: " << it << " iterations. " << endl;
    cout << "Elapsed time: " <<  elapsed.count() << " seconds. " << endl; 
    cout << "Total matchings: " << match_count << " matchings." << endl;
    cout << "Total augmenting paths: " << sum_aug_paths << endl;
    cout << "Maximum size augmenting path: " << max_aug_path << endl;
    cout << "BFS calls: " << bfs_calls << endl;
    cout << "BFS operations: " << bfs_operations << endl;
    cout << "DFS calls: " << dfs_calls << endl;
    cout << "DFS operations: " << dfs_operations << endl;
}



void Graph::symmetricDifference(list<int> aug_path){
    //update matchings
    while(!aug_path.empty()){
        int u = aug_path.front();
        aug_path.pop_front();
        int v = aug_path.front();
        aug_path.pop_front();
        //cout << "u: " << u << " v: " << v << endl;
        if(matched[u] != -1){
            //cout << "\t u was matched with " << matched[u] << endl;
            matched[matched[u]] = -1;
            matched[u] = -1;
        }
        if(matched[v] != -1){
            //cout << "\t v was matched with " << matched[v] << endl;
            matched[matched[v]] = -1;
            matched[v] = -1;
        }
        matched[u] = v;
        matched[v] = u;
        //cout << "\t new match formed (" << matched[u] << "," << matched[v] << ")" << endl;
    }
}

list<list<int>> Graph::findMultipleAugmentingPath(){
    vector<bool> visited(V, false);
    vector<int>  depth(V, -1);
    queue<int>   q;
    // get free vertices from M group
    //cout << "free vertices from M: ";
    for(int n = 0; n < V/2;n++){
        if(matched[n] == -1){
            q.push(n);
            depth[n] = 0;
            //cout << n << " ";
        }
    }
    //cout << endl;
    
    int cutoff_depth = V;
    bfs_calls += 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        visited[u] = true;
        if(depth[u] >= cutoff_depth)
            continue;
        //cout << "node " << u << " d-" << depth[u] << endl;
        for(int v : adj_lst[u]){
            bfs_operations+=1;
            //cout << "\tnode " << v ;
            if(visited[v]) {
                //cout << " already visited " << endl;
                continue;
            }
            if(matched[v] == -1){  // M --> N step with a free node 'v', found an augmenting path
                cutoff_depth = depth[u]+1;
                depth[v]  = depth[u]+1;
                visited[v] = true; // mark v as visited
                //cout << "\tdepth " << depth[v] << " IS CUTOFF" << endl;
                continue;
            }
            else if(matched[v] != u){  // M --> N step with 'v' already matched but not 'u'
                depth[v]  = depth[u]+1;
                visited[v] = true; // mark v as visited
                
                // OPTIMIZATION: only one option, go back N --> M using matched edge
                int n = v;
                int m = matched[n];
                if(visited[m]) continue; // if m already visited go to the next v
                depth[m]  = depth[n]+1; //otherwise gets its new depth
                //cout << "\tdepth " << depth[v] << " than " << m << " at depth " << depth[m] << endl;
                q.push(m);
                continue;
            }
            cout << endl;
        }
    }

    list<list<int>> disjoint_paths;
    generateDisjointPaths(depth, disjoint_paths);
    return disjoint_paths;
}


void Graph::generateDisjointPaths(vector<int>& depth, list<list<int>>& disjoint_paths){
    for(int i = 0; i < V; i++){
        for(int v = 0; v < V; v++){
            if(depth[v] == i){
                //cout << " node " << v << " found at " << i << endl;
            }
        }
    }

    vector<bool> active(V, true);
    //cout << "free vertices from N: ";
    for(int n=V/2; n < V;n++){
        if(matched[n]!= -1 || depth[n]==-1) continue;
        //cout << n << " d(" << depth[n] << ") \n";
        int leaf = n;
        list<int> path;
        bool found_path = false;
        
        dfs_calls+=1;
        extractPath(leaf, path, depth, active, found_path);
        
        // path should contain a free variable from N (leaf) and a free variable from M
        if (found_path) {
            disjoint_paths.emplace_back(path);
        }
    }
    //cout << endl;
}

void Graph::extractPath(int u, list<int>& path, vector<int>& depth, vector<bool>& active, bool& found_path) {
    dfs_operations+=1;
    //cout << "node: " << u << " active? " << active[u] << " matched? " << matched[u] << " depth? " << depth[u] << endl;
    if(!active[u]){
        return;
    }
    else if(matched[u]==-1 && u < V/2){ //found a free variable from M
        found_path= true;
        active[u] = false;
        path.emplace_back(u);
        //cout << "\t" << u << endl;
        return;
    }

    for(int v:adj_lst[u]){
        //cout << "\t\tadjacent " << v << " " << depth[v] << endl;
        
        if(depth[u]-1 == depth[v]){ 
            extractPath(v, path, depth, active, found_path);
            if(found_path){
                active[u]=false;
                path.emplace_back(u);
                //cout << "\t" << u << endl;
                return;
            }
        }
    }

    active[u]=false;
}

void Graph::readMatchings(){
    cout << "matchings are: ";
    for(int n=0; n < V/2; n++){
        if(matched[n]!=-1){
            cout << "("<< matched[matched[n]] <<","<< matched[n] <<")";
        }
    }

    // for(int n=0; n < V/2; n++){
    //     if(matched[n]!=-1){
    //         cout << "("<< matched[matched[n]] <<","<< matched[n] <<")";
    //     }
    // }
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

