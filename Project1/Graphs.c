#include <stdio.h>
#include "Graphs.h"

graph* graph_alloc() 
{
	graph* g = (graph*) malloc(sizeof(graph));
	g->count = 0;
	g->adj_list = NULL;
}

node* node_init(int value, node* next)
{
	node* n = (node*)malloc(sizeof(node));
	n->next = NULL;
	n->value = value;
}

graph* graph_init(int vertices_count)
{
	graph* g = (graph*)malloc(sizeof(graph));
	g->count = vertices_count;
	g->adj_list = (list*)malloc(vertices_count * sizeof(list));

	node* n;
	for (int i = 0; i < vertices_count; ++i)
		g->adj_list[i].head = NULL;//node_init(i, NULL);

	return g;
}

void add_arc(graph* g, int first, int second) // Установим указатель на вторую вершину в конец листа вершин, связанных с первой
{
	node* cur_node = g->adj_list[first].head;
	if (cur_node) 
	{
		while (cur_node->next) {
			if (cur_node->value == second) return;
			cur_node = cur_node->next;
		}
		cur_node->next = node_init(second, NULL);
	}
	else {
		g->adj_list[first].head = node_init(second, NULL);
	}
}

void add_edge(graph* g, int first, int second) {
	add_arc(g, first, second);
	add_arc(g, second, first);
}

void del_arc(graph* g, int first, int second) 
{
	// Проверяем существуют ли эти вершины
	if (first > g->count || second > g->count)
		return;

	// Проверяем основную вершину списка и убираем, если значение совпало
	node* cur_node = g->adj_list[first].head;
	if (cur_node) {
		if (cur_node->value == second)
		{
			g->adj_list[first].head = cur_node->next;
			free(cur_node);
			return;
		}

		while (cur_node->next)
		{
			// Убираем нод, если его значение совпало
			if (cur_node->next->value == second)
			{
				node* n = cur_node->next;
				cur_node->next = n->next;
				free(n);
				return;
			}
			cur_node = cur_node->next;
		}
	}
}

void del_edge(graph* g, int first, int second) {
	del_arc(g, first, second);
	del_arc(g, second, first);
}

void graph_print(graph* g) 
{
	node* cur_node;
	for (int i = 0; i < g->count; ++i)
	{
		printf("%d -", i);
		cur_node = g->adj_list[i].head;
		while (cur_node)
		{
			printf(" %d", cur_node->value);
			cur_node = cur_node->next;
		}
		printf("\n");
	}
}

void node_free(node* n)
{
	if (n->next) node_free(n->next);
	free(n);
} 

void graph_free(graph* g) 
{
	for (int i = 0; i < g->count; ++i) 
	{
		node_free(g->adj_list[i].head);
		free(g->adj_list[i]);
	}
	free(g->adj_list);
	free(g);
}