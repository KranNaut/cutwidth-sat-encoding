/*
A header file to read
the input graph from a
DIMACS file.
*/
#ifndef READ_GRAPH_H_
#define READ_GRAPH_H_

#include <string>

struct Node {
    int vertex;
    struct Node *next;
};

struct Graph {
    int num_vertices;
    int num_edges;
    Node **adjacency_list;
};

Graph read_dimacs(std::string filename);
Graph parse_dimacs_header(std::string line);
void parse_dimacs_edge(std::string line, Graph &graph);
Node* add_edge(Node* head, int vertex);


#endif // !READ_GRAPH_H_