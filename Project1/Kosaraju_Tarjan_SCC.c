#include "Graphs_algorithms.h"
#include <malloc.h>

int* scc_dfs1(const graph* g)
{
	int* visit = (int*)calloc(g->count, sizeof(int) * g->count); // �������� �� �������
	if (!visit) return NULL;

	stack* vertexes_by_times = stack_alloc(); // ������ ������ �� �������
	stack* path = stack_alloc(); // �������� ����
	node* cur_node; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (visit[i]) continue; // ���� ��� ������, ����������

		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (!visit[cur_node->vertex]) // ���� �� ��������� ��� �������
			{
				visit[cur_node->vertex] = 1;
				if (g->adj_list[cur_node->vertex].head) // ���� �� �� ���� ����
				{
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = g->adj_list[cur_node->vertex].head; // ��������� � ������ ��������� �������
					continue;
				}
				else
				{
					stack_push(vertexes_by_times, cur_node->vertex);
					visit[cur_node->vertex] = 2;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)) // ������������ �� �������� ����
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
	int* scc = (int*)calloc(gT->count, sizeof(int) * gT->count); // 0 - �� ����������
	if (!scc) return NULL;

	int cur_SCC_index = 0; // ������ ������� ���������� ������� ���������

	int id = 0;
	int* v = &id;
	stack* path = stack_alloc(); // �������� ����
	node* cur_node; // ������� ��� �������
	while (stack_pop(vertexes_by_time, v))
	{
		id = *v;
		if (scc[id]) continue; // ���� ��� ������, ����������

		++cur_SCC_index;
		cur_node = gT->adj_list[id].head;
		scc[id] = cur_SCC_index;

		while (cur_node)
		{
			if (!scc[cur_node->vertex]) // ���� �� ��������� ��� �������
			{
				scc[cur_node->vertex] = cur_SCC_index;
				if (gT->adj_list[cur_node->vertex].head) // ���� �� �� ���� ����
				{
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = gT->adj_list[cur_node->vertex].head; // ��������� � ������ ��������� �������
					continue;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)); // ������������ �� �������� ����
			cur_node = cur_node->next;
		}

	}

	stack_free(path);
	return scc;
}


// ����� ����������������. ��� ��������������� ���� G. ����� � ������� ���� �����������.
int* Condensation_graph_transposition(const graph* g)
{
	stack* vertexes_by_times = scc_dfs1(g);

	graph* gT = graph_transposition(g);
	int* scc = scc_dfs2(gT, vertexes_by_times);

	graph_free(gT);

	return scc;
}

// ��� ��������������� ���� G. ����� � ������� ���� �����������.
// ����� ������� NULL ��� ������ ��������� ������
int* Condensation_graph(const graph* g)
{
	int* times = (int*)calloc(g->count, sizeof(int) * g->count);
	int* low_link = (int*)calloc(g->count, sizeof(int) * g->count);
	int* component = (int*)calloc(g->count, sizeof(int) * g->count);
	if (!low_link || !times || !component) return NULL;

	int node_id = 0, cur_time = 1, cur_component = 1;
	stack* path = stack_alloc(); // �������� ����
	node* cur_node, *path_node; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (times[i]) continue; // ���� ��� ������, ����������

		int* on_stack = (int*)calloc(g->count, sizeof(int) * g->count); // ��������� �� ������� � �����
		if (!on_stack) return NULL;

		stack* nodes_stack = stack_alloc(); // ���������� �������

		cur_node = g->adj_list[i].head;
		on_stack[i] = 1;

		times[i] = low_link[i] = cur_time;
		component[i] = cur_component;

		while (cur_node)
		{
			if (!times[cur_node->vertex]) // ���� �� ���������
			{
				stack_push(nodes_stack, cur_node); // ��������� � ����
				on_stack[cur_node->vertex] = 1;

				times[cur_node->vertex] = low_link[cur_node->vertex] = ++cur_time; // �������������� low_link

				if (g->adj_list[cur_node->vertex].head) // ���� �� �� ���� ����
				{
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = g->adj_list[cur_node->vertex].head; // ��������� � ������ ��������� �������
					continue;
				}
			}
			else if (on_stack[cur_node->vertex] && times[cur_node->vertex] == low_link[cur_node->vertex]) // ���� ����� ���������� ������� ���������
			{
				while (stack_pop(nodes_stack, &path_node)) // ����������� �� ����� ������� ����
				{
					on_stack[path_node->vertex] = 0;
					low_link[path_node->vertex] = times[cur_node->vertex]; // ����������� �� low_link ���� �������
					component[path_node->vertex] = cur_component;
					if (path_node->vertex == cur_node->vertex) break; // ���� ������ � ���� �������, �� ��������� ����������
				}
				++cur_component;
			}

			while (!cur_node->next && (node_id = cur_node->vertex, stack_pop(path, &cur_node))) // ������������ �� �������� ����
			{
				if (on_stack[node_id]) 
				{
					on_stack[node_id] = 0;
					stack_pop(nodes_stack, &path_node);
				}
			}

			cur_node = cur_node->next;
		}

		//printf_s("["); // ��� ��������
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
	for (int i = 0; i < count; ++i) // ������� ������� 
	{
		cur_node = g->adj_list[i].head;

		while (cur_node) 
		{
			// ���� ���� ����� ������������ � ���� ��� ����, �� ������ ���� � ����� �����
			if (condensation[i] != condensation[cur_node->vertex]) add_arc(new_graph, condensation[i], condensation[cur_node->vertex]);
			cur_node = cur_node->next;
		}
	}
	return new_graph;
}