#include<iostream>
#include "read_graph.h"
#include "encoding.h"


int main()
{
    std::string fileName= "F:\\future\\UoL\\Dissertation\\Source\\cutwidth-sat-encoding\\source\\example.dimacs";
    Graph inputGraph = read_dimacs(fileName);
    std::cout << "Number of vertice: " << inputGraph.num_vertices << std::endl;
    std::cout << "Number of edges: " << inputGraph.num_edges << std::endl;
    
    create_encoding(inputGraph);
    
    std::cout<< "Done~~~~~~~~" << std::endl;
    
    return 0;
}
