#include<iostream>
#include "read_graph.h"


int main()
{
    std::string fileName= "F:\\future\\UoL\\Dissertation\\Source\\cutwidth-sat-encoding\\source\\example.dimacs";
    Graph inputGraph = read_dimacs(fileName);
    std::cout << "Number of vertice: " << inputGraph.num_vertices << std::endl;
    for(int i = 0; i < inputGraph.num_vertices; i++)
    {
        std::cout << "Vertex " << i << " has neighbours: ";
        Node* current = inputGraph.adjacency_list[i];
        while(current != NULL)
        {
            std::cout << current->vertex << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
    return 0;
}
