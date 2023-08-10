#include<iostream>
#include "read_graph.h"
#include "encoding.h"


int main()
{   
    std::string fileName= "example.dimacs";
    Graph inputGraph = read_dimacs(fileName);
    std::cout << "Number of vertice: " << inputGraph.num_vertices << std::endl;
    std::cout << "Number of edges: " << inputGraph.num_edges<< "\n" << std::endl;
    
    create_encoding(inputGraph);


    for(auto i: inputGraph.edge_list){
        std::cout << std::get<0>(i) << " " << std::get<1>(i) << std::endl;
    }
    
    

    std::cout<< "Done~~~~~~~~"  <<  std::endl;


    
    return 0;
}
