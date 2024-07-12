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


Graph::Graph(int V):V(V), M(0), adj_lst(V), matched(V,-1) {

}

void Graph::addEdge(int u, int v){
    M+=1;
    adj_lst[u].push_back(v);
    adj_lst[v].push_back(u);
}

list<int> Graph::findAugmentingPath(){
    cout << "search for augmenting path." << endl;
    vector<bool> visited(V, false);
    vector<int> parent(V, -1);
    queue<int> q;
    // get free vertices from M group
    for(int n = 0; n < V/2;n++){
        if(matched[n] == -1){
            visited[n]=true;
            q.push(n);
        }
    }

    int iterations = 0;
    list<int> aug_path;

    while(!q.empty()){
        iterations+=1;
        int u = q.front();
        q.pop();
        visited[u] = true;
        cout << "\nnode " << u << endl;
        for(int v : adj_lst[u]){
            if(visited[v]) continue;
            cout << "--> node " << v << endl;
            if(matched[v] == -1){  // M --> N step with a free node 'v', found an augmenting path
                cout << "\tfree" << endl;
                int n = v;
                parent[v] = u;
                while(n != -1){
                    aug_path.emplace_back(n);
                    n = parent[n];
                }
                cout << "augmenting path of size " << aug_path.size() << " found after " << iterations << " iterations." << endl;
                cout << "path: ";
                for (int element : aug_path) {
                    cout << element << " ";
                }
                cout << endl;
                return aug_path;
            }
            else if(matched[v] != u){  // M --> N step with 'v' already matched but not 'u'
                cout << "\talready matched with node " << matched[v] << endl;
                parent[v] = u;
                // only one option, go back N --> M using matched edge
                int n = v;
                int m = matched[n];
                parent[m] = n;
                q.push(m);
            }
        }
    }
    cout << "augmenting path not found." << endl;    
    return aug_path;    
}

void Graph::maximumMatching(){
    list<int> aug_path;
    while(true){
        list<int> aug_path = findAugmentingPath();
        if(aug_path.size() == 0) break;
        symmetricDifference(aug_path);
        
        readMatchings();
    }
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
    while(!q.empty()){
        int u = q.front();
        q.pop();
        visited[u] = true;
        if(depth[u] >= cutoff_depth)
            continue;

        for(int v : adj_lst[u]){
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
    generateDisjointPaths(depth, free_leafs, disjoint_paths);
    cout << "disjoint paths are: " << endl;
    for (const auto &path : disjoint_paths) {
        for (int node : path) {
            cout << node << " ";
        }
        cout << endl;
    }
    return disjoint_paths;
}


void Graph::generateDisjointPaths(vector<int>& depth, list<int>& free_leafs, list<list<int>>& disjoint_paths){
    vector<bool> active(V, true);
    //for(int leaf:free_leafs){
    for(int n=V/2; n < V;n++){
        if(matched[n]!= -1 & depth[n]>0) continue;
        int leaf = n;
        list<int> path;
        bool found_path = false;
        cout<< "Search for disjoint path starting with " << leaf << endl;
        extractPath(leaf, path, depth, active, found_path);
        // path should contain a free variable from N (leaf) and a free variable from M
        if (found_path) {
            disjoint_paths.emplace_back(path);
        }
    }
}

void Graph::extractPath(int u, list<int>& path, vector<int>& depth, vector<bool>& active, bool& found_path) {
    if(!active[u]){
        return;
    }
    else if(found_path){
        active[u] = false;
        path.emplace_back(u);
        cout << "\t" << u << endl;
        return;
    }
    else if(matched[u]==-1 && u < V/2){ //found a free variable from M
        found_path= true;
        active[u] = false;
        path.emplace_back(u);
        cout << "found path!" << endl;
        cout << "\t" << u << endl;
        return;
    }

    for(int v:adj_lst[u]){
        //only adjacent nodes with lower depth: '>' operation is sufficient, otherwise wouldn't be shortest path by triangle inequality
        if(depth[u] > depth[v]){ 
            extractPath(v, path, depth, active, found_path);
            if(found_path){
                active[u]=false;
                path.emplace_back(u);
                cout << "\t" << u << endl;
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

