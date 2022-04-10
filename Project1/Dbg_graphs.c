#include "MathPMI_Debug.h"
#include "Graphs.h"
#include "Graph_std.h"
#include "Graphs_algorithms.h"

void graphs_dbg()
{
	printf("¬ведите путь к файлу: \n");
	char* path = "WorkFiles/Graph.txt";
	graph* g = graph_read(path);
	graph_print(g);
	char* new_path = "WorkFiles/Graph_new.txt";
	graph_write(g, new_path);

	int* a = Bipartile_graph_BFS(g);
	for (int i = 0; i < g->count; ++i)
		printf("%d ", a[i]);
	printf("\n");

	int* b = Bipartile_graph_DFS(g);
	for (int i = 0; i < g->count; ++i)
		printf("%d ", b[i]);
	printf("\n");

	/*scanf_s("%d", &n);
	graph* g = graph_init(n);
	graph_print(g);*/
}