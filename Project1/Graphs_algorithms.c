#include <stdio.h>
#include "Graphs_algorithms.h"

int* Bipartile_graph_BFS(graph* g)
{
	int* parity = (int*)calloc(g->count, sizeof(int)); // 0 - не пройденные, 1 - нечёт, 2 - чёт


	node* cur_node;
	node* front_node;// = (node*)malloc(sizeof(node));
	queue* front = queue_alloc(); // фронт
	for (int i = 0; i < g->count; ++i) // Смотрим все вершины
	{
		if (parity[i]) continue;
		parity[i] = 1;
		cur_node = g->adj_list[i].head;
		while (cur_node) // отправляем связанные вершины во фронт, если не проходили
		{
			if (!parity[cur_node->value])
			{
				parity[cur_node->value] = 2;
				queue_push(front, cur_node);
			}
			cur_node = cur_node->next;
		}

		while (queue_pop(front, &front_node) && front_node) // Вытаскиваем из очереди вершины фронта
		{
			cur_node = g->adj_list[front_node->value].head;
			while (cur_node) // Смотрим вершины соединённые с фронтовой
			{
				if (!parity[cur_node->value]) // Если не проходили
				{
					parity[cur_node->value] = parity[front_node->value] == 1 ? 2 : 1;
					queue_push(front, cur_node);
				}
				else if (parity[cur_node->value] == parity[front_node->value]) // Если чётности равны, то нашли нечётный цикл
				{
					queue_free(front);
					free(parity);
					return NULL;
				}
				cur_node = cur_node->next;
			}
		}
	}
	queue_free(front);
	return parity;
}


int* Bipartile_graph_DFS(graph* g)
{
	int* parity = (int*)calloc(sizeof(int) * g->count); // 0 - не пройденные, 1 - нечёт, 2 - чёт

	stack* path = stack_alloc(); // Обратный путь
	int cur_main_parity; // текущая чётность основной вершины
	node* cur_node; // текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (parity[i]) continue; // Если уже прошли, пропускаем

		cur_main_parity = 1;
		parity[i] = 1;
		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (parity[cur_node->value])
			{
				if (parity[cur_node->value] == cur_main_parity) // Если нашли нечётный цикл, возвращаем чётность некоторых вершин
					return parity;
			}
			else
			{
				parity[cur_node->value] = cur_main_parity == 1 ? 2 : 1; // Выставляем чётность
				if (g->adj_list[cur_node->value].head) // Если из неё есть пути
				{
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = g->adj_list[cur_node->value].head; // Переходим к первой связанной вершине
					cur_main_parity = cur_main_parity == 1 ? 2 : 1;
					continue;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)) // Возвращаемся по вершинам пути
			{
				cur_main_parity = cur_main_parity == 1 ? 2 : 1;
			}
			cur_node = cur_node->next;
		}
	}
	return parity;
}

int* Topological_sort_graph(graph* g)
{
	int* order = (int*)calloc(sizeof(int) * g->count); // 0 - не пройденные

	int cur_max = g->count-1; // Максимальное доступное значение
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // Текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (!order[i]) // Если не посещали эту вершину
		{
			cur_node = g->adj_list[i].head;

			while (cur_node) 
			{
				if (!order[cur_node->value]) // Если не проходили эту вершину
				{
					if (g->adj_list[cur_node->value].head) // Если из неё есть пути
					{
						stack_push(path, cur_node); // Добавляем текущую в путь
						cur_node = g->adj_list[cur_node->value].head; // Переходим к первой связанной вершине
						continue;
					}
					else
						order[cur_node->value] = cur_max--;
				}
				while (!cur_node->next && stack_pop(path, &cur_node)) // Возвращаемся, пока больше нет путей из основной вершины
					order[cur_node->value] = cur_max--;
				cur_node = cur_node->next;
			}
			// Вернулись обратно
			order[i] = cur_max--;
		}
	}
	return order;
}