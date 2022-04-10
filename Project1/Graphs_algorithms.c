#include <stdio.h>
#include "Graphs_algorithms.h"

int* Bipartile_graph_BFS(graph* g)
{
	int* parity = (int*)calloc(g->count, sizeof(int)); // 0 - не пройденные, 1 - нечёт, 2 - чёт

	parity[0] = 1;

	queue* front = queue_alloc(); // фронт
	node* cur_node = g->adj_list[0].head;
	while (cur_node) 
	{
		parity[cur_node->value] = 2;
		queue_push(front, cur_node);
		cur_node = cur_node->next;
	}

	node* front_node = (node*)malloc(sizeof(node));
	while (queue_pop(front, &front_node) && front_node) // Вытаскиваем из очереди одну вершину фронта
	{
		cur_node = g->adj_list[front_node->value].head;
		while (cur_node) // Смотрим вершины соединённые с фронтовой
		{ 
			if (!parity[cur_node->value]) // Если не прошли
			{
				parity[cur_node->value] = parity[front_node->value] == 1 ? 2 : 1;
				queue_push(front, cur_node);
			}
			else if (parity[cur_node->value] == parity[front_node->value]) // Если чётности равны, то нашли нечётный цикл
			{
				queue_free(front);
				return parity;
			}
			cur_node = cur_node->next;
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
					stack_push(path, cur_node); // Добавляем её в путь
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
	int* visit = (int*)malloc(sizeof(int) * g->count); // 0 - не пройденные
	for (int i = 0; i < g->count; ++i)
		visit[i] = 0;

	stack* chain = stack_alloc(); // Цепочка нодов вершин
	int min = g->count;
	node* cur_node; // текущий нод
	for (int i = 0; i < g->count; ++i)
	{
		if (visit[i] == 0)
		{
			visit[i] = 1;
			cur_node = g->adj_list[i].head;

			while (cur_node)
			{
				if (!visit[cur_node->value]) // Если не проходили эту вершину
				{
					visit[i] = 1;

					stack_push(chain, &cur_node); // Добавляем нод вершины в цепочку
					if (g->adj_list[cur_node->value].head)
						cur_node = g->adj_list[cur_node->value].head;
					else
						visit[cur_node->value] = min--;
				}
				else if (visit[cur_node->value])
				while (!cur_node->next && stack_pop(chain, &cur_node)) // Возвращаемся по цепочке, если больше нет путей из основной вершины
					/*if (visit[)*/
					visit[cur_node->value];
				cur_node = cur_node->next;
			}

		}
	}
	return visit;
}