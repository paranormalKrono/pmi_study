#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "Graphs_measured.h"

graph_m* graph_m_alloc()
{
	graph_m* g = (graph_m*)malloc(sizeof(graph_m));
	if (g)
	{
		g->count = 0;
		g->adj_list = NULL;
	}
	return g;
}

node_m* node_m_init(int value, float weight, node_m* next)
{
	node_m* n = (node_m*)malloc(sizeof(node_m));
	if (n)
	{
		n->next = next;
		n->vertex = value;
		n->weight = weight;
	}
	return n;
}

graph_m* graph_m_init(int vertices_count)
{
	graph_m* g = (graph_m*)malloc(sizeof(graph_m));
	if (g)
	{
		g->count = vertices_count;
		g->adj_list = (list_m*)malloc(vertices_count * sizeof(list_m));

		if (!g->adj_list)
		{
			printf_s("[Критическая ошибка] Не выделена память под adj_list для графа\n");
			free(g);
			return NULL;
		}

		for (int i = 0; i < vertices_count; ++i)
		{
			g->adj_list[i].head = NULL;
			g->adj_list[i].weight = 0;
		}
	}

	return g;
}

graph_m* graph_m_copy(const graph_m* g)
{
	graph_m* new_graph_m = graph_m_init(g->count);

	node_m* cur_node_m;
	for (int i = 0; i < g->count; ++i)
	{
		new_graph_m->adj_list[i].weight = g->adj_list->weight;
		cur_node_m = g->adj_list[i].head;
		while (cur_node_m)
		{
			add_m_arc(new_graph_m, i, cur_node_m->vertex, cur_node_m->weight);

			cur_node_m = cur_node_m->next;
		}
	}
	/*graph_m* new_graph_m = malloc(sizeof(g));
	if (!new_graph_m) return NULL;
	memcpy(new_graph_m, g, sizeof(g));
	if (!new_graph_m) return NULL;*/
	return new_graph_m;
}


graph_m* graph_m_transposition(graph_m* g)
{
	graph_m* new_graph_m = graph_m_init(g->count);

	node_m* cur_node_m;
	for (int i = 0; i < g->count; ++i)
	{
		new_graph_m->adj_list[i].weight = g->adj_list[i].weight;
		cur_node_m = g->adj_list[i].head;
		while (cur_node_m)
		{
			add_m_arc(new_graph_m, cur_node_m->vertex, i, cur_node_m->weight);

			cur_node_m = cur_node_m->next;
		}
	}

	return new_graph_m;
}

void add_m_arc(graph_m* g, int first, int second, float weight) // Установим указатель на вторую вершину в конец листа вершин, связанных с первой
{
	node_m* cur_node_m = g->adj_list[first].head;
	if (cur_node_m)
	{
		while (cur_node_m->next)
		{
			if (cur_node_m->vertex == second) return;
			cur_node_m = cur_node_m->next;
		}
		if (cur_node_m->vertex == second) return;
		cur_node_m->next = node_m_init(second, weight, NULL);
	}
	else
	{
		g->adj_list[first].head = node_m_init(second, weight, NULL);
	}
}

void add_m_edge(graph_m* g, int first, int second, float weight) 
{
	add_m_arc(g, first, second, weight);
	add_m_arc(g, second, first, weight);
}

void del_m_arc(graph_m* g, int first, int second)
{
	// Проверяем существуют ли эти вершины
	if (first > g->count || second > g->count)
		return;

	// Проверяем основную вершину списка и убираем, если значение совпало
	node_m* cur_node_m = g->adj_list[first].head;
	if (cur_node_m) {
		if (cur_node_m->vertex == second)
		{
			g->adj_list[first].head = cur_node_m->next;
			free(cur_node_m);
			return;
		}

		while (cur_node_m->next)
		{
			// Убираем нод, если его значение совпало
			if (cur_node_m->next->vertex == second)
			{
				node_m* n = cur_node_m->next;
				cur_node_m->next = n->next;
				node_m_free(n);
				return;
			}
			cur_node_m = cur_node_m->next;
		}
	}
}

void del_m_edge(graph_m* g, int first, int second)
{
	del_m_arc(g, first, second);
	del_m_arc(g, second, first);
}


void graph_m_print(graph_m* g)
{
	node_m* cur_node_m;
	for (int i = 0; i < g->count; ++i)
	{
		printf_s("%d [%.3f] -", i, g->adj_list[i].weight);
		cur_node_m = g->adj_list[i].head;
		while (cur_node_m)
		{
			printf_s(" %d [%.3f]", cur_node_m->vertex, cur_node_m->weight);
			cur_node_m = cur_node_m->next;
		}
		printf_s("\n");
	}
}

void node_m_free(node_m* n)
{
	if (n->next) node_m_free(n->next);
	free(n);
}

void graph_m_free(graph_m* g)
{
	for (int i = 0; i < g->count; ++i)
	{
		if (g->adj_list[i].head)
		{
			node_m_free(g->adj_list[i].head);
		}
	}
	free(g->adj_list);
	free(g);
}