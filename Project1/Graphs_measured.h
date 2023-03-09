#ifndef	_GRAPHS_MEASURED_H_
#define	_GRAPHS_MEASURED_H_

typedef struct graph_m
{
	int count;
	struct list_m* adj_list;
} graph_m;

typedef struct list_m
{
	float weight;
	struct node_m* head;
} list_m;

typedef struct node_m
{
	int vertex;
	float weight;
	struct node_m* next;
} node_m;


graph_m* graph_m_alloc();
graph_m* graph_m_init(int n);
graph_m* graph_m_copy(const graph_m* g);
graph_m* graph_m_transposition(graph_m* g);
node_m* node_m_init(int value, float weight, node_m* next);
void node_m_free(node_m* n);
void add_m_arc(graph_m* g, int first, int second, float weight);
void del_m_arc(graph_m* g, int first, int second);
void add_m_edge(graph_m* g, int first, int second, float weight);
void del_m_edge(graph_m* g, int first, int second);
void graph_m_print(graph_m* g);
void graph_m_free(graph_m* g);


#endif // _GRAPHS_MEASURED_H_