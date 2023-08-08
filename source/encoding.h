#ifndef ENCODING_H_
#define ENCODING_H_

#include <string>
#include <unordered_map>
#include "read_graph.h"



extern std::unordered_map <std::string, int> clause_map;
void create_encoding(Graph &inputGraph, int max_cutwidth);
void write_encoding(std::string clause);
void write_clause_map(std::unordered_map <std::string, int> clause_map);



#endif // !ENCODING_H_