#include <Windows.h> 
#include "MathPMI_Debug.h"
#include "Graph_std.h"
#include "Menu_c.h"
#include "measure_calculating.h"
#include "Dbg_graphs.h"

void dfa_dbg();
void sat2(); 
void condensation();
void condensation_transposition();
void bipartile_bfs();
void bipartile_dfs();
void topological_sort();
void transitive_closure();
void graphs_dbg()
{
	printf_s("__GRAPHS_DEBUG__\n");

	const int text_count = 9;
	const char* Text[] = {
		"Конечные автоматы",
		"Проверка формулы 2-sat",
		"Конденсация графа",
		"Конденсация графа через транспонирование",
		"Проверка на двудольность BFS",
		"Проверка на двудольность DFS",
		"Топологическая сортировка",
		"Транзитивное замыкание",
		"Построение меры"
	};
	const void (*methods[])() = {
		dfa_dbg,
		sat2,
		condensation,
		condensation_transposition,
		bipartile_bfs,
		bipartile_dfs,
		topological_sort,
		transitive_closure,
		measure_calculating
	};

	printf_s("Выберите, что нужно сделать: ");
	int choice = choice_menu_h(Text, text_count);
	methods[choice]();
}


void print_variables(const variable** variables, int count);
graph* get_graph();
void write_graph(const graph* g);

void transitive_closure() 
{
	graph* g = get_graph();

	graph* g_new = transitive_closure_Purdom_graph(g);

	write_graph(g_new);
}



dfa* example_dfa1();
dfa* example_dfa2();
void dfa_print_properties(dfa* d, int x, int x_range, int x_range2);
void dfa_dbg()
{
	
	int x = 5;
	printf_s("Автомат проверяющий число %d\n", x);
	dfa* d = dfa_int(x);
	dfa_print_properties(d, 5, 10, 10);

	dfa* d1 = example_dfa1();
	printf_s("\nАвтомат проверяющий {01, 10}*\n");
	dfa_print_properties(d1, 6, 15, 15);

	dfa* d2 = example_dfa2();
	printf_s("\nАвтомат проверяющий {1 << n}*\n");
	dfa_print_properties(d2, 8, 15, 15);


	printf_s("\nАвтомат проверяющий {1 << n}* и {01, 10}*\n");
	dfa* d1_u_d2 = dfa_union(d1, d2);
	dfa_print_properties(d1_u_d2, 10, 15, 15);


	/*char* str = "(a^b)|c";
	variable vars [3] = {{"a", d1}, {"b", d2}, {"c", d}};
	dfa* d3 = dfa_get_str_result(str, vars, 3);
	dfa_print_properties(d3, 8, 15, 15);*/

	free(d);
	dfa_free(d1);
	dfa_free(d2);
	//dfa_free(d1_u_d2);
	//dfa_free(d3);
	printf_s("\n");
}

// {01, 10}*
dfa* example_dfa1() 
{
	dfa* d1 = dfa_init(4);
	dfa_add_arc(d1, 1, 2, 1);
	dfa_add_arc(d1, 1, 3, 0);
	dfa_add_arcs(d1, 2, 1, 0);
	dfa_add_arcs(d1, 3, 1, 1);
	d1->adj_states[1].state = state_end;
	return d1;
}

// 1 << n
dfa* example_dfa2()
{
	dfa* d2 = dfa_init(3);
	dfa_add_arcs(d2, 1, 2, 1);
	dfa_add_arc(d2, 2, 2, 0);
	dfa_add_arc(d2, 2, DFA_CORKSCREW_VERTEX, 1);
	d2->adj_states[2].state = state_end;
	return d2;
}

void dfa_print_properties(dfa* d, int x, int x_range, int x_range2) 
{
	dfa_print(d);

	printf_s("Результат проверки %d - %d\n", x, dfa_check_int(d, x));
	printf_s("Числа, которые может проверить этот автомат до %d:\n\n", x_range);
	dfa_print_range(d, x_range);

	dfa* d_ = dfa_implement(d);
	printf_s("Обратный автомат\n");
	dfa_print(d_);
	printf_s("Числа, которые может проверить обратный автомат до %d:\n\n", x_range2);
	dfa_print_range(d_, x_range2);
	free(d_);
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

int choice_graph_type();

graph* get_graph() 
{
	unsigned char* path = "WorkFiles/";
	printf_s("Выберите тип записи графа в файле для чтения: ");

	int choice = choice_graph_type();

	printf_s("Выберите файл с графом: ");
	path = file_choice_menu(path);

	graph* g = graph_read(choice, path);

	if (g) 
	{
		free(path);
		return g;
	}
	else
	{
		printf_s("Ошибка, нет графа.");
		free(path);
		return NULL;
	}

}

graph_m* get_graph_m()
{
	unsigned char* path = "WorkFiles/";
	printf_s("Выберите тип записи графа в файле для чтения: ");

	int choice = choice_graph_type();

	printf_s("Выберите файл с графом: ");
	path = file_choice_menu(path);

	graph_m* g = graph_m_read(choice, path);

	if (g)
	{
		free(path);
		return g;
	}
	else
	{
		printf_s("Ошибка, нет графа.");
		free(path);
		return NULL;
	}

}

void write_graph(const graph* g)
{
	char* path = "WorkFiles/";

	printf_s("Выберите тип записи графа в файл: ");
	int choice = choice_graph_type();

	printf_s("Выберите файл для записи графа: ");
	path = file_choice_menu(path);
	graph_write(choice, g, path);

	free(path);
}

void write_graph_m(const graph_m* g)
{
	char* path = "WorkFiles/";

	printf_s("Выберите тип записи графа в файл: ");
	int choice = choice_graph_type();

	printf_s("Выберите файл для записи графа: ");
	path = file_choice_menu(path);
	graph_m_write(choice, g, path);

	free(path);
}

int choice_graph_type() 
{
	const int types_count = 2;
	char* types_texts[] =
	{
		"Списки смежности",
		"Матрица смежности"
	};
	return choice_menu_h(types_texts, types_count);
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