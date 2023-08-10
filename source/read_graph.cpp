#include "read_graph.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <tuple>


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
    add_edge(graph, vertex1, vertex2);
    add_edge(graph, vertex2, vertex1);
}

void add_edge(Graph &graph, int v1, int v2){
    graph.edge_list.push_back(std::make_tuple(v1, v2));
}



bool check_edge(Graph &graph, int v1, int v2){
    for(auto edge: graph.edge_list){
        if(std::get<0>(edge) == v1 && std::get<1>(edge) == v2)
            return true;
    }
    return false;
}