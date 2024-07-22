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
class BvND{
struct Edge {
    int m;
    int n;
    double weight;
};

#include <cmath>

bool isEqual(double a, double b, double epsilon = 1e-5) {
    return std::fabs(a - b) < epsilon;
}

private:
    // For the matchings we have V nodes, where [0, V/2) from M group and [V/2, V) from N group
    vector<list<Edge*>> adj_lst;
    vector<Edge*> allEdges;
    vector<int> tmp_matched;
    vector<int> MATCHED;
    int V; // total vertices
    double H = 0;
    int L_index = 0;

public:
    BvND(int V) : V(V), adj_lst(V), tmp_matched(V, -1), MATCHED(V, -1){}
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
        if (weight > H) {
            H = weight;
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
            if(tmp_matched[m] == -1){
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
                    if(tmp_matched[n]==-1){ // found free n, assign the cuttoff
                        cutt_off = dist[n];
                        continue;
                    }
                    dist[tmp_matched[n]] = dist[m]+2;  //move back to M with matched n
                    q.push(tmp_matched[n]);
                }
            }
        }

        vector<bool> active(V, true);
        bool found_path = false;
        for(int n=V/2; n < V;n++){
            if(tmp_matched[n]!= -1 || dist[n]==-1) continue;
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
            //if(edge->weight < bottleneck || !active[edge->m]) continue;
            if(!(edge->weight>bottleneck || isEqual(edge->weight,bottleneck)) || !active[edge->m]) continue;

            //found a free variable from u, its over
            if(depth[edge->m] == 0){
                active[edge->m] = false;
                tmp_matched[edge->m] = n;
                tmp_matched[n] = edge->m;
                path_len+=2;
                return true;
            }
            // check for active u
            if(depth[n]-1 == depth[edge->m]){
                active[edge->m] = false;    
                if(extractPath(tmp_matched[edge->m], depth, active, path_len, bottleneck)){
                    tmp_matched[edge->m] = n;
                    tmp_matched[n] = edge->m;
                    path_len+=2;
                    return true;
                }
            }
        }
        return false;
    }
    
    void maximumMatching(const double &bottleneck){
        int it = 0;
        fill(tmp_matched.begin(), tmp_matched.end(), -1);
        while(true){
            if(!findMultipleAugmentingPaths(bottleneck)){
                break;
            }
        }
    }
    

    // ######################### Birkhoff-von Neumann stuff ##############################
    void BNDecomposition(const double precision = 1.0E-5){
        std::sort(allEdges.begin(), allEdges.end(), [](const Edge* a, const Edge* b) { return a->weight < b->weight;});
        H = allEdges[allEdges.size()-1]->weight;
        while (H > precision) {
            cout << "============NEXT DECOMPOSITION============" << endl;
            double bottleneck = findBottleneck();
            cout << "BOTTLENECK: " << bottleneck << endl;
            cout << "HIGHEST VALUE: " << H << endl;
            if (bottleneck != -1) {
                discountFromMatchings(bottleneck);
            }else{
                cout << "bottleneck reached 0 without converging" << endl;
                exit(0);
            }
            std::sort(allEdges.begin(), allEdges.end(), [](const Edge* a, const Edge* b) { return a->weight < b->weight;});
            H = allEdges[allEdges.size()-1]->weight;
            
            //std::this_thread::sleep_for(std::chrono::seconds(1));  // Pause for 1 second

        }
    }

    void discountFromMatchings(const double &bottleneck){
        //reset perfect matchings
        // for(int m = 0; m < V/2;m++){
        //     if(MATCHED[m] != -1){ 
        //         MATCHED[MATCHED[m]] = -1;
        //         MATCHED[m] = -1;
        //     }
        // }
        maximumMatching(bottleneck);
        // update perfect matchings
        // for(int m = 0; m < V/2;m++){
        //     if(tmp_matched[m] != -1){
        //         MATCHED[m] = tmp_matched[m];
        //         MATCHED[tmp_matched[m]] = m;
        //     }
        // }
        
        //validateMatching(bottleneck); //testing failing point
        
        
        for(int m=0; m < V/2; m++){
            if(adj_lst[m].empty()) continue; // if there are no edges left, skip m.
            
            if(tmp_matched[m] == -1 || (tmp_matched[tmp_matched[m]] != m)){ //temporary fail exit point
                cout << "something wrong here." << endl;
                exit(0);
            }

            bool edge_remove = false;
            for(auto m_edge = adj_lst[m].begin(); m_edge != adj_lst[m].end(); m_edge++){
                if ((*m_edge)->n == tmp_matched[m]) { 
                    (*m_edge)->weight -= bottleneck;    //discount edge, NOTE: ALSO I CAN MOVE THE NEW WEIGHT TO THE LEFT
                    if ((*m_edge)->weight <= 1.0E-5) { // in case its is lower than our precision, remove it from adj list
                        adj_lst[m].erase(m_edge);
                        edge_remove = true;
                        break;
                    }
                }
            }
            
            if(edge_remove){ //also remove from n
                for(auto n_edge = adj_lst[tmp_matched[m]].begin(); n_edge != adj_lst[tmp_matched[m]].end(); n_edge++){
                    if ((*n_edge)->m == m){
                        adj_lst[tmp_matched[m]].erase(n_edge);
                        break;
                    }
                }
            }
        }
        
    }

    double findBottleneck(){
        for(int e_i = L_index; e_i < allEdges.size(); e_i++){
            if(allEdges[e_i]->weight < 1.0E-5 || isEqual(allEdges[e_i]->weight, 1.0E-5)){
                L_index+=1;    
            }else{
                break;
            }
            
        }
        int low = L_index;
        int high = allEdges.size();
        int it   = 0;
        double B = -1;
        cout << "BINARY SEARCH: high " << high << " low " << low << endl;
        while (high > low) {
            int mid = (high+low)/2;
            double b = allEdges[mid]->weight;
            cout << "\titeration " << it << " => h:" << high << " l:" << low << " m:" << mid << " bottleneck: " << b << endl;
            //cout << "BEFORE" << endl;
            //readEdgeValues();
            bool answer = perfectMatchingExists(b);
            //cout << "AFTER" << endl;
            //readEdgeValues();
            if (answer) {
                if(boost_check(b)){
                    cout<< "\tfound perfect matching, boost agree" << endl;
                }else{
                    cout<< "\tfound perfect matching BOOST DISAGRE!" << endl; //failing point
                    exit(0);
                }
                low = mid+1;
                B = b;
            } else {
                if(!boost_check(b)){
                    cout << "\tdid not found, boost agree." << endl;
                }else{
                    cout << "did not find BOOST DISAGRE!" << endl; // failing point
                    exit(0);
                }
                high = mid-1;
            }
            it+=1;
            //readTmpMatchings();
        }
        return B;
    }
    
    bool perfectMatchingExists(const double &bottleneck) {
        maximumMatching(bottleneck);
        for(int m=0; m < V/2;m++){
            if (tmp_matched[m] == -1 && !adj_lst[m].empty()) {
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
            if(MATCHED[v] == -1){
                cout << "not a perfect matching, something wrong" << endl;
                exit(0);
            }
            if(MATCHED[MATCHED[v]] != v){
                cout << "uncorrespondent matchings, something wrong" << endl;
                exit(0);
            }
            
            int m_vertex = -1;
            int n_vertex = -1;
            if(v < V/2){
                m_vertex = v;
                n_vertex = MATCHED[v];
            }else{
                m_vertex = MATCHED[v];
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

    void readTmpMatchings(){
        cout << "matchings are: ";
        for(int m=0; m < V/2; m++){
            if(tmp_matched[m]!=-1){
                //locate weight
                double w =  -1;
                for(auto e:adj_lst[m]){
                    if(e->n == tmp_matched[m]){
                        w = e->weight;
                        break;
                    }
                }
                cout << "("<< tmp_matched[tmp_matched[m]] << ","<< tmp_matched[m] <<", " << w << ") ";
                
            }
        }
        cout << endl;
    }

    int countMatchings(){
        int match_count = 0;
        for(int n=0; n < V/2; n++){
            if(MATCHED[n]!=-1){
                match_count+=1;
            }
        }
        return match_count;
    }

    int countTmpMatchings(){
        int match_count = 0;
        for(int n=0; n < V/2; n++){
            if(tmp_matched[n]!=-1){
                match_count+=1;
            }
        }
        return match_count;
    }
};