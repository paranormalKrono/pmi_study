#include "Graphs_algorithms.h"
#include <malloc.h>

int* Bipartile_graph_BFS(const graph* g)
{
	int* parity = (int*)calloc(g->count, sizeof(int)); // 0 - не пройденные, 1 - нечёт, 2 - чёт
	if (!parity) return NULL;

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
			if (!parity[cur_node->vertex])
			{
				parity[cur_node->vertex] = 2;
				queue_push(front, cur_node);
			}
			cur_node = cur_node->next;
		}

		while (queue_pop(front, &front_node) && front_node) // Вытаскиваем из очереди вершины фронта
		{
			cur_node = g->adj_list[front_node->vertex].head;
			while (cur_node) // Смотрим вершины соединённые с фронтовой
			{
				if (!parity[cur_node->vertex]) // Если не проходили
				{
					parity[cur_node->vertex] = parity[front_node->vertex] == 1 ? 2 : 1;
					queue_push(front, cur_node);
				}
				else if (parity[cur_node->vertex] == parity[front_node->vertex]) // Если чётности равны, то нашли нечётный цикл
				{
					queue_free(front);
					return;
				}
				cur_node = cur_node->next;
			}
		}
	}

	queue_free(front);
	return parity;
}


int* Bipartile_graph_DFS(const graph* g)
{
	int* parity = (int*)calloc(g->count, sizeof(int) * g->count); // 0 - не пройденные, 1 - нечёт, 2 - чёт
	if (!parity) return NULL;

	int cur_main_parity; // текущая чётность основной вершины
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (parity[i]) continue; // Если уже прошли, пропускаем

		cur_main_parity = 1;
		parity[i] = 1;
		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (parity[cur_node->vertex])
			{
				if (parity[cur_node->vertex] == cur_main_parity) // Если нашли нечётный цикл, возвращаем чётность некоторых вершин
					return parity;
			}
			else
			{
				parity[cur_node->vertex] = cur_main_parity == 1 ? 2 : 1; // Выставляем чётность
				if (g->adj_list[cur_node->vertex].head) // Если из неё есть пути
				{
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = g->adj_list[cur_node->vertex].head; // Переходим к первой связанной вершине
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

	stack_free(path);
	return parity;
}