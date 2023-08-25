#include "encoding.h"



#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
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
std::unordered_map <int, std::string> rev_clause_map;
int num_var = 1;


std::tuple <bool, std::vector<CMSat::lbool>> create_encoding(Graph &graph, int max_cutwidth) {
    
    CMSat::SATSolver solver;
    std::vector<std::vector<CMSat::Lit>> clauses;
    std::vector<CMSat::Lit> temp_clause;

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
            rev_clause_map[num_var] = self_clause1;
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
                    trans_clause1 = "O(" + std::to_string(j) + "," + std::to_string(k) + ")";
                    trans_clause2 = "O(" + std::to_string(k) + "," + std::to_string(i) + ")";
                    trans_clause3 = "O(" + std::to_string(j) + "," + std::to_string(i) + ")";
                    if(clause_map.find(trans_clause1) == clause_map.end()){
                        clause_map[trans_clause1] = num_var;
                        rev_clause_map[num_var] = trans_clause1;
                        num_var++;
                    }
                    if(clause_map.find(trans_clause2) == clause_map.end()){
                        clause_map[trans_clause2] = num_var;
                        rev_clause_map[num_var] = trans_clause2;
                        num_var++;
                    }
                    if(clause_map.find(trans_clause3) == clause_map.end()){
                        clause_map[trans_clause3] = num_var;
                        rev_clause_map[num_var] = trans_clause3;
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
                // reflixive_clause += "-O" + std::to_string(j) + std::to_string(k) + " -O"  + std::to_string(k) + std::to_string(j) + " 0\n";


                anti_ref_clause1 = "O(" + std::to_string(j) + "," + std::to_string(k) + ")";
                anti_ref_clause2 = "O(" + std::to_string(k) + "," + std::to_string(j) + ")";
                if(clause_map.find(anti_ref_clause1) == clause_map.end()){
                    clause_map[anti_ref_clause1] = num_var;
                    rev_clause_map[num_var] = anti_ref_clause1;
                    num_var++;
                }
                if(clause_map.find(anti_ref_clause2) == clause_map.end()){
                    clause_map[anti_ref_clause2] = num_var;
                    rev_clause_map[num_var] = anti_ref_clause2;
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
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")" ;
                        counter_clause += count_clause1 + " 0\n";
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var;
                            rev_clause_map[num_var] = count_clause1;
                            num_var++;
                        }
                        // writing temp clause
                        temp_clause.push_back(CMSat::Lit(clause_map[count_clause1], false));
                        // adding temp clause to clauses
                        clauses.push_back(temp_clause);
                        temp_clause.clear();
                    }

                    else if(n >= max_cutwidth){
                        assume = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";

                        if(clause_map.find(assume) == clause_map.end()){
                            clause_map[assume] = num_var;
                            rev_clause_map[num_var] = assume;
                            num_var++;
                        }
                        assumptions.push_back(CMSat::Lit(clause_map[assume], true));
                    }
                    
                    

                    if((vertex != i && vertex != j->vertex) ){
                        count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                        count_clause2 = "O(" + std::to_string(i+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause3 = "O(" + std::to_string(vertex+1) + "," + std::to_string(i+1) + ")";
                        count_clause4 = "O(" + std::to_string(vertex+1) + "," + std::to_string((j->vertex)+1) + ")";
                        count_clause5 = "O(" + std::to_string((j->vertex)+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause6 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n+1) + ")" ;
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var;
                            rev_clause_map[num_var] = count_clause1;
                            num_var++;
                        }
                        if(clause_map.find(count_clause6) == clause_map.end()){
                            clause_map[count_clause6] = num_var;
                            rev_clause_map[num_var] = count_clause6;
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
                        count_clause2 = "O(" + std::to_string(vertex+1) + "," + std::to_string(vertex+1) + ")";
                        count_clause3 = "O(" + std::to_string(vertex+1) + "," + std::to_string((j->vertex)+1) + ")";
                        count_clause4 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n+1) + ")" ;
                        if(clause_map.find(count_clause1) == clause_map.end()){
                            clause_map[count_clause1] = num_var;
                            rev_clause_map[num_var] = count_clause1;   
                            num_var++;
                        }
                        if(clause_map.find(count_clause4) == clause_map.end()){
                            clause_map[count_clause4] = num_var;
                            rev_clause_map[num_var] = count_clause4;
                            num_var++;
                        }

                        
                        counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) + 
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
                

                      
                    count_clause1 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e) + "," + std::to_string(n) + ")";
                    count_clause4 = "C(" + std::to_string(vertex+1) + "," + std::to_string(e+1) + "," + std::to_string(n) + ")";
                    if(clause_map.find(count_clause1) == clause_map.end()){
                        clause_map[count_clause1] = num_var;
                        rev_clause_map[num_var] = count_clause1;   
                        num_var++;
                    }
                    if(clause_map.find(count_clause4) == clause_map.end()){
                        clause_map[count_clause4] = num_var;
                        rev_clause_map[num_var] = count_clause4;
                        num_var++;
                    }

                    counter_clause += "-C" + std::to_string(vertex+1) + std::to_string(e) + std::to_string(n) + 
                                    " C" + std::to_string(vertex+1) + std::to_string(e+1) + std::to_string(n) + " 0\n";

                    //writing temp clause
                    temp_clause.push_back(CMSat::Lit(clause_map[count_clause1], true));
                    temp_clause.push_back(CMSat::Lit(clause_map[count_clause4], false));

                    // pushing temp clause to clauses
                    clauses.push_back(temp_clause);
                    temp_clause.clear();

                    num_clauses++;

                    
                
                }
               e++;
            }              
        }
    }
    

    solver.set_num_threads(4);
    solver.new_vars(num_var);

    for(auto clause : clauses){
        solver.add_clause(clause);
    } 

    write_clause_map(clause_map);


    CMSat::lbool ret = solver.solve(&assumptions);

    if(ret == CMSat::l_True){
        std::cout << "SAT" << std::endl;
        std::vector<CMSat::lbool> model = solver.get_model();
        for(int i = 0; i < model.size(); i++){
            if(model[i] == CMSat::l_True){
                //std::cout<<"True "<< rev_clause_map[i] << std::endl;
                //std::cout<< counter_clause << std::endl;
            }
        }

        return std::make_tuple(true, model);
    }else if(ret == CMSat::l_False){
        std::cout << "UNSAT" << std::endl;
        return std::make_tuple(false, std::vector<CMSat::lbool>());
        
        
    }



    std::string comment  = "c Generated encoding file\n";
    std::string header = "p cnf " + std::to_string(num_var)
    + " " + std::to_string(num_clauses)
    + "\n";
    std::string encoding = comment + header + self_clause + transitive_clause + anti_reflixive_clause + counter_clause;
    write_encoding(encoding);
    

}




