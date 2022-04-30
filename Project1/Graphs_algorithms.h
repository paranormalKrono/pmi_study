#ifndef _GRAPHS_ALGORITHMS_H_
#define	_GRAPHS_ALGORITHMS_H_
#include "Graphs.h"
#include "Lessons/sq/stack_list.h"
#include "Lessons/sq/queue_list.h"

int* Bipartile_graph_BFS(graph* g);
int* Bipartile_graph_DFS(graph* g);
int* Topological_sort_graph(graph* g);
int* Condensation_graph_transposition(graph* g);
int* Condensation_graph(graph* g);

#endif // _GRAPHS_ALGORITHMS_H_