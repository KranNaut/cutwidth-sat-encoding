#include "encoding.h"

#include <string>
#include <iostream>


void create_encoding(Graph &graph) {
    std::string clause = "";
    
    for(int i = 1; i <= graph.num_vertices; i++){
        for(int j = 1; j <= graph.num_vertices; j++){
            for(int k = 1; k <= graph.num_vertices; k++){
                if(j != k && j != i && k != i){
                  clause += "-" + std::to_string(i) + " -" + std::to_string(j) + " " + std::to_string(k) + " 0\n";
                }
            }
        }
    }

    if(clause == "") std::cout << "Empty clause" << std::endl;
    else   
    std::cout << clause << std::endl;
}
