#include "read_graph.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>


Graph read_dimacs(std::string filename) {

    std::ifstream file(filename);
    std::cout<<"Reading file: "<<filename<<std::endl;

    Graph graph;

    for (std::string line; std::getline(file, line); )  {
        std::cout << "Parsing line: " << line << std::endl;

        if (line[0] == 'c') {
            std::cout << "Skipping comment: " << line << std::endl;
            continue;
        } else if (line[0] == 'p') {
            std::cout << "Parsing header: " << line << std::endl;
            graph = parse_dimacs_header(line);
        } else {
            std::cout << "Parsing edge: " << line << std::endl;
            parse_dimacs_edge(line, graph);
        }
    }
    std::cout << "Done parsing file" << std::endl;
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
    iss >> word;
    graph.num_edges = std::stoi(word);
    graph.adjacency_list = new Node*[graph.num_vertices];
    for (int i = 0; i < graph.num_vertices; i++) {
        graph.adjacency_list[i] = NULL;
    }
    return graph;
}

void parse_dimacs_edge(std::string line, Graph &graph){
    std::istringstream iss(line);
    std::string word;
    iss >> word;  // Skip the first word "e"
    iss >> word;
    std::cout << word << std::endl;
    int vertex1 = std::stoi(word) - 1;
    iss >> word;
    int vertex2 = std::stoi(word) - 1;
    graph.adjacency_list[vertex1] = add_edge(graph.adjacency_list[vertex1], vertex2);
    //graph.adjacency_list[vertex2] = add_edge(graph.adjacency_list[vertex2], vertex1);
}

Node* add_edge(Node* head, int vertex) {

    if (head == NULL) {
        head = new Node{vertex, NULL};
        return head;
    }

    Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new Node{vertex, NULL};

    return head;
}

Node* remove_edge(Node* head, int vertex){
    Node* current = head;

    if (current == NULL) {
        return NULL;
    }
    else if (current->vertex == vertex) {
        Node* temp = current;
        current = current->next;
        delete temp;
        return current;
    }
    else {
        while (current->next != NULL) {
            if (current->next->vertex == vertex) {
                Node* temp = current->next->next;
                delete current->next;
                current->next = temp;
                return head;
            }
            current = current->next;
        }
        return head;
    }
}

bool check_edge(Graph &graph, int vertex1, int vertex2){
    Node* current = graph.adjacency_list[vertex1];
    while (current != NULL) {
        if (current->vertex == vertex2) {
            return true;
        }
        current = current->next;
    }
    return false;
}