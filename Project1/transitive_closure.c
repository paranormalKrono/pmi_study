#include "Graphs_algorithms.h"
#include "MathPMI_Debug.h"
#include <malloc.h>

// Возвращает транзитивно замкнутый граф через конденсацию и топологическую сортировку
graph* transitive_closure_Purdom_graph(const graph* g)
{
	graph* g_new = graph_copy(g);

	int* cond = Condensation_graph(g);

	// Соединяем все вершины в компонентах сильной связности
	int c = 0;
	for (int i = 0; i < g->count - 1; ++i) 
	{
		c = cond[i];
		for (int j = i + 1; j < g->count; ++j) 
		{
			if (c == cond[j]) add_edge(g_new, i, j);
		}
	}
	
	graph* g_scc = get_SCC_graph(g, cond, g->count);

	int* topo = Topological_sort_Tarjan_graph(g_scc);

	/*printf_s("\n");
	graph_print(g_scc); 
	printf_s("[");
	for (int i = 0; i < g_scc->count; ++i)
	{
		printf_s("%3d", i);
	}
	printf_s("  ]\n");
	printf_s("[");
	print_ints("%3d", topo, g_scc->count);
	printf_s("  ]\n");*/


	int c1 = 0, c2 = 0;
	node* cur_node;
	stack* path = stack_alloc(); // Обратный путь

	for (int i = 0; i < g_scc->count; ++i) // Идём по компонентам
	{
		c1 = topo[i]; // Запоминаем первую компоненту
		int* col = (int*)calloc(g_scc->count, sizeof(int) * g_scc->count); // 0 - не пройденные
		if (!col) return NULL;

		cur_node = g_scc->adj_list[c1].head;

		while (cur_node)
		{
			c2 = cur_node->vertex;
			if (!col[c2])
			if (g_scc->adj_list[cur_node->vertex].head) // Если из неё есть пути
			{
				stack_push(path, cur_node); // Добавляем текущую в путь
				cur_node = g_scc->adj_list[cur_node->vertex].head; // Переходим к первой связанной вершине
				continue;
			}
			else
			{
				col[c2] = 1;
				add_arc(g_scc, c1, c2);

				// Соединяем вершины из этих компонент
				for (int k = 0; k < g->count; ++k)
				{
					if (cond[k] == c1)
					{
						for (int t = k + 1; t < g->count; ++t)
							if (cond[t] == c2) add_arc(g_new, k, t);
					}
					else if (cond[k] == c2)
					{
						for (int t = k + 1; t < g->count; ++t)
							if (cond[t] == c1) add_arc(g_new, t, k);
					}
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)) // Возвращаемся, пока нет путей из основной вершины
			{
				c2 = cur_node->vertex;
				col[c2] = 1;
				add_arc(g_scc, c1, c2);

				// Соединяем вершины из этих компонент
				for (int k = 0; k < g->count; ++k)
				{
					if (cond[k] == c1)
					{
						for (int t = k + 1; t < g->count; ++t)
							if (cond[t] == c2) add_arc(g_new, k, t);
					}
					else if (cond[k] == c2) 
					{
						for (int t = k + 1; t < g->count; ++t)
							if (cond[t] == c1) add_arc(g_new, t, k);
					}
				}
			}
			cur_node = cur_node->next;
		}
		free(col);
	}

	free(cond);
	free(topo);
	graph_free(g_scc);
	return g_new;
}
