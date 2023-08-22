#include<iostream>
#include <chrono>
#include "read_graph.h"
#include "encoding.h"


int main(int argc,char *argv[])
{   

    int timeout = 100;
    std::string fileName= "graph.dimacs";
    if (argc > 1) {
        fileName = argv[1];
    }
    if (argc > 2) {
        timeout = int(argv[2]);
    }
    Graph inputGraph = read_dimacs(fileName);
    std::cout << "Number of vertice: " << inputGraph.num_vertices << std::endl;
    std::cout << "Number of edges: " << inputGraph.num_edges<< "\n" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    std::string result = find_cutwidth(inputGraph,timeout);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    
    std::cout << result;
    std::cout << "Finished in " << duration.count() << " milliseconds" << std::endl;



    
    return 0;
}
