#include "Graphs_algorithms.h"
#include <malloc.h>

int* Topological_sort_graph(const graph* g)
{
	int* order = (int*)calloc(g->count, sizeof(int) * g->count); // 0 - не пройденные
	if (!order) return NULL;

	int cur_max = g->count - 1; // Максимальное доступное значение
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // Текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (!order[i]) // Если не посещали эту вершину
		{
			cur_node = g->adj_list[i].head;

			while (cur_node)
			{
				if (!order[cur_node->vertex]) // Если не проходили эту вершину
				{
					if (g->adj_list[cur_node->vertex].head) // Если из неё есть пути
					{
						stack_push(path, cur_node); // Добавляем текущую в путь
						cur_node = g->adj_list[cur_node->vertex].head; // Переходим к первой связанной вершине
						continue;
					}
					else
						order[cur_node->vertex] = cur_max--;
				}
				while (!cur_node->next && stack_pop(path, &cur_node)) // Возвращаемся, пока больше нет путей из основной вершины
					order[cur_node->vertex] = cur_max--;
				cur_node = cur_node->next;
			}
			// Вернулись обратно
			order[i] = cur_max--;
		}
	}

	stack_free(path);
	return order;
}