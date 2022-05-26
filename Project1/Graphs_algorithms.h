#ifndef _GRAPHS_ALGORITHMS_H_
#define	_GRAPHS_ALGORITHMS_H_
#include "Graphs.h"
#include "Variables.h"
#include "Lessons/sq/stack_list.h"
#include "Lessons/sq/queue_list.h"

int* Bipartile_graph_BFS(const graph* g);
int* Bipartile_graph_DFS(const graph* g);
int* Topological_sort_graph(const graph* g);
int* Condensation_graph_transposition(const graph* g);
int* Condensation_graph(const graph* g); 
void sat2_check(const graph* g, variable** variables, const int variables_count);
graph* sat2_graph(char* sat2, variable*** variables, int* variables_count);

#endif // _GRAPHS_ALGORITHMS_H_