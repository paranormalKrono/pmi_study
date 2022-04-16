#include <Windows.h> 
#include "MathPMI_Debug.h"
#include "Graphs.h"
#include "Graph_std.h"
#include "Graphs_algorithms.h"
#include "Menu.h"


void print_graph_property(void* print_vertexes_property(graph*), graph* g);
void graphs_dbg()
{
	printf_s("__GRAPHS_DEBUG__\nВыберите файл с графом:");
	unsigned char* path = "WorkFiles/";

	path = file_choice_menu(path);
	graph* g = graph_read(path);
	graph_print(g);

	
	printf_s("Проверка на двудольность BFS\n");
	print_graph_property(Bipartile_graph_BFS, g);
	printf_s("Проверка на двудольность DFS\n");
	print_graph_property(Bipartile_graph_DFS, g);
	printf_s("Топологическая сортировка\n");
	print_graph_property(Topological_sort_graph, g);

	printf_s("\nВыберите файл для записи в него графа:");
	path = file_choice_menu(path);
	graph_write(g, path);

	free(path);
}

void print_graph_property(void *print_vertexes_property(graph*), graph* g) 
{
	printf_s("[");
	for (int i = 0; i < g->count; ++i) 
	{
		printf_s("%3d", i);
	}
	printf_s("  ]\n");
	printf_s("[");
	print_ints("%3d", print_vertexes_property(g), g->count);
	printf_s("  ]\n");
}