#ifndef _GRAPH_STD_H_
#define	_GRAPH_STD_H_
#include "Graphs.h"
#include "Graphs_measured.h"

typedef enum graph_std_type
{
	Adjective_lists,
	Adjective_matrix
} graph_std_type;

graph* graph_read(graph_std_type t, char* path);
graph* graph_write(graph_std_type t, graph* g, char* path);

graph* graph_read_adj_lists(char *path); 
int graph_write_adj_lists(graph *g, char* path);

graph* graph_read_adj_matrix(char* path);
int graph_write_adj_matrix(graph* g, char* path);

graph_m* graph_m_read(graph_std_type t, char* path);
graph_m* graph_m_write(graph_std_type t, graph_m* g, char* path);

graph_m* graph_m_read_adj_lists(char* path);
int graph_m_write_adj_lists(graph_m* g, char* path);

#endif // _GRAPH_STD_H_