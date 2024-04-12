#include "Hypergraph.h"
#include <fstream>
#include <sstream>
#include <limits>
void Hypergraph::addNode(int id) {
    nodes.push_back(id);
}

void Hypergraph::addEdge(const std::vector<int>& nodeIds) {
    Hyperedge edge;
    for (int id : nodeIds) {
        Node* node = &nodes[id - 1];
        edge.addNode(node);
    }
    edges.push_back(edge);
}

void Hypergraph::readFile(const std::string& filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        //std::cerr << "File not found" << std::endl;
        return;
    }

    int nets, nds;
    input_file >> nets >> nds;
    input_file.ignore();

    for (int i = 1; i <= nds; ++i) {
        addNode(i);
    }

    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream iss(line);
        std::vector<int> nodeIds;
        int nodeId;
        while (iss >> nodeId) {
            nodeIds.push_back(nodeId);
        }
        addEdge(nodeIds);
    }
    for(int i = 0; i < edges.size(); i++){
        for(auto* node : edges[i].connectedNodes){
            node->connectedEdges.push_back(&edges[i]);
        }
    }
    input_file.close();
}

void Hypergraph::FMAlgorithm() {
    int half_size = nodes.size()/2;
    for(int i = 0; i < nodes.size();i++){
        nodes[i].group = i % 2 ? 0 : 1;
        i % 2 ? ++A : ++B;
    }
    for(auto& node : nodes){
            node.calculateGain();
            if(node.group == 0) {
                if(buckets_A.size() <= offset + node.G) buckets_A.resize(offset + node.G + 1);
                buckets_A[offset + node.G].push_back(&node);
            } else {
                if(buckets_B.size() <= offset + node.G) buckets_B.resize(offset + node.G + 1);
                buckets_B[offset + node.G].push_back(&node);
            }
            //std::cout << node.G << endl;
        }
        //show_Buckets();
       
        bool improved = true;
        while(improved){
            double ratio = ((double)A) / (A+B);
            //cout << "current ratio: " << ratio << endl;
            improved = false;
            Node* best = nullptr;
            //calculate Gain first time
            if(ratio > 0.55){
                for(int i = buckets_A.size() - 1; i >= 0; --i){
                    if(!buckets_A[i].empty()){
                        for(auto& node : buckets_A[i]){
                            if(!node->locked && node->G >= 0){
                                best = node;
                                //switchgroup(node);
                                improved = true;
                                break;
                            }
                        }
                        if(improved) break;
                    }
                }
            }else if (ratio < 0.45){
                for(int i = buckets_B.size() - 1; i >= 0; --i){
                    if(!buckets_B[i].empty()){
                        for(auto& node : buckets_B[i]){
                            if(!node->locked && node->G >= 0){
                                best = node;
                                //switchgroup(node);
                                improved = true;
                                break;
                            }
                        }
                        if(improved) break;
                    }
                }
            }else{
                int a_max, b_max;
                for(int i = buckets_A.size() - 1; i >= 0; --i){
                    if(!buckets_A[i].empty()){
                        a_max = i;
                    }
                }
                for(int i = buckets_B.size() - 1; i >= 0; --i){
                    if(!buckets_B[i].empty()){
                        b_max = i;
                    }
                }
                if(a_max > b_max){
                    for(int i = buckets_A.size() - 1; i >= 0; --i){
                        if(!buckets_A[i].empty()){
                            for(auto& node : buckets_A[i]){
                                if(!node->locked && node->G >= 0){
                                    best = node;
                                    //switchgroup(node);
                                    improved = true;
                                    break;
                                }
                            }
                            if(improved) break;
                        }
                }
                }else{
                    for(int i = buckets_B.size() - 1; i >= 0; --i){
                        if(!buckets_B[i].empty()){
                            for(auto& node : buckets_B[i]){
                                if(!node->locked && node->G >= 0){
                                    best = node;
                                    //switchgroup(node);
                                    improved = true;
                                    break;
                                }
                            }
                            if(improved) break;
                        }
                    }
                }
            }
            if(best != nullptr){
                for(auto* edge : best->connectedEdges){
                    //check F(n) and T(n) for all connected edges
                    if(edge->group_count[1 - best->group] == 0){
                        for(auto* node : edge->connectedNodes ){
                            if(node->id != best->id && !node->locked){
                                updateGain(node,1);
                            }
                        }
                    }else if(edge->group_count[1 - best->group] == 1){
                        for(auto* node : edge->connectedNodes ){
                            if(node->group != best->group && !node->locked){
                                updateGain(node,-1);
                                break;
                            }
                        }
                    }
                    edge->group_count[best->group]--;
                    edge->group_count[1 - best->group]++;
                }
                switchgroup(best);
                //show_Buckets();
            }
            else{
                cout << "best is nullptr! " << endl;
            }
        }
        output_file();
    
}

void Hypergraph::updateGain(Node* node, int deltaGain){
    vector<list<Node*>>& curr_bucketlist = (node->group) ? buckets_B : buckets_A;
    int curr_idx = offset + node->G;
    curr_bucketlist[curr_idx].remove(node);
    node->G = node->G + deltaGain;
    int new_idx = offset + node->G;
    if (new_idx >= curr_bucketlist.size()) {
        curr_bucketlist.resize(new_idx + 1);
    }
    curr_bucketlist[new_idx].push_back(node);
}

void Hypergraph::switchgroup(Node* node){
    vector<list<Node*>>& currentBucketList = (node->group == 0) ? buckets_A : buckets_B;
    vector<list<Node*>>& targetBucketList = (node->group == 0) ? buckets_B : buckets_A;
    int curr_idx = offset + node->G;
    currentBucketList[curr_idx].remove(node);
    node->group = 1 - node->group;
    node->locked = true;
    if(node->group){
        ++B;
        --A;
    }else{
        ++A;
        --B;
    }
    //cout << "switch node " << node->id << " from " << 1 - node->group << " to " << node->group << endl; 
    //comment it since no need to put it in another bucket while locked.
    //if(curr_idx >= targetBucketList.size()){
    //    targetBucketList.resize(curr_idx + 1);
    //}
    //targetBucketList[curr_idx].push_back(node); 
}

void Hypergraph::show_Buckets(){
    cout << "==============show Buckets================= " << endl;
    cout << "Buckets A: " << endl;
    for(int i = buckets_A.size() - 1; i > 0; i--){
        cout << "Index "<< i - offset << ": ";
        if(i >buckets_A.size() - 6){
            for(auto x : buckets_A[i]){
                cout << x->id << " ";
            }
        }else {cout << endl; break;}
        cout << endl;
    }
    cout << "Buckets B: " << endl;
    for(int i = buckets_B.size() - 1; i > 0; i--){
        cout << "Index "<< i - offset<< ": ";
        if(i >buckets_B.size() - 6){
            for(auto x : buckets_B[i]){
                cout << x->id << " ";
            }
        }else {cout << endl; break;}
        cout << endl;
    }
    cout << "=========================================== " << endl;
}

void Hypergraph::output_file(){
    ofstream out("output.txt");
    for(auto node : nodes){
        out << node.group << endl;
    }
}