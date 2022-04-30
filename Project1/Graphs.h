#ifndef	_GRAPHS_H_
#define	_GRAPHS_H_

typedef struct graph 
{
	int count;
	struct list* adj_list;
} graph;

typedef struct list
{
	struct node* head;
} list;

typedef struct node
{
	int value;
	struct node* next;
} node;


graph* graph_alloc();
graph* graph_init(int n);
graph* graph_transposition(graph* g);
node* node_init(int value, node* next);
void add_arc(graph* g, int first, int second);
void del_arc(graph* g, int first, int second);
void add_edge(graph* g, int first, int second);
void del_edge(graph* g, int first, int second);
void graph_print(graph *g);
void graph_free(graph *g);


#endif // _GRAPHS_H_