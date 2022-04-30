#include <stdio.h>
#include "Graphs_algorithms.h"

int* Topological_sort_graph(graph* g)
{
	int* order = (int*)calloc(sizeof(int) * g->count); // 0 - �� ����������

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
				if (!order[cur_node->value]) // ���� �� ��������� ��� �������
				{
					if (g->adj_list[cur_node->value].head) // ���� �� �� ���� ����
					{
						stack_push(path, cur_node); // ��������� ������� � ����
						cur_node = g->adj_list[cur_node->value].head; // ��������� � ������ ��������� �������
						continue;
					}
					else
						order[cur_node->value] = cur_max--;
				}
				while (!cur_node->next && stack_pop(path, &cur_node)) // ������������, ���� ������ ��� ����� �� �������� �������
					order[cur_node->value] = cur_max--;
				cur_node = cur_node->next;
			}
			// ��������� �������
			order[i] = cur_max--;
		}
	}

	stack_free(path);
	return order;
}