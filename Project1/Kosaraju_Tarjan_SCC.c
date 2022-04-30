#include "Graphs_algorithms.h"

int* scc_dfs1(graph* g)
{
	int* visit = (int*)calloc(sizeof(int) * g->count); // �������� �� �������
	int* vertexes_by_time = (int*)calloc(sizeof(int) * g->count); // ������ ������ �� �������

	int cur_time = g->count;
	stack* path = stack_alloc(); // �������� ����
	node* cur_node; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (visit[i]) continue; // ���� ��� ������, ����������

		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (!visit[cur_node->value]) // ���� �� ��������� ��� �������
			{
				vertexes_by_time[--cur_time] = cur_node->value;
				visit[cur_node->value] = 1;
				if (g->adj_list[cur_node->value].head) // ���� �� �� ���� ����
				{
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = g->adj_list[cur_node->value].head; // ��������� � ������ ��������� �������
					continue;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)); // ������������ �� �������� ����
			cur_node = cur_node->next;
		}
	}

	free(visit);
	stack_free(path);
	return vertexes_by_time;
}


int* scc_dfs2(graph* gT, int* vertexes_by_time)
{
	int* scc = (int*)calloc(sizeof(int) * gT->count); // 0 - �� ����������

	int cur_SCC_index = 0; // ������ ������� ���������� ������� ���������

	stack* path = stack_alloc(); // �������� ����
	node* cur_node; // ������� ��� �������
	for (int i = gT->count - 1; i > 0; --i)
	{
		if (scc[vertexes_by_time[i]]) continue; // ���� ��� ������, ����������

		++cur_SCC_index;
		cur_node = gT->adj_list[vertexes_by_time[i]].head;
		scc[vertexes_by_time[i]] = cur_SCC_index;

		while (cur_node)
		{
			if (!scc[cur_node->value]) // ���� �� ��������� ��� �������
			{
				scc[cur_node->value] = cur_SCC_index;
				if (gT->adj_list[cur_node->value].head) // ���� �� �� ���� ����
				{
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = gT->adj_list[cur_node->value].head; // ��������� � ������ ��������� �������
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
int* Condensation_graph_transposition(graph* g)
{
	int* vertexes_by_time = scc_dfs1(g);

	graph* gT = graph_transposition(g);
	int* scc = scc_dfs2(gT, vertexes_by_time);

	graph_free(gT);
	free(vertexes_by_time);

	return scc;
}

// ��� ��������������� ���� G. ����� � ������� ���� �����������.
// ����� ������� NULL ��� ������ ��������� ������
int* Condensation_graph(graph* g)
{
	int* times = (int*)calloc(sizeof(int) * g->count); // ����������
	int* low_link = (int*)calloc(sizeof(int) * g->count);
	int* on_stack = (int*)calloc(sizeof(int) * g->count); // ��������� �� ������� � �����
	if (!times || !low_link || !on_stack) return;

	stack* nodes_stack = stack_alloc(); // ���������� �������

	int node_id = 0;
	int cur_time = 1;
	stack* path = stack_alloc(); // �������� ����
	node* cur_node, * n, * head; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (times[i]) continue; // ���� ��� ������, ����������


		cur_node = g->adj_list[i].head;
		on_stack[i] = 1;
		times[i] = low_link[i] = cur_time;

		while (cur_node)
		{
			if (!times[cur_node->value]) // ���� �� ���������
			{
				stack_push(nodes_stack, cur_node);
				on_stack[cur_node->value] = 1;
				times[cur_node->value] = low_link[cur_node->value] = ++cur_time;

				if (g->adj_list[cur_node->value].head) // ���� �� �� ���� ����
				{
					head = cur_node;
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = g->adj_list[cur_node->value].head; // ��������� � ������ ��������� �������
					continue;
				}
			}
			else if (on_stack[cur_node->value] && times[cur_node->value] == low_link[cur_node->value]) // ���� ����� ���������� ������� ���������
			{
				while (stack_pop(nodes_stack, &n))
				{
					on_stack[n->value] = 0;
					low_link[n->value] = times[cur_node->value];
					if (n->value == cur_node->value) break;
				}
			}

			while (!cur_node->next && (node_id = cur_node->value, stack_pop(path, &cur_node))) // ������������ �� �������� ����
			{
				if (low_link[cur_node->value] > low_link[node_id])
					low_link[cur_node->value] = low_link[node_id]; // ������ ������� �������, ���� �� ������
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