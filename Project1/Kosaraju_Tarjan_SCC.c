#include "Graphs_algorithms.h"

int* scc_dfs1(graph* g)
{
	int* visit = (int*)calloc(sizeof(int) * g->count); // пройдена ли вершина
	int* vertexes_by_time = (int*)calloc(sizeof(int) * g->count); // номера вершин по времени

	int cur_time = g->count;
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (visit[i]) continue; // Если уже прошли, пропускаем

		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (!visit[cur_node->value]) // Если не проходили эту вершину
			{
				vertexes_by_time[--cur_time] = cur_node->value;
				visit[cur_node->value] = 1;
				if (g->adj_list[cur_node->value].head) // Если из неё есть пути
				{
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = g->adj_list[cur_node->value].head; // Переходим к первой связанной вершине
					continue;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)); // Возвращаемся по вершинам пути
			cur_node = cur_node->next;
		}
	}

	free(visit);
	stack_free(path);
	return vertexes_by_time;
}


int* scc_dfs2(graph* gT, int* vertexes_by_time)
{
	int* scc = (int*)calloc(sizeof(int) * gT->count); // 0 - не пройденные

	int cur_SCC_index = 0; // Индекс текущей компоненты сильной связности

	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // текущий нод вершины
	for (int i = gT->count - 1; i > 0; --i)
	{
		if (scc[vertexes_by_time[i]]) continue; // Если уже прошли, пропускаем

		++cur_SCC_index;
		cur_node = gT->adj_list[vertexes_by_time[i]].head;
		scc[vertexes_by_time[i]] = cur_SCC_index;

		while (cur_node)
		{
			if (!scc[cur_node->value]) // Если не проходили эту вершину
			{
				scc[cur_node->value] = cur_SCC_index;
				if (gT->adj_list[cur_node->value].head) // Если из неё есть пути
				{
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = gT->adj_list[cur_node->value].head; // Переходим к первой связанной вершине
					continue;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)); // Возвращаемся по вершинам пути
			cur_node = cur_node->next;
		}
	}

	stack_free(path);
	return scc;
}


// Через транспонирование. Дан ориентированный граф G. Петли и кратные рёбра допускаются.
int* Condensation_graph_transposition(graph* g)
{
	int* vertexes_by_time = scc_dfs1(g);

	graph* gT = graph_transposition(g);
	int* scc = scc_dfs2(gT, vertexes_by_time);

	graph_free(gT);
	free(vertexes_by_time);

	return scc;
}

// Дан ориентированный граф G. Петли и кратные рёбра допускаются.
// Может вернуть NULL при ошибке выделения памяти
int* Condensation_graph(graph* g)
{
	int* times = (int*)calloc(sizeof(int) * g->count); // компонента
	int* low_link = (int*)calloc(sizeof(int) * g->count);
	int* on_stack = (int*)calloc(sizeof(int) * g->count); // находится ли вершина в стеке
	if (!times || !low_link || !on_stack) return;

	stack* nodes_stack = stack_alloc(); // пройденные вершины

	int node_id = 0;
	int cur_time = 1;
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node, * n, * head; // текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (times[i]) continue; // Если уже прошли, пропускаем


		cur_node = g->adj_list[i].head;
		on_stack[i] = 1;
		times[i] = low_link[i] = cur_time;

		while (cur_node)
		{
			if (!times[cur_node->value]) // если не проходили
			{
				stack_push(nodes_stack, cur_node);
				on_stack[cur_node->value] = 1;
				times[cur_node->value] = low_link[cur_node->value] = ++cur_time;

				if (g->adj_list[cur_node->value].head) // Если из неё есть пути
				{
					head = cur_node;
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = g->adj_list[cur_node->value].head; // Переходим к первой связанной вершине
					continue;
				}
			}
			else if (on_stack[cur_node->value] && times[cur_node->value] == low_link[cur_node->value]) // Если нашли компоненту сильной связности
			{
				while (stack_pop(nodes_stack, &n))
				{
					on_stack[n->value] = 0;
					low_link[n->value] = times[cur_node->value];
					if (n->value == cur_node->value) break;
				}
			}

			while (!cur_node->next && (node_id = cur_node->value, stack_pop(path, &cur_node))) // Возвращаемся по вершинам пути
			{
				if (low_link[cur_node->value] > low_link[node_id])
					low_link[cur_node->value] = low_link[node_id]; // Меняем уровень вершины, если он меньше
			}
			cur_node = cur_node->next;
		}
		on_stack[i] = 0;
	}

	free(on_stack);
	free(times);
	stack_free(nodes_stack);
	stack_free(path);
	return low_link;
}