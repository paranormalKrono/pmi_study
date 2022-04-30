#include <Windows.h> 
#include "MathPMI_Debug.h"
#include "Graphs.h"
#include "Graph_std.h"
#include "Graphs_algorithms.h"
#include "Menu.h"


void print_graph_property(int* print_vertexes_property(graph*), graph* g);
void graphs_dbg()
{
	printf_s("__GRAPHS_DEBUG__\nВыберите файл с графом:");
	unsigned char* path = "WorkFiles/";

	path = file_choice_menu(path);
	graph* g = graph_read(path);
	graph_print(g);
	
	int choice = 1;
	int Text_count = 6;
	const char* Text[6] = {
		"Ничего",
		"Проверка на двудольность BFS",
		"Проверка на двудольность DFS",
		"Топологическая сортировка",
		"Конденсация графа через транспонирование",
		"Конденсация графа"
	};
	while (choice)
	{
		printf_s("Выберите, что нужно сделать: ");
		choice = choice_menu_h(Text, Text_count);

		switch (choice)
		{
		case 1:
			print_graph_property(Bipartile_graph_BFS, g);
			break;
		case 2:
			print_graph_property(Bipartile_graph_DFS, g);
			break;
		case 3:
			print_graph_property(Topological_sort_graph, g);
			break;
		case 4:
			print_graph_property(Condensation_graph_transposition, g);
			break;
		case 5:
			print_graph_property(Condensation_graph, g);
			break;
		default:
			break;
		}
	}

	printf_s("\nВыберите файл для записи в него графа:");
	path = file_choice_menu(path);
	graph_write(g, path);

	free(path);
}

void print_graph_property(int *print_vertexes_property(graph*), graph* g) 
{
	int* result = print_vertexes_property(g);
	if (result) 
	{
		printf_s("[");
		for (int i = 0; i < g->count; ++i)
		{
			printf_s("%3d", i);
		}
		printf_s("  ]\n");
		printf_s("[");
		print_ints("%3d", result, g->count);
		printf_s("  ]\n");
		free(result);
	}
	else
	{
		fprintf_s(stderr, "Неверный граф.\n");
	}
}