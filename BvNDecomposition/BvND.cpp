#include <queue>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm> 
#include <thread> 
#include <chrono>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#define BOOST_CHECK 0
#define VERBOSE 0
class BvND{
struct Edge {
    int m;
    int n;
    double weight;
};

#include <cmath>

bool isEqual(double a, double b, double epsilon = 0.00001) {
    return std::fabs(a - b) < epsilon;
}

private:
    // For the matchings we have V nodes, where [0, V/2) from M group and [V/2, V) from N group
    vector<list<Edge*>> adj_lst;
    vector<Edge*> allEdges;
    vector<int> match;
    int V; // total vertices
    
public:
    BvND(int V) : V(V), adj_lst(V), match(V, -1){}
    ~BvND() {
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
        if (n < V / 2) { // index range
            n += V / 2;
        }
        
        Edge* edge = new Edge{m, n, weight};
        adj_lst[m].push_back(edge);
        adj_lst[n].push_back(edge);
        allEdges.push_back(edge);
    }

    bool findMultipleAugmentingPaths(const double &bottleneck){
        vector<int>  dist(V+1, 10000000);
        queue<int>   q;
        for(int m = 0; m < V/2;m++){
            if(match[m] == -1){
                q.push(m);
                dist[m] = 0;
            }
        }
        int cutt_off = 10000001;
        while(!q.empty()){
            int m = q.front();
            q.pop();
            if(dist[m] > cutt_off){ 
                continue;
            }
            
            for(const auto &edge : adj_lst[m]){
                int n = edge->n;
                
                //if(edge->weight < bottleneck) continue; // considering bottleneck lowerbound
                if(!(edge->weight>bottleneck || isEqual(edge->weight,bottleneck))) continue;

                if(dist[n] == dist[V]){ // in case matched[n] not visited yet
                    dist[n] = dist[m]+1;
                    if(match[n]==-1){ // found free n, assign the cuttoff
                        cutt_off = dist[n];
                        continue;
                    }
                    dist[match[n]] = dist[m]+2;  //move back to M with matched n
                    q.push(match[n]);
                }
            }
        }

        vector<bool> active(V, true);
        bool found_path = false;
        for(int n=V/2; n < V;n++){
            if(match[n]!= -1 || dist[n]==-1) continue;
            int leaf = n;
            int path_len = 0;
            found_path |= extractPath(leaf, dist, active, path_len, bottleneck);
        }
        
        return found_path;
    }

    bool extractPath(int n, vector<int> &depth, vector<bool> &active, int &path_len, const double &bottleneck){
        active[n] = false;
        
        //search for reachable edges to v
        for(const auto &edge:adj_lst[n]){
            if(!active[edge->m]) continue;
            
            if(!(edge->weight>bottleneck || isEqual(edge->weight,bottleneck)) || !active[edge->m]) continue;

            //found a free variable from u, its over
            if(depth[edge->m] == 0){
                active[edge->m] = false;
                match[edge->m] = n;
                match[n] = edge->m;
                path_len+=2;
                return true;
            }
            // check for active u
            if(depth[n]-1 == depth[edge->m]){
                active[edge->m] = false;    
                if(extractPath(match[edge->m], depth, active, path_len, bottleneck)){
                    match[edge->m] = n;
                    match[n] = edge->m;
                    path_len+=2;
                    return true;
                }
            }
        }
        return false;
    }
    
    void maximumMatching(const double &bottleneck){
        int it = 0;
        fill(match.begin(), match.end(), -1);
        while(true){
            if(!findMultipleAugmentingPaths(bottleneck)){
                break;
            }
        }
    }
    

