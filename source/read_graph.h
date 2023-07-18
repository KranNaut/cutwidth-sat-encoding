/*
A header file to read
the input graph from a
DIMACS file.
*/

#include <string>

struct Node {
    int vertex;
    struct Node *next;
};

struct Graph {
    int num_vertices;
    Node **adjacency_list;
};

Graph read_dimacs(std::string filename);

