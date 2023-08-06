#include "encoding.h"

#include <string>
#include <iostream>


void write_encoding(std::string clause) {
    // std::ofstream encoding_file;
    // encoding_file.open("encoding.txt", std::ios_base::app);
    // encoding_file << clause;
    // encoding_file.close();

    std::cout << clause;
}



void create_encoding(Graph &graph, int max_cutwidth) {
    
    int num_clauses = 0;
    
    std::string transitive_clause = "";
    for(int i = 1; i <= graph.num_vertices; i++){
        for(int j = 1; j <= graph.num_vertices; j++){
            for(int k = j + 1; k <= graph.num_vertices; k++){
                if(j != k && j != i && k != i){
                  transitive_clause += "-O" + std::to_string(j) + std::to_string(k) + " -O"  + std::to_string(k) + std::to_string(i) + " O" + std::to_string(j)+ std::to_string(i) + " 0\n";
                  num_clauses++;
                }
            }
        }
    }

    std::string anti_reflixive_clause = "";
    for(int j = 1; j <= graph.num_vertices; j++){
        for(int k = j + 1; k <= graph.num_vertices; k++){
            if(j != k){
                anti_reflixive_clause += "-O" + std::to_string(j) + std::to_string(k) + " -O"  + std::to_string(k) + std::to_string(j) + " 0\n";
                num_clauses++;
            }
        }
    }
    
    std::string counter_clause = "";
    int counter = 0;
    // Iterating over the edges of the graph
    for(int k = 0; k < graph.num_vertices; k++){
        //Counter variable for each vertex
        for(int i = 0; i < graph.num_vertices-1; i++){
            for(int j = i+1; j < graph.num_vertices; j++){
                if(check_edge(graph, i, j)){
                    //std::cout << "-C" << k+1 << counter<< " -O" << i+1 << k+1 << " -O" << k+1 << j+1 << " C" << k+1 << counter + 1 << " 0\n"; 
                    counter_clause += "-C" + std::to_string(k+1) + std::to_string(counter) + 
                                    " -O" + std::to_string(i+1) + std::to_string(k+1) +
                                    " -O" + std::to_string(k+1) + std::to_string(j+1) + 
                                    " C" + std::to_string(k+1) + std::to_string(counter + 1) +
                                    " 0\n"; 
                    counter++;
                    num_clauses++;
                }
            }
        }
        counter = 0;
    }

    
    write_encoding("c Generated encoding file\n");
    write_encoding("p cnf " + 
    (std::stoi( "1" + std::to_string(graph.num_vertices)+ std::to_string(graph.num_vertices)) > 
    std::stoi( "2" + std::to_string(graph.num_vertices)+ std::to_string(graph.num_edges)) ? 
    "1" + std::to_string(graph.num_vertices)+ std::to_string(graph.num_vertices) :
    "2" + std::to_string(graph.num_vertices)+ std::to_string(graph.num_edges)) + " " + std::to_string(num_clauses)
    + "\n");
    write_encoding(transitive_clause);
    write_encoding(anti_reflixive_clause);
    write_encoding(counter_clause);


}
