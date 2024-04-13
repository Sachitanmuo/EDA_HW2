#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Hypergraph.h"
using namespace std;

int main(int argc, char* argv[]) {
    //initialize parameters
    fclose(stdout);
    Hypergraph hg;
    hg.readFile(argv[1]);
    hg.FMAlgorithm();
    /*
    int edgeCount = 1;
    for (const auto& edge : hg.edges) {
        cout << "Hyperedge " << edgeCount++ << " connects nodes: ";
        for (const auto& node : edge.connectedNodes) {
            cout << node->id << " ";
        }
        cout << endl;
    }
    */
      
    return 0;
}