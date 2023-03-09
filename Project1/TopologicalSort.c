#include "Graphs_algorithms.h"
#include <malloc.h>

// ���������� ������� ������
int* Topological_sort_graph(const graph* g)
{
	int* order = (int*)calloc(g->count, sizeof(int) * g->count); // 0 - �� ����������
	if (!order) return NULL;

	int cur_max = g->count - 1; // ������������ ��������� ��������
	stack* path = stack_alloc(); // �������� ����
	node* cur_node; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (!order[i]) // ���� �� �������� ��� �������
		{
			cur_node = g->adj_list[i].head;

			while (cur_node)
			{
				if (!order[cur_node->vertex]) // ���� �� ��������� ��� �������
				{
					if (g->adj_list[cur_node->vertex].head) // ���� �� �� ���� ����
					{
						stack_push(path, cur_node); // ��������� ������� � ����
						cur_node = g->adj_list[cur_node->vertex].head; // ��������� � ������ ��������� �������
						continue;
					}
					else
						order[cur_node->vertex] = cur_max--;
				}
				while (!cur_node->next && stack_pop(path, &cur_node)) // ������������, ���� ������ ��� ����� �� �������� �������
					order[cur_node->vertex] = cur_max--;
				cur_node = cur_node->next;
			}
			// ��������� �������
			order[i] = cur_max--;
		}
	}

	stack_free(path);
	return order;
}

// ���������� ������ ������ � �������������� �������
int* Topological_sort_Tarjan_graph(const graph* g)
{
	int* top_order = (int*)calloc(g->count, sizeof(int) * g->count);
	int* col = (int*)calloc(g->count, sizeof(int) * g->count); // 0 - �� ����������
	if (!top_order || !col) return NULL;

	int cur_order = g->count - 1;
	stack* path = stack_alloc(); // �������� ����
	node* cur_node; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (!col[i]) // ���� �� �������� ��� �������
		{
			col[i] = 1;
			cur_node = g->adj_list[i].head;

			while (cur_node)
			{
				if (!col[cur_node->vertex]) // ���� �� ��������� ��� �������
				{
					col[cur_node->vertex] = 1;
					if (g->adj_list[cur_node->vertex].head) // ���� �� �� ���� ����
					{
						stack_push(path, cur_node); // ��������� ������� � ����
						cur_node = g->adj_list[cur_node->vertex].head; // ��������� � ������ ��������� �������
						continue;
					}
					else
					{
						top_order[cur_order--] = cur_node->vertex;
						col[cur_node->vertex] = 2;
					}
				}
				while (!cur_node->next && stack_pop(path, &cur_node)) // ������������, ���� ������ ��� ����� �� �������� �������
				{
					top_order[cur_order--] = cur_node->vertex;
					col[cur_node->vertex] = 2;
				}
				cur_node = cur_node->next;
			}
			// ��������� �������
			top_order[cur_order--] = i;
			col[i] = 2; // ׸����
		}
	}

	stack_free(path);
	return top_order;
}