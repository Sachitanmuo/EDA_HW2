#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

#include <vector>
#include <string>
#include <iostream>
#include <list>
using namespace std;
class Hyperedge;
class Node;
class Hyperedge {
public:
    std::vector<Node*> connectedNodes;
    int group_count[1] = {0}; // [0] for A, [1] for B
    bool counted = false;
    void addNode(Node* node) {
        connectedNodes.push_back(node);
    }
};

class Node {
public:
    int id;
    int group = 0; // 0 for A 1 for B
    int FS = 0;
    int TE = 0;
    int G = 0;
    bool locked = false;
    Node(int id) : id(id), group(0), FS(0), TE(0), G(0),locked(false){}
    std::vector<Hyperedge*> connectedEdges;
    void addEdge(Hyperedge* edge) {
        connectedEdges.push_back(edge);
    }
    void calculateGain(){
        FS = 0;
        TE = 0;
        for(auto* edge : connectedEdges){
            bool same = true;
            bool diff = true;
            if(1){
                for(auto* node : edge->connectedNodes){
                    edge->group_count[node->group]++;
                    //not all the nodes in the same group
                    if(group != node->group) same = false;
                    //node is not the only one that belongs to another group
                    if(node->id != id && node->group == group) diff = false;
                    }
                edge->counted = true;
            }else{
                if(edge->group_count[group]) same = false;
                if(edge->group_count[group] != 1) diff = false;
            }
            if(same) TE++;
            if(diff) FS++;
        }
        G = FS - TE;
        cout << "Node " << id << " Gain: " << G << endl;
        //cout << FS << " " << TE << endl;
    }
};


class Hypergraph {
public:
    std::vector<Node> nodes;
    std::vector<Hyperedge> edges;
    int offset = 10000;
    int A = 0;
    int B = 0;
    vector<list<Node*>> buckets_A;
    vector<list<Node*>> buckets_B;
    void addNode(int id);
    void addEdge(const std::vector<int>& nodeIds);
    void readFile(const std::string& filename);
    void FMAlgorithm();
    void updateGain(Node* node, int deltaGain);
    void switchgroup(Node* node);
    void show_Buckets();
    void output_file();
};

#endif // HYPERGRAPH_H