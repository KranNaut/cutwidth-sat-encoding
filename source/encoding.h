#ifndef ENCODING_H_
#define ENCODING_H_

#include <string>
#include "read_graph.h"



void create_encoding(Graph &inputGraph, int max_cutwidth);
void write_encoding(std::string clause);



#endif // !ENCODING_H_