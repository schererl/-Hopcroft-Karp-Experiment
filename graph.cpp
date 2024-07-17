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
        for(auto aug_path: maximal_augpaths){
            symmetricDifference(aug_path);
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
        if(matched[u]){
            matched[matched[u]] = -1;
            matched[u] = -1;
        }
        if(matched[v]){
            matched[matched[v]] = -1;
            matched[v] = -1;
        }
        matched[u] = v;
        matched[v] = u;
    }

}

list<list<int>> Graph::findMultipleAugmentingPath(){
    vector<bool> visited(V, false);
    vector<int>  depth(V, 0);
    list<int>    free_leafs;
    queue<int> q;
    // get free vertices from M group
    for(int n = 0; n < V/2;n++){
        if(matched[n] == -1){
            visited[n]=true;
            q.push(n);
        }
    }

    int cutoff_depth = V;
    bfs_calls += 1;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        visited[u] = true;
        if(depth[u] >= cutoff_depth)
            continue;

        for(int v : adj_lst[u]){
            bfs_operations+=1;
            if(visited[v]) continue;
            if(matched[v] == -1){  // M --> N step with a free node 'v', found an augmenting path
                cutoff_depth = depth[u]+1;
                depth[v]  = depth[u]+1;
                free_leafs.emplace_back(v);
            }
            else if(matched[v] != u){  // M --> N step with 'v' already matched but not 'u'
                depth[v]  = depth[u]+1;
                // only one option, go back N --> M using matched edge
                int n = v;
                int m = matched[n];
                depth[m]  = depth[n]+1;
                q.push(m);
            }
        }
    }

    list<list<int>> disjoint_paths;
    generateDisjointPaths(depth, disjoint_paths);
    return disjoint_paths;
}


void Graph::generateDisjointPaths(vector<int>& depth, list<list<int>>& disjoint_paths){
    vector<bool> active(V, true);
    for(int n=V/2; n < V;n++){
        if(matched[n]!= -1 & depth[n]>0) continue;
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
}

void Graph::extractPath(int u, list<int>& path, vector<int>& depth, vector<bool>& active, bool& found_path) {
    dfs_operations+=1;
    if(!active[u]){
        return;
    }
    else if(found_path){
        active[u] = false;
        path.emplace_back(u);
        return;
    }
    else if(matched[u]==-1 && u < V/2){ //found a free variable from M
        found_path= true;
        active[u] = false;
        path.emplace_back(u);
        return;
    }

    for(int v:adj_lst[u]){
        //only adjacent nodes with lower depth: '>' operation is sufficient, otherwise wouldn't be shortest path by triangle inequality
        if(depth[u] > depth[v]){ 
            extractPath(v, path, depth, active, found_path);
            if(found_path){
                active[u]=false;
                path.emplace_back(u);
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
            cout << "("<< n <<","<< matched[n] <<")";
        }
    }
    cout << endl;
}

Graph::~Graph() {
    for (auto &list : adj_lst) {
        list.clear(); 
    }
}

