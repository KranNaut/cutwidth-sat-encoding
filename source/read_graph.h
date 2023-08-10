/*
A header file to read
the input graph from a
DIMACS file.
*/
#ifndef READ_GRAPH_H_
#define READ_GRAPH_H_

#include <string>
#include <vector>

struct Node {
    int vertex;
    struct Node *next;
};

struct Graph {
    int num_vertices;
    int num_edges;
    std::vector <std::tuple<int,int>> edge_list;
};

Graph read_dimacs(std::string filename);
Graph parse_dimacs_header(std::string line);
void parse_dimacs_edge(std::string line, Graph &graph);
void add_edge(Graph &graph, int v1, int v2);
bool check_edge(Graph &graph, int v1, int v2);

#endif // !READ_GRAPH_H_