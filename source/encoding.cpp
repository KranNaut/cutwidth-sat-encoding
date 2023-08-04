#include "encoding.h"

#include <string>
#include <iostream>


void create_encoding(Graph &graph) {
    std::string transitive_clause = "";
    std::string anti_reflixive_clause = "";
    
    for(int i = 1; i <= graph.num_vertices; i++){
        for(int j = 1; j <= graph.num_vertices; j++){
            for(int k = j + 1; k <= graph.num_vertices; k++){
                if(j != k && j != i && k != i){
                  transitive_clause += "-O" + std::to_string(j) + std::to_string(k) + " -O"  + std::to_string(k) + std::to_string(i) + " O" + std::to_string(j)+ std::to_string(i) + " 0\n";
                }
            }
        }
    }
    for(int j = 1; j <= graph.num_vertices; j++){
        for(int k = j + 1; k <= graph.num_vertices; k++){
            if(j != k){
                anti_reflixive_clause += "-O" + std::to_string(j) + std::to_string(k) + " -O"  + std::to_string(k) + std::to_string(j) + " 0\n";
            }
        }
    }

      
    std::cout << anti_reflixive_clause << std::endl;
}
