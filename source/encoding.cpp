#include "encoding.h"

#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>

#include <string>
#include <iostream>
#include <fstream>



void write_encoding(std::string clause) {
    std::ofstream encoding_file;
    encoding_file.open("encoding.sat", std::ios_base::trunc);
    encoding_file << clause;
    encoding_file.close();
}

void write_clause_map(std::unordered_map <std::string, int> clause_map) {
    
    std::ofstream clause_map_file;
    clause_map_file.open("clause_map.csv", std::ios_base::trunc);
    for (auto it = clause_map.begin(); it != clause_map.end(); ++it) {
        clause_map_file << "\""<< it->first << "\""<< "," 
                        << "\""<< it->second << "\""<< "\n";
    }
    clause_map_file.close();
}


std::unordered_map <std::string, int> clause_map;


void create_encoding(Graph &graph, int max_cutwidth) {
    
    int num_var = 0;
    int num_clauses = 0;
    std::string trans_clause1 = "";
    std::string trans_clause2 = "";
    std::string trans_clause3 = "";
    std::string transitive_clause = "";

    // CMSat::SATSolver solver;
    // std::vector<CMSat::Lit> clause;
    // solver.set_num_threads(4);

    for(int i = 1; i <= graph.num_vertices; i++){
        for(int j = 1; j <= graph.num_vertices; j++){
            for(int k = 1; k <= graph.num_vertices; k++){
                if(j != k && j != i && k != i){
                    // transitive_clause += "-O" + std::to_string(j) + std::to_string(k) + 
                    // " -O"  + std::to_string(k) + std::to_string(i) + 
                    // " O" + std::to_string(j)+ std::to_string(i) + " 0\n";


                    trans_clause1 = "O(" + std::to_string(j) + "," + std::to_string(k) + ")";
                    trans_clause2 = "O(" + std::to_string(k) + "," + std::to_string(i) + ")";
                    trans_clause3 = "O(" + std::to_string(j) + "," + std::to_string(i) + ")";
                    if(clause_map.find(trans_clause1) == clause_map.end()){
                        clause_map[trans_clause1] = num_var+1;
                        num_var++;
                    }
                    if(clause_map.find(trans_clause2) == clause_map.end()){
                        clause_map[trans_clause2] = num_var+1;
                        num_var++;
                    }
                    if(clause_map.find(trans_clause3) == clause_map.end()){
                        clause_map[trans_clause3] = num_var+1;
                        num_var++;
                    }
                    transitive_clause += "-" + std::to_string(clause_map[trans_clause1]) + " -" + std::to_string(clause_map[trans_clause2]) + " " + std::to_string(clause_map[trans_clause3]) + " 0\n";
                    num_clauses++;
                }
            }
        }
    }

    std::string anti_reflixive_clause = "";
    std::string anti_ref_clause1 = "";
    std::string anti_ref_clause2 = "";


    for(int j = 1; j <= graph.num_vertices; j++){
        for(int k = j + 1; k <= graph.num_vertices; k++){
            if(j != k){
                // anti_reflixive_clause += "-O" + std::to_string(j) + std::to_string(k) + " -O"  + std::to_string(k) + std::to_string(j) + " 0\n";


                anti_ref_clause1 = "O(" + std::to_string(j) + "," + std::to_string(k) + ")";
                anti_ref_clause2 = "O(" + std::to_string(k) + "," + std::to_string(j) + ")";
                if(clause_map.find(anti_ref_clause1) == clause_map.end()){
                    clause_map[anti_ref_clause1] = num_var+1;
                    num_var++;
                }
                if(clause_map.find(anti_ref_clause2) == clause_map.end()){
                    clause_map[anti_ref_clause2] = num_var+1;
                    num_var++;
                }
                anti_reflixive_clause +=  std::to_string(clause_map[anti_ref_clause1]) + " " + std::to_string(clause_map[anti_ref_clause2]) + " 0\n";
                anti_reflixive_clause += "-" + std::to_string(clause_map[anti_ref_clause1]) + " -" + std::to_string(clause_map[anti_ref_clause2]) + " 0\n";
                num_clauses+= 2;
            }
        }
    }
    
    std::string counter_clause = "";
    int counter = 0;
    std::string count_clause1 = "";
    std::string count_clause2 = "";
    std::string count_clause3 = "";
    std::string count_clause4 = "";
    std::string count_clause5 = "";
    std::string count_clause6 = "";

    // Iterating over the edges of the graph
    for(int vertex = 0; vertex < graph.num_vertices; vertex++){
        int e = 1;
        for(int i=0; i < graph.num_vertices; i++){
            for(auto j = graph.adjacency_list[i]; j; j= j->next){
                
                if((vertex != i && vertex != j->vertex) ){
                    
                    for(int n = 0; n < e; n++){
                    //std::cout << "Iterating Vertex " << vertex + 1 << " Edge number " << e << " iteration " << n+1 << " from " << i+1 << " to " << j->vertex + 1 << std::endl;
                    //counter_clause += "Iterating Vertex " + std::to_string(vertex + 1) + " Edge number" + std::to_string(e) + " from " + std::to_string(i+1) + " to " + std::to_string(j->vertex + 1) + "\n"; 

                    
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        count_clause2 = "O(" + std::to_string(i+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause3 = "O(" + std::to_string(vertex+1) + "," + std::to_string(i+1) + ")";
                        count_clause4 = "O(" + std::to_string(vertex+1) + "," + std::to_string((j->vertex)+1) + ")";
                        count_clause5 = "O(" + std::to_string((j->vertex)+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause6 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n+1) + ")";
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var+1;
                            num_var++;
                        }
                        if(clause_map.find(count_clause2) == clause_map.end()){
                            clause_map[count_clause2] = num_var+1;
                            std::cout << "count_clause2 " << count_clause2 << " " << num_var+1 << std::endl;
                            num_var++;
                        }
                        if(clause_map.find(count_clause3) == clause_map.end()){
                            clause_map[count_clause3] = num_var+1;
                            std::cout << "count_clause3 " << count_clause3 << " " << num_var+1 << std::endl;
                            num_var++;
                        }
                        if(clause_map.find(count_clause4) == clause_map.end()){
                            clause_map[count_clause4] = num_var+1;
                            std::cout << "count_clause4 " << count_clause4 << " " << num_var+1 << std::endl;
                            num_var++;
                        }
                        if(clause_map.find(count_clause5) == clause_map.end()){
                            clause_map[count_clause5] = num_var+1;
                            std::cout << "count_clause5 " << count_clause5 << " " << num_var+1 << std::endl;
                            num_var++;
                        }
                        if(clause_map.find(count_clause6) == clause_map.end()){
                            clause_map[count_clause6] = num_var+1;
                            num_var++;
                        }

                        counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) + 
                                        " -O" + std::to_string(i+1) + std::to_string(vertex+1) + 
                                        " -O" + std::to_string(vertex+1) + std::to_string((j->vertex)+1) + 
                                        " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n+1) + " 0\n";
                                        
                        counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) +
                                        " -O" + std::to_string(vertex+1) + std::to_string(i+1) + 
                                        " -O" + std::to_string((j->vertex)+1) + std::to_string(vertex+1) +
                                        " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n+1) + " 0\n";
                        num_clauses+= 2;
                    }
                }
                else if (vertex != j->vertex) {

                    for(int n = 0; n < e; n++){
                        //std::cout << "Iterating Vertex " << vertex + 1 << " Edge number " << e << " iteration " << n+1 << " from " << i+1 << " to " << j->vertex + 1 << std::endl;
                        
                        
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        count_clause2 = "O(" + std::to_string(i+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause3 = "O(" + std::to_string((j->vertex)+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause4 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n+1) + ")";
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var+1;
                            num_var++;
                        }
                        if(clause_map.find(count_clause2) == clause_map.end()){
                            clause_map[count_clause2] = num_var+1;
                            std::cout << "count_clause2- " << count_clause2 << " " << num_var+1 << std::endl;
                            num_var++;
                        }
                        if(clause_map.find(count_clause3) == clause_map.end()){
                            clause_map[count_clause3] = num_var+1;
                            std::cout << "count_clause3- " << count_clause3 << " " << num_var+1 << std::endl;
                            num_var++;
                        }
                        if(clause_map.find(count_clause4) == clause_map.end()){
                            clause_map[count_clause4] = num_var+1;
                            num_var++;
                        }

                        // Only check if the edge starts on the right side of the vertex and ends on it
                        counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) + 
                                        " -O" + std::to_string(i+1) + std::to_string(vertex+1) + 
                                        " -O" + std::to_string(vertex+1) + std::to_string((j->vertex)+1) + 
                                        " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n+1) + " 0\n";

                        num_clauses++;
                    }

                }    
                e++;       
            }
        }
    }





    // for(int k = 0; k < graph.num_vertices; k++){
    //     //Counter variable for each vertex
    //     for(int i = 0; i < graph.num_vertices-1; i++){
    //         int jstart = ((k == graph.num_vertices) && (i == graph.num_vertices))  ? i : i+1;
    //         for(int j = jstart; j < graph.num_vertices; j++){
    //             if(k != i){
    //                 if(check_edge(graph, i, j)){
    //                     counter_clause += "-C" + std::to_string(k+1) + std::to_string(counter) + 
    //                                     " -O" + std::to_string(i+1) + std::to_string(k+1) +
    //                                     " -O" + std::to_string(k+1) + std::to_string(j+1) + 
    //                                     " C" + std::to_string(k+1) + std::to_string(counter + 1) +
    //                                     " 0\n"; 



                                        
    //                     // count_clause1 = "C(" + std::to_string(k+1) + "," + std::to_string(counter) + ")";
    //                     // count_clause2 = "O(" + std::to_string(i+1) + "," + std::to_string(k+1) + ")";
    //                     // count_clause3 = "O(" + std::to_string(k+1) + "," + std::to_string(j+1) + ")";
    //                     // count_clause4 = "C(" + std::to_string(k+1) + "," + std::to_string(counter + 1) + ")";

    //                     // if(clause_map.find(count_clause1) == clause_map.end()){
    //                     //     clause_map[count_clause1] = num_var+1;
    //                     //     num_var++;
    //                     // }
    //                     // if(clause_map.find(count_clause2) == clause_map.end()){
    //                     //     clause_map[count_clause2] = num_var+1;
    //                     //     num_var++;
    //                     // }
    //                     // if(clause_map.find(count_clause3) == clause_map.end()){
    //                     //     clause_map[count_clause3] = num_var+1;
    //                     //     num_var++;
    //                     // }
    //                     // if(clause_map.find(count_clause4) == clause_map.end()){
    //                     //     clause_map[count_clause4] = num_var+1;
    //                     //     num_var++;
    //                     // }
    //                     // counter_clause += "-" + std::to_string(clause_map[count_clause1]) + 
    //                     // " -" + std::to_string(clause_map[count_clause2]) + 
    //                     // " -" + std::to_string(clause_map[count_clause3]) + 
    //                     // " " + std::to_string(clause_map[count_clause4]) + " 0\n";
    //                      if (k!=j){
    //                         // count_clause2 = "O(" + std::to_string(j+1) + "," + std::to_string(k+1) + ")";
    //                         // if(clause_map.find(count_clause2) == clause_map.end()){
    //                         //     clause_map[count_clause2] = num_var+1;
    //                         //     num_var++;
    //                         // }
    //                         // count_clause3 = "O(" + std::to_string(k+1) + "," + std::to_string(i+1) + ")";
    //                         // if(clause_map.find(count_clause3) == clause_map.end()){
    //                         //     clause_map[count_clause3] = num_var+1;
    //                         //     num_var++;
    //                         // }
    //                         // counter_clause += "-" + std::to_string(clause_map[count_clause1]) +
    //                         // " -" + std::to_string(clause_map[count_clause2]) +
    //                         // " -" + std::to_string(clause_map[count_clause3]) +
    //                         // " " + std::to_string(clause_map[count_clause4]) + " 0\n";


    //                         counter_clause += "-C" + std::to_string(k+1) + std::to_string(counter) + 
    //                                     " -O" + std::to_string(j+1) + std::to_string(k+1) +
    //                                     " -O" + std::to_string(k+1) + std::to_string(i+1) + 
    //                                     " C" + std::to_string(k+1) + std::to_string(counter + 1) +
    //                                     " 0\n";

    //                     }
    //                     num_clauses++;
    //                     counter++;
    //                 }
    //             }
    //         }
    //     }
    //     counter = 0;
    // }

    std::string comment  = "c Generated encoding file\n";
    std::string header = "p cnf " + std::to_string(num_var)
    + " " + std::to_string(num_clauses)
    + "\n";
    std::string encoding = comment + header + transitive_clause + anti_reflixive_clause + counter_clause;
    write_encoding(encoding);
    write_clause_map(clause_map);

}
