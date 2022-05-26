#include <Windows.h> 
#include "MathPMI_Debug.h"
#include "Graph_std.h"
#include "Menu.h"

void dfa_dbg();
void sat2(); 
void condensation();
void condensation_transposition();
void bipartile_bfs();
void bipartile_dfs();
void topological_sort();
void graphs_dbg()
{
	printf_s("__GRAPHS_DEBUG__\n");
	
	const int text_count = 8;
	const char* Text[8] = {
		"Конечные автоматы",
		"Проверка формулы 2-sat",
		"Конденсация графа",
		"Конденсация графа через транспонирование",
		"Проверка на двудольность BFS",
		"Проверка на двудольность DFS",
		"Топологическая сортировка",
		"Ничего"
	};
	const int nothing_choice = text_count - 1;

	int choice = 0;
	while (choice != nothing_choice)
	{
		printf_s("Выберите, что нужно сделать: ");
		choice = choice_menu_h(Text, text_count);

		switch (choice)
		{
		case 0:
			dfa_dbg();
			break;
		case 1:
			sat2();
			break;
		case 2:
			condensation();
			break;
		case 3:
			condensation_transposition();
			break;
		case 4:
			bipartile_bfs();
			break;
		case 5:
			bipartile_dfs();
			break;
		case 6:
			topological_sort();
			break;
		default:
			break;
		}
	}

	//print_graph();
}


void print_variables(const variable** variables, int count);
void print_graph_property(int* print_vertexes_property(graph*), graph* g);
graph* get_graph();
void write_graph(const graph* g);

void dfa_dbg()
{
	int x = 10;
	printf_s("Автомат проверяющий число %d\n", x);
	dfa* d = dfa_int(x);
	dfa_print(d);
	printf_s("\n");
	int result = dfa_check_int(d, x);
	printf_s("Результат проверки %d - %d\n\n", x, result);
	printf_s("Числа, которые может проверить этот автомат:\n");
	x = 20;
	dfa_print_range(d, x);
	free(d);

	// {01, 10}*
	printf_s("Автомат проверяющий {01, 10}*\n");
	dfa* d1 = dfa_init(4);
	dfa_add_arc(d1, DFA_START_VERTEX, 2, 1);
	dfa_add_arc(d1, DFA_START_VERTEX, 3, 0);
	dfa_add_arcs(d1, 2, DFA_START_VERTEX, 0);
	dfa_add_arcs(d1, 3, DFA_START_VERTEX, 1);
	d1->adj_states[DFA_START_VERTEX].state = state_end;

	dfa_print(d1);
	printf_s("\n");

	x = 100;
	result = dfa_check_int(d1, x);
	printf_s("Результат проверки %d - %d\n", x, result);
	printf_s("Числа, которые может проверить этот автомат:\n");
	dfa_print_range(d1, x);

	// 1 << n
	printf_s("Автомат проверяющий {1 << n}*\n");
	dfa* d2 = dfa_init(3);
	dfa_add_arcs(d2, DFA_START_VERTEX, 2, 1);
	dfa_add_arc(d2, 2, 2, 0);
	dfa_add_arc(d2, 2, DFA_CORKSCREW_VERTEX, 1);
	d2->adj_states[2].state = state_end;

	printf_s("\n");
	dfa_print(d2);
 
	x = 32768;
	result = dfa_check_int(d2, x);
	printf_s("Результат проверки %d - %d\n", x, result);
	printf_s("Числа, которые может проверить этот автомат:\n");
	dfa_print_range(d2, x);

	dfa_free(d1);
	dfa_free(d2);

	printf_s("\n");
}

void sat2() 
{
	// char* sat2_formula = "(x|y)&(~y|~z)&(z|~x)&(x|x)";
	char* sat2_formula = "(x\\/y)/\\(~y\\/~z)/\\(z\\/~x)/\\(x\\/x)";
	printf_s("%s = 1\n", sat2_formula);

	variable** variables = (variable**)malloc(sizeof(variable*));
	int variables_count;

	graph* g = sat2_graph(sat2_formula, &variables, &variables_count);
	graph_print(g);

	sat2_check(g, variables, variables_count);
	printf_s("При таких значениях булевых переменных формула верна:\n");
	print_variables(variables, variables_count);

	graph_free(g);
	for (int i = 0; i < variables_count; ++i) variable_free(variables[i]);
	free(variables);
}
void condensation() 
{
	graph* g = get_graph();
	graph_print(g);
	print_graph_property(Condensation_graph, g);
	graph_free(g);
}
void condensation_transposition()
{
	graph* g = get_graph();
	graph_print(g);
	print_graph_property(Condensation_graph_transposition, g);
	graph_free(g);
}
void bipartile_bfs()
{
	graph* g = get_graph();
	graph_print(g);
	print_graph_property(Bipartile_graph_BFS, g);
	graph_free(g);
}
void bipartile_dfs()
{
	graph* g = get_graph();
	graph_print(g);
	print_graph_property(Bipartile_graph_DFS, g);
	graph_free(g);
}
void topological_sort()
{
	graph* g = get_graph();
	graph_print(g);
	print_graph_property(Topological_sort_graph, g);
	graph_free(g);
}

void write_graph(const graph* g) 
{
	char* path = "WorkFiles/";
	printf_s("\nВыберите файл для записи в него графа:");
	path = file_choice_menu(path);
	graph_write(g, path);

	free(path);
}


graph* get_graph() 
{
	unsigned char* path = "WorkFiles/";
	printf_s("Выберите файл с графом : ");

	path = file_choice_menu(path);
	graph* g = graph_read(path);
	free(path);
	return g;
}

void print_variables(const variable** variables, int count) 
{
	printf_s("[");
	for (int i = 0; i < count; ++i) 
	{
		printf_s("%3s", variables[i]->name);
	}
	printf_s("]\n"); 
	printf_s("[");
	for (int i = 0; i < count; ++i)
	{
		printf_s("%3d", variables[i]->value);
	}
	printf_s("]\n");
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
		fprintf_s(stderr, "Результат не был получен. Возможно, неверный граф.\n");
	}
}