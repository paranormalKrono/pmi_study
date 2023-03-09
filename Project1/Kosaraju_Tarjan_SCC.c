#include "Graphs_algorithms.h"
#include <malloc.h>

int* scc_dfs1(const graph* g)
{
	int* visit = (int*)calloc(g->count, sizeof(int) * g->count); // пройдена ли вершина
	if (!visit) return NULL;

	stack* vertexes_by_times = stack_alloc(); // номера вершин по времени
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (visit[i]) continue; // Если уже прошли, пропускаем

		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (!visit[cur_node->vertex]) // Если не проходили эту вершину
			{
				visit[cur_node->vertex] = 1;
				if (g->adj_list[cur_node->vertex].head) // Если из неё есть пути
				{
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = g->adj_list[cur_node->vertex].head; // Переходим к первой связанной вершине
					continue;
				}
				else
				{
					stack_push(vertexes_by_times, cur_node->vertex);
					visit[cur_node->vertex] = 2;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)) // Возвращаемся по вершинам пути
				if (visit[cur_node->vertex] != 2)
				{
					stack_push(vertexes_by_times, cur_node->vertex);
					visit[cur_node->vertex] = 2;
				}
			cur_node = cur_node->next;
		}
		stack_push(vertexes_by_times, i);
	}

	free(visit);
	stack_free(path);
	return vertexes_by_times;
}


int* scc_dfs2(const graph* gT, stack* vertexes_by_time)
{
	int* scc = (int*)calloc(gT->count, sizeof(int) * gT->count); // 0 - не пройденные
	if (!scc) return NULL;

	int cur_SCC_index = 0; // Индекс текущей компоненты сильной связности

	int id = 0;
	int* v = &id;
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node; // текущий нод вершины
	while (stack_pop(vertexes_by_time, v))
	{
		id = *v;
		if (scc[id]) continue; // Если уже прошли, пропускаем

		++cur_SCC_index;
		cur_node = gT->adj_list[id].head;
		scc[id] = cur_SCC_index;

		while (cur_node)
		{
			if (!scc[cur_node->vertex]) // Если не проходили эту вершину
			{
				scc[cur_node->vertex] = cur_SCC_index;
				if (gT->adj_list[cur_node->vertex].head) // Если из неё есть пути
				{
					stack_push(path, cur_node); // Добавляем текущую в путь
					cur_node = gT->adj_list[cur_node->vertex].head; // Переходим к первой связанной вершине
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
int* Condensation_graph_transposition(const graph* g)
{
	stack* vertexes_by_times = scc_dfs1(g);

	graph* gT = graph_transposition(g);
	int* scc = scc_dfs2(gT, vertexes_by_times);

	graph_free(gT);

	return scc;
}

// Дан ориентированный граф G. Петли и кратные рёбра допускаются.
// Может вернуть NULL при ошибке выделения памяти
int* Condensation_graph(const graph* g)
{
	int* times = (int*)calloc(g->count, sizeof(int) * g->count);
	int* low_link = (int*)calloc(g->count, sizeof(int) * g->count);
	int* component = (int*)calloc(g->count, sizeof(int) * g->count);
	if (!low_link || !times || !component) return NULL;

	int node_id = 0, cur_time = 1, cur_component = 1;
	stack* path = stack_alloc(); // Обратный путь
	node* cur_node, *path_node; // текущий нод вершины
	for (int i = 0; i < g->count; ++i)
	{
		if (times[i]) continue; // Если уже прошли, пропускаем

		int* on_stack = (int*)calloc(g->count, sizeof(int) * g->count); // находится ли вершина в стеке
		if (!on_stack) return NULL;

		stack* nodes_stack = stack_alloc(); // пройденные вершины

		cur_node = g->adj_list[i].head;
		on_stack[i] = 1;

		times[i] = low_link[i] = cur_time;
		component[i] = cur_component;

		while (cur_node)
		{
			if (!times[cur_node->vertex]) // если не проходили
			{
				stack_push(nodes_stack, cur_node); // добавляем в стек
				on_stack[cur_node->vertex] = 1;

				times[cur_node->vertex] = low_link[cur_node->vertex] = ++cur_time; // инициализируем low_link

				if (g->adj_list[cur_node->vertex].head) // если из неё есть пути
				{
					stack_push(path, cur_node); // добавляем текущую в путь
					cur_node = g->adj_list[cur_node->vertex].head; // переходим к первой связанной вершине
					continue;
				}
			}
			else if (on_stack[cur_node->vertex] && times[cur_node->vertex] == low_link[cur_node->vertex]) // Если нашли компоненту сильной связности
			{
				while (stack_pop(nodes_stack, &path_node)) // вытаскиваем из стека вершину пути
				{
					on_stack[path_node->vertex] = 0;
					low_link[path_node->vertex] = times[cur_node->vertex]; // присваиваем ей low_link этой вершины
					component[path_node->vertex] = cur_component;
					if (path_node->vertex == cur_node->vertex) break; // если пришли к этой вершине, то завершили компоненту
				}
				++cur_component;
			}

			while (!cur_node->next && (node_id = cur_node->vertex, stack_pop(path, &cur_node))) // Возвращаемся по вершинам пути
			{
				if (on_stack[node_id]) 
				{
					on_stack[node_id] = 0;
					stack_pop(nodes_stack, &path_node);
				}
			}

			cur_node = cur_node->next;
		}

		//printf_s("["); // для проверки
		//print_ints("%3d", low_link, g->count);
		//printf_s("]\n");

		free(on_stack);
		stack_free(nodes_stack);
	}

	for (int i = 0; i < g->count; ++i) 
	{
		if (component[i] == 0) component[i] = cur_component++;
		else --component[i];
	}

	free(times);
	free(low_link);
	stack_free(path);
	return component;
}

graph* get_SCC_graph(graph* g, int* condensation, int count) 
{
	int scc_count = 0;
	for (int i = 0; i < count; ++i) if (scc_count < condensation[i]) scc_count = condensation[i];
	++scc_count;

	graph* new_graph = graph_init(scc_count);
	node* cur_node;
	for (int i = 0; i < count; ++i) // Обходим вершины 
	{
		cur_node = g->adj_list[i].head;

		while (cur_node) 
		{
			// Ищем дуги между компонентами и если они есть, то создаём дугу в новом графе
			if (condensation[i] != condensation[cur_node->vertex]) add_arc(new_graph, condensation[i], condensation[cur_node->vertex]);
			cur_node = cur_node->next;
		}
	}
	return new_graph;
}