#ifndef ENCODING_H_
#define ENCODING_H_

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include "read_graph.h"
#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>



extern std::unordered_map <std::string, int> clause_map;
extern std::vector<std::tuple<int,int>> edge_list;
//extern CMSat::SATSolver solver;
// vector of vector for all clauses
//extern std::vector<std::vector<CMSat::Lit>> clauses;


std::tuple <bool, std::vector<CMSat::lbool>> create_encoding(Graph &inputGraph, int max_cutwidth);
void write_encoding(std::string clause);
void write_clause_map(std::unordered_map <std::string, int> clause_map);
std::string find_cutwidth(Graph &graph, int timeout);


#endif // !ENCODING_H_