std::string find_cutwidth(Graph &graph, int timeout){

    int delta = 0;
    int counter = 0;
    std::time_t t1 = std::time(0);
    for(int i = 0; i < graph.num_vertices; i++){
        counter = 0;
        for(auto current = graph.adjacency_list[i]; current; current = current->next){
            counter+=1;
        }
        if (counter>delta){
            delta = counter;
        }
    }
    std::cout << "Delta: " << delta << std::endl;



    //binary search for the cutwidth
    int lower = delta/2;
    int upper = graph.num_edges;
    int mid = (lower + upper)/2;
    bool result;
    std::vector<CMSat::lbool> model;
    std::vector<CMSat::lbool> temp_model;
    std::string output = "";


    while(upper>lower){
        std::cout << "Lower: " << lower << std::endl;
        std::cout << "Upper: " << upper << std::endl;
        std::cout << "Mid: " << mid << std::endl;
        std::time_t t2 = std::time(0);
        if(t2-t1 > timeout){
            output = "Upper bound: " + std::to_string(upper) + "\n" +
                     "Lower bound: " + std::to_string(lower) + "\n";
            return output;
        }
        tie(result, temp_model) = create_encoding(graph, mid);
        if(result == true){
            std::cout << "SAT" << std::endl;
            model = temp_model;
            upper = mid;
            mid = (lower + upper)/2;
        }else if(result == false){
            std::cout << "UNSAT" << std::endl;
            lower = mid + 1;
            mid = (lower + upper)/2;
        }
    }
    output = "\nCutwidth: " + std::to_string(mid-1) + "\n";

    
    std::unordered_map <int, int> order_map = {};
    for(int i=0; i < model.size(); i++){
        if(model[i] == CMSat::l_True){
            //std::cout<<"True "<< rev_clause_map[i] << std::endl;
            std::string clause = rev_clause_map[i];
            if(clause[0] == 'O') {
                std::string delimiter = ",";
                std::string token = clause.substr(2, clause.find(delimiter));
                int vertex = std::stoi(token);
                token = clause.substr(clause.find(delimiter)+1, clause.find(")")-clause.find(delimiter)-1);
                int vertex2 = std::stoi(token);
                if(vertex != vertex2){
                    order_map[vertex] = order_map[vertex] + 1;
                }
            }
        }
    }

    for(int i=0; i< graph.num_vertices; i++){
        for(int j =1; j<= graph.num_vertices; j++){
            if(order_map[j] == i){
                output +=  std::to_string(j) + " ";
            }
        }
    }

    return output;
}
