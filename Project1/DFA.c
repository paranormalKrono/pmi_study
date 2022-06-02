#include "DFA.h"
#include <stdio.h>

dfa* dfa_alloc()
{
	dfa* d = (dfa*)malloc(sizeof(dfa));

	if (!d) return NULL;

	d->count = 0;
	d->adj_states = NULL;

	return d;
}


dnode* dnode_init(int value, int vertex, dnode* next)
{
	dnode* n = (dnode*)malloc(sizeof(dnode));
	if (!n) return NULL;

	n->value = value;
	n->vertex = vertex;
	n->next = next;
}

// Количество состояний с учётом 1 штопорной вершины; Если вершин 0, то вернёт alloc
dfa* dfa_init(int states_count)
{
	if (states_count == 0) return dfa_alloc();
	dfa* d = (dfa*)malloc(sizeof(dfa));

	if (!d) return NULL;

	d->init_state = 1;
	d->count = states_count;
	d->adj_states = (dstate*)malloc(states_count * sizeof(dstate));
	if (!d->adj_states) 
	{
		free(d);
		return NULL;
	}

	dnode* n;
	for (int i = 0; i < states_count; ++i)
	{
		d->adj_states[i].state = state_unknown;
		d->adj_states[i].head = NULL;
	}
	dfa_add_arcs(d, DFA_CORKSCREW_VERTEX, DFA_CORKSCREW_VERTEX, 0); // Для штопора

	return d;
}

dfa* dfa_copy(dfa* d) 
{
	dfa* dc = dfa_init(d->count);
	dnode* cur_node;
	for (int i = 0; i < d->count; ++i)
	{
		cur_node = d->adj_states[i].head;
		while (cur_node)
		{
			dfa_add_arc(dc, i, cur_node->vertex, cur_node->value);
			cur_node = cur_node->next;
		}
	}
	return dc;
}

void dfa_free(dfa* d)
{
	for (int i = 0; i < d->count; ++i)
	{
		if (d->adj_states[i].head)
		{
			dnode_free(d->adj_states[i].head);
		}
	}
	free(d->adj_states);
	free(d);
}

// со штопором
void dfa_add_arcs(dfa* d, int first, int second, int value) 
{
	if (value) 
	{
		// first -> штопор & second
		dfa_add_arc(d, first, DFA_CORKSCREW_VERTEX, 0);
		dfa_add_arc(d, first, second, 1);
	}
	else
	{
		// first -> second & штопор
		dfa_add_arc(d, first, second, 0);
		dfa_add_arc(d, first, DFA_CORKSCREW_VERTEX, 1);
	}
}

void dfa_del_arcs(dfa* d, int first) 
{
	dnode_free(d->adj_states[first].head); // Удаляем путь из first в second и путь в штопор
	d->adj_states[first].head = NULL;
}

// в d из first в second по value = 0 | 1
void dfa_add_arc(dfa* d, int first, int second, int value)
{
	dnode* cur_node = d->adj_states[first].head;
	if (cur_node)
	{
		while (cur_node->next)
		{
			if (cur_node->value == value) return;
			if (cur_node->next->value > value) 
				cur_node->next = dnode_init(value, second, cur_node->next);
			cur_node = cur_node->next;
		}
		if (cur_node->value == value) return;
		if (cur_node->value > value)
			d->adj_states[first].head = dnode_init(value, second, cur_node);
		else cur_node->next = dnode_init(value, second, NULL);
	}
	else 
	{
		d->adj_states[first].head = dnode_init(value, second, NULL);
	}
}

void dfa_del_arc(dfa* d, int first, int second, int value)
{
	// Проверяем существуют ли эти вершины
	if (first > d->count || second > d->count)
		return;

	// Проверяем основную вершину списка и убираем, если значение совпало
	dnode* cur_node = d->adj_states[first].head;
	if (cur_node) 
	{
		if (cur_node->vertex == second && cur_node->value == value)
		{
			d->adj_states[first].head = cur_node->next;
			free(cur_node);
			return;
		}

		while (cur_node->next)
		{
			// Убираем нод, если его значение совпало
			if (cur_node->next->vertex == second && cur_node->value == value)
			{
				dnode* n = cur_node->next;
				cur_node->next = n->next;
				free(n);
				return;
			}
			cur_node = cur_node->next;
		}
	}
}

void dfa_print(dfa* g)
{
	printf_s(" %d\n", g->count);
	dnode* cur_node;
	
	for (int i = 0; i < g->count; ++i)
	{
		printf_s("[%2d] %8s -", i, g->adj_states[i].state == state_end ? "end" : "unknown");
		cur_node = g->adj_states[i].head;
		while (cur_node)
		{
			printf_s(" %d|%d", cur_node->vertex, cur_node->value);
			cur_node = cur_node->next;
		}
		printf_s("\n");
	}
}

void dnode_free(dnode* n)
{
	if (n->next) dnode_free(n->next);
	free(n);
}

