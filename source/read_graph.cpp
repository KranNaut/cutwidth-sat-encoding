#include "read_graph.h"
#include <cstdlib>
#include <fstream>
#include <sstream>


Graph parse_dimacs(std::ifstream& file);
Graph parse_dimacs_header(std::string line);
Graph parse_dimacs_edge(std::string line, Graph graph);
void add_edge(Node* head, int vertex);

Graph read_dimacs(std::string &filename) {
    std::ifstream file("example.dimacs");
    Graph graph = parse_dimacs(file);
    return graph;
}


Graph parse_dimacs(std::ifstream& file) {
    Graph graph;
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == 'c') {
            continue;
        } else if (line[0] == 'p') {
            graph = parse_dimacs_header(line);
        } else {
            graph = parse_dimacs_edge(line, graph);
        }
    }
    return graph;
}

Graph parse_dimacs_header(std::string line) {
    Graph graph;
    std::istringstream iss(line);
    std::string word;
    iss >> word;  // Skip the first word "p"
    iss >> word;  // Skip the second word "edge"
    iss >> word;
    graph.num_vertices = std::stoi(word);
    graph.adjacency_list = new Node*[graph.num_vertices];
    for (int i = 0; i < graph.num_vertices; i++) {
        graph.adjacency_list[i] = NULL;
    }
    return graph;
}

Graph parse_dimacs_edge(std::string line, Graph graph){
    std::istringstream iss(line);
    std::string word;
    iss >> word;  // Skip the first word "e"
    int vertex1 = std::stoi(word);
    iss >> word;
    int vertex2 = std::stoi(word);
    add_edge(graph.adjacency_list[vertex1], vertex2);
    add_edge(graph.adjacency_list[vertex2], vertex1);
    return graph;
}

void add_edge(Node* head, int vertex) {
    Node* new_node = new Node;
    
    Node **temp = &head;
    Node *current = head;
    while (current->next != NULL) {
        temp = &current->next;
        current = current->next;
    }
    new_node->vertex = vertex;
    new_node->next = NULL;
    current->next = new_node;
}