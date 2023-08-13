#include "encoding.h"



#include <string>
#include <iostream>
#include <fstream>

#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>


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
CMSat::SATSolver solver;
std::vector<std::vector<CMSat::Lit>> clauses;
std::vector<CMSat::Lit> temp_clause;

void create_encoding(Graph &graph, int max_cutwidth) {
    
    int num_var = 0;
    int num_clauses = 0;
    std::string trans_clause1 = "";
    std::string trans_clause2 = "";
    std::string trans_clause3 = "";
    std::string transitive_clause = "";


    std::string self_clause = "";
    std::string self_clause1 = "";
    for(int i = 1; i <= graph.num_vertices; i++){
        // O(i,i) = 1
        self_clause1 = "O(" + std::to_string(i) + "," + std::to_string(i) + ")";
        self_clause += self_clause1 + " 0\n";
        if(clause_map.find(self_clause1) == clause_map.end()){
            clause_map[self_clause1] = num_var;
            num_var++;
        }
        num_clauses++;
        // writing temp clause
        temp_clause.push_back(CMSat::Lit(clause_map[self_clause1], false));
        // adding temp clause to clauses
        clauses.push_back(temp_clause);
        temp_clause.clear();
        
    }



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
                        clause_map[trans_clause1] = num_var;
                        num_var++;
                    }
                    if(clause_map.find(trans_clause2) == clause_map.end()){
                        clause_map[trans_clause2] = num_var;
                        num_var++;
                    }
                    if(clause_map.find(trans_clause3) == clause_map.end()){
                        clause_map[trans_clause3] = num_var;
                        num_var++;
                    }

                    // writing temp clause
                    temp_clause.push_back(CMSat::Lit(clause_map[trans_clause1], true));
                    temp_clause.push_back(CMSat::Lit(clause_map[trans_clause2], true));
                    temp_clause.push_back(CMSat::Lit(clause_map[trans_clause2], false));
                    // adding temp clause to clauses
                    clauses.push_back(temp_clause);
                    temp_clause.clear();


                    transitive_clause += "-" + trans_clause1 + " -" + trans_clause2 + " " + trans_clause3 + " 0\n";
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
                    clause_map[anti_ref_clause1] = num_var;
                    num_var++;
                }
                if(clause_map.find(anti_ref_clause2) == clause_map.end()){
                    clause_map[anti_ref_clause2] = num_var;
                    num_var++;
                }
                anti_reflixive_clause +=  anti_ref_clause1 + " " + anti_ref_clause2 + " 0\n";
                
                // writing temp clause
                temp_clause.push_back(CMSat::Lit(clause_map[anti_ref_clause1], false));
                temp_clause.push_back(CMSat::Lit(clause_map[anti_ref_clause2], false));
                
                // adding temp clause to clauses
                clauses.push_back(temp_clause);
                temp_clause.clear();

                anti_reflixive_clause += "-" + anti_ref_clause1 + " -" + anti_ref_clause2 + " 0\n";

                // writing temp clause
                temp_clause.push_back(CMSat::Lit(clause_map[anti_ref_clause1], true));
                temp_clause.push_back(CMSat::Lit(clause_map[anti_ref_clause2], true));

                // adding temp clause to clauses
                clauses.push_back(temp_clause);
                temp_clause.clear();


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

    std::vector<CMSat::Lit> assumptions;
    std::string assume;

    // Iterating over the edges of the graph
    for(int vertex = 0; vertex < graph.num_vertices; vertex++){
        int e = 1;
        for(int i=0; i < graph.num_vertices; i++){
            for(auto j = graph.adjacency_list[i]; j; j= j->next){
                
                for(int n = 0; (n <= e); n++){

                    if(n==0){
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        counter_clause += count_clause1 + " 0\n";
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var;
                            num_var++;
                        }
                        // writing temp clause
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause1], false));
                        // adding temp clause to clauses
                        clauses.push_back(temp_clause);
                        temp_clause.clear();
                    }

                    if(n == max_cutwidth){
                        assume = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        if(clause_map.find(assume) == clause_map.end()){
                            clause_map[assume] = num_var;
                            num_var++;
                        }
                        assumptions.push_back(CMSat::Lit(clause_map[assume], true));
                    }
                    
                    if((vertex != i && vertex != j->vertex) ){
                    
                    //std::cout << "Iterating Vertex " << vertex + 1 << " Edge number " << e << " iteration " << n+1 << " from " << i+1 << " to " << j->vertex + 1 << std::endl;
                    //counter_clause += "Iterating Vertex " + std::to_string(vertex + 1) + " Edge number" + std::to_string(e) + " from " + std::to_string(i+1) + " to " + std::to_string(j->vertex + 1) + "\n"; 

                    
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        count_clause2 = "O(" + std::to_string(i+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause3 = "O(" + std::to_string(vertex+1) + "," + std::to_string(i+1) + ")";
                        count_clause4 = "O(" + std::to_string(vertex+1) + "," + std::to_string((j->vertex)+1) + ")";
                        count_clause5 = "O(" + std::to_string((j->vertex)+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause6 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n+1) + ")";
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var;
                            num_var++;
                        }
                        if(clause_map.find(count_clause6) == clause_map.end()){
                            clause_map[count_clause6] = num_var;
                            num_var++;
                        }


                        counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) + 
                                        " -O" + std::to_string(i+1) + std::to_string(vertex+1) + 
                                        " -O" + std::to_string(vertex+1) + std::to_string((j->vertex)+1) + 
                                        " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n+1) + " 0\n";

                        // writing temp clause
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause1], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause2], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause4], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause6], false));

                        // pushing temp clause to clauses
                        clauses.push_back(temp_clause);
                        temp_clause.clear();
                                        
                        counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) +
                                        " -O" + std::to_string(vertex+1) + std::to_string(i+1) + 
                                        " -O" + std::to_string((j->vertex)+1) + std::to_string(vertex+1) +
                                        " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n+1) + " 0\n";

                        // writing temp clause
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause1], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause3], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause5], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause6], false));

                        // pushing temp clause to clauses
                        clauses.push_back(temp_clause);
                        temp_clause.clear();
                        
                        num_clauses+= 2;
                    }else if (vertex != j->vertex) {
                        //std::cout << "Iterating Vertex- " << vertex + 1 << " Edge number " << e << " iteration " << n+1 << " from " << i+1 << " to " << j->vertex + 1 << std::endl;
                        
                        
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        count_clause2 = "O(" + std::to_string(i+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause3 = "O(" + std::to_string(vertex+1) + "," + std::to_string((j->vertex)+1) + ")";
                        count_clause4 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n+1) + ")";
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var;
                            num_var++;
                        }
                        if(clause_map.find(count_clause4) == clause_map.end()){
                            clause_map[count_clause4] = num_var;
                            num_var++;
                        }

                        // Only check if the edge starts on the right side of the vertex and ends on it
                        counter_clause += "--C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) + 
                                        " -O" + std::to_string(i+1) + std::to_string(vertex+1) + 
                                        " -O" + std::to_string(vertex+1) + std::to_string((j->vertex)+1) + 
                                        " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n+1) + " 0\n";

                        // writing temp clause
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause1], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause2], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause3], true));
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause4], false));

                        // pushing temp clause to clauses
                        clauses.push_back(temp_clause);
                        temp_clause.clear();

                        num_clauses++;
                    }
                }
                
                e++; 
            }                 
        }
    }
    

    solver.set_num_threads(4);
    solver.new_vars(num_var+1);

    for(auto clause : clauses){
        std::cout << "Adding clause: "<< clause << std::endl;
        solver.add_clause(clause);
    }

    std::cout << "Number of variables: " << num_var+1 << std::endl;
    std::cout << "Number of clauses: " << num_clauses << std::endl; 



    CMSat::lbool ret = solver.solve(&assumptions);
    if(ret == CMSat::l_True){
        std::cout << "SAT" << std::endl;
        std::vector<CMSat::lbool> model = solver.get_model();
        for(int i = 0; i < model.size(); i++){
            if(model[i] == CMSat::l_True){
                std::cout << "Variable " << i << " is true" << std::endl;
            }else{
                std::cout << "Variable " << i << " is false" << std::endl;
            }
        }
    }else if(ret == CMSat::l_False){
        std::cout << "UNSAT" << std::endl;
    }



    std::string comment  = "c Generated encoding file\n";
    std::string header = "p cnf " + std::to_string(num_var)
    + " " + std::to_string(num_clauses)
    + "\n";
    std::string encoding = comment + header + self_clause + transitive_clause + anti_reflixive_clause + counter_clause;
    write_encoding(encoding);
    write_clause_map(clause_map);

}