    // ######################### Birkhoff-von Neumann stuff ##############################
    void BNDecomposition(const double precision = 0.00001){
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(allEdges.begin(), allEdges.end(), [](const Edge* a, const Edge* b) { return a->weight < b->weight;});
        double sum__b = 0;
        int it = 0;
        vector<double> bottlenecks;
        while(1 - sum__b > precision){
            
            double bottleneck = findBottleneck();
        #if VERBOSE
            cout << "============NEXT DECOMPOSITION============" << endl;
            cout << "BOTTLENECK: " << bottleneck << endl;
            cout << "DENSITY: " << sum__b << endl;
        #endif
            if (bottleneck != -1) {
                discountFromMatchings(bottleneck);
                bottlenecks.push_back(bottleneck);
            } else {
                break;
            }
            it += 1;
            sum__b += bottleneck;
            std::sort(allEdges.begin(), allEdges.end(), [](const Edge* a, const Edge* b) { return a->weight < b->weight;});
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        cout << "Iterations: " << it  << endl;
        cout << "Elapsed Time: " << duration.count() << endl;
        cout << "Alpha Sum: " << sum__b << endl;
        #if VERBOSE
            cout << "Bottlenecks found: ";
            for (const auto& b : bottlenecks) {
                cout << b << " ";
            }
        #endif
        
    }

    void discountFromMatchings(const double &bottleneck){
        maximumMatching(bottleneck);
        
        for(int m=0; m < V/2; m++){
            if(adj_lst[m].empty()) continue; // if there are no edges left, skip m.
            bool edge_remove = false;
            for(auto m_edge = adj_lst[m].begin(); m_edge != adj_lst[m].end(); m_edge++){
                if ((*m_edge)->n == match[m]) { 
                    (*m_edge)->weight -= bottleneck; 
                    
                    // remove edge if weight is below precision
                    if ((*m_edge)->weight < 0.00001 || isEqual((*m_edge)->weight, 0.00001)) {
                        m_edge = adj_lst[m].erase(m_edge);
                        edge_remove = true;
                    } 
                    break;
                }
            }
            
            if(edge_remove){
                for(auto n_edge = adj_lst[match[m]].begin(); n_edge != adj_lst[match[m]].end(); n_edge++){
                    if ((*n_edge)->m == m){
                        if (edge_remove) {
                            adj_lst[match[m]].erase(n_edge);
                        }
                        break;
                    }
                }
            }
        }
    }
        
    
    double findBottleneck(){
        int low = 0;
        int high = allEdges.size();
        int it   = 0;
        double B = -1;
    #if VERBOSE
        cout << "BINARY SEARCH: high " << high << " low " << low << endl;
    #endif
        while (high > low+1) {
            int mid = (high+low)/2;
            double b = allEdges[mid]->weight;
        #if VERBOSE
            cout << "\titeration " << it << " => h:" << high << " l:" << low << " m:" << mid << " bottleneck: " << b << endl;
        #endif
            
            if (perfectMatchingExists(b)) {
            #if BOOST_CHECK    
                if(boost_check(b)){
                    cout<< "\tfound perfect matching, boost agree" << endl;
                }else{
                    cout<< "\tfound perfect matching BOOST DISAGRE!" << endl; //failing point
                    exit(0);
                }
            #endif
                low = mid;
                B = b;
            } else {
            #if BOOST_CHECK
                if(!boost_check(b)){
                    cout << "\tdid not found, boost agree." << endl;
                }else{
                    cout << "did not find BOOST DISAGRE!" << endl; // failing point
                    exit(0);
                }
            #endif
                high = mid-1;
            }
            it+=1;
        }
        return B;
    }
    
    bool perfectMatchingExists(const double &bottleneck) {
        maximumMatching(bottleneck);
        for(int m=0; m < V/2;m++){
            if (match[m] == -1 && !adj_lst[m].empty()) {
                return false;
            }
        }
        
        return true;
    }

// ##################### DEBUG STUFF ###################### //
    bool boost_check(const double bottleneck) {
        using namespace boost;

        typedef adjacency_list<vecS, vecS, undirectedS> Graph;
        Graph g;
        //cout << "boost lib testing with bottleneck " << bottleneck << endl;
        for (auto e : allEdges) {
            if (e->weight > bottleneck || isEqual(e->weight, bottleneck))
                boost::add_edge(e->m, e->n, g);
            //else cout << "\tedge not included " << e->m << " " << e->n << " " << e->weight << " checking if pass bottleneck " << bottleneck << "<=" << e->weight << "->" << (isEqual(bottleneck, e->weight)) << endl;
            
        }

        std::vector<graph_traits<Graph>::vertex_descriptor> mate(num_vertices(g));

        edmonds_maximum_cardinality_matching(g, &mate[0]);

        //std::cout << "Maximum matching size: " << matching_size(g, &mate[0]) << std::endl;
        // check if it is perfect matching
        for (std::size_t i = 0; i < mate.size(); ++i) {
            if (mate[i] == graph_traits<Graph>::null_vertex() && !adj_lst[i].empty()) {
                return false;
                //std::cout << "Vertex " << i << " is unpaired, containing valid edges" << std::endl;
                // for (auto edge : adj_lst[i]) {
                //     bool is_free = mate[edge->n] == graph_traits<Graph>::null_vertex();
                //     std::cout << " (" << edge->m << ", " << edge->n << ", " << is_free << ")  ";
                // }
                //std::cout << std::endl;
            }
        }
        return true;
    }

    void validateMatching(const double &bottleneck){
        for(int v; v < V; v++){
            if(match[v] == -1){
                cout << "not a perfect matching, something wrong" << endl;
                exit(0);
            }
            if(match[match[v]] != v){
                cout << "uncorrespondent matchings, something wrong" << endl;
                exit(0);
            }
            
            int m_vertex = -1;
            int n_vertex = -1;
            if(v < V/2){
                m_vertex = v;
                n_vertex = match[v];
            }else{
                m_vertex = match[v];
                n_vertex = v;
            }

            if(m_vertex == -1 && n_vertex != -1 || m_vertex != -1 && n_vertex == -1)
            {
                cout << "MATCHED structure is wrong." << endl;;
            }

            for(auto edge:allEdges){
                if(edge->m == m_vertex && edge->n == n_vertex){
                    if(edge->weight < bottleneck){
                        cout << "bottleneck violated " << edge->m << " " << edge->n << ": " << edge->weight << endl;
                        exit(0);
                    }
                }
            }
        }
    }

    void readEdgeValues(){
        cout << "edge values: " << endl;
        for (auto e : allEdges) {
            cout << "\tedge " << e->m << " " << e->n << " " << e->weight << endl;
        }

    }

    void readTmpMatchings(){
        cout << "matchings are: ";
        for(int m=0; m < V/2; m++){
            if(match[m]!=-1){
                //locate weight
                double w =  -1;
                for(auto e:adj_lst[m]){
                    if(e->n == match[m]){
                        w = e->weight;
                        break;
                    }
                }
                cout << "("<< match[match[m]] << ","<< match[m] <<", " << w << ") ";
                
            }
        }
        cout << endl;
    }

    int countMatchings(){
        int match_count = 0;
        for(int n=0; n < V/2; n++){
            if(match[n]!=-1){
                match_count+=1;
            }
        }
        return match_count;
    }

    int countTmpMatchings(){
        int match_count = 0;
        for(int n=0; n < V/2; n++){
            if(match[n]!=-1){
                match_count+=1;
            }
        }
        return match_count;
    }
};