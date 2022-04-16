#include <stdio.h>
#include "Graphs_algorithms.h"

int* Bipartile_graph_BFS(graph* g)
{
	int* parity = (int*)calloc(g->count, sizeof(int)); // 0 - �� ����������, 1 - �����, 2 - ���


	node* cur_node;
	node* front_node;// = (node*)malloc(sizeof(node));
	queue* front = queue_alloc(); // �����
	for (int i = 0; i < g->count; ++i) // ������� ��� �������
	{
		if (parity[i]) continue;
		parity[i] = 1;
		cur_node = g->adj_list[i].head;
		while (cur_node) // ���������� ��������� ������� �� �����, ���� �� ���������
		{
			if (!parity[cur_node->value])
			{
				parity[cur_node->value] = 2;
				queue_push(front, cur_node);
			}
			cur_node = cur_node->next;
		}

		while (queue_pop(front, &front_node) && front_node) // ����������� �� ������� ������� ������
		{
			cur_node = g->adj_list[front_node->value].head;
			while (cur_node) // ������� ������� ���������� � ���������
			{
				if (!parity[cur_node->value]) // ���� �� ���������
				{
					parity[cur_node->value] = parity[front_node->value] == 1 ? 2 : 1;
					queue_push(front, cur_node);
				}
				else if (parity[cur_node->value] == parity[front_node->value]) // ���� �������� �����, �� ����� �������� ����
				{
					queue_free(front);
					free(parity);
					return NULL;
				}
				cur_node = cur_node->next;
			}
		}
	}
	queue_free(front);
	return parity;
}


int* Bipartile_graph_DFS(graph* g)
{
	int* parity = (int*)calloc(sizeof(int) * g->count); // 0 - �� ����������, 1 - �����, 2 - ���

	stack* path = stack_alloc(); // �������� ����
	int cur_main_parity; // ������� �������� �������� �������
	node* cur_node; // ������� ��� �������
	for (int i = 0; i < g->count; ++i)
	{
		if (parity[i]) continue; // ���� ��� ������, ����������

		cur_main_parity = 1;
		parity[i] = 1;
		cur_node = g->adj_list[i].head;

		while (cur_node)
		{
			if (parity[cur_node->value])
			{
				if (parity[cur_node->value] == cur_main_parity) // ���� ����� �������� ����, ���������� �������� ��������� ������
					return parity;
			}
			else
			{
				parity[cur_node->value] = cur_main_parity == 1 ? 2 : 1; // ���������� ��������
				if (g->adj_list[cur_node->value].head) // ���� �� �� ���� ����
				{
					stack_push(path, cur_node); // ��������� ������� � ����
					cur_node = g->adj_list[cur_node->value].head; // ��������� � ������ ��������� �������
					cur_main_parity = cur_main_parity == 1 ? 2 : 1;
					continue;
				}
			}
			while (!cur_node->next && stack_pop(path, &cur_node)) // ������������ �� �������� ����
			{
				cur_main_parity = cur_main_parity == 1 ? 2 : 1;
			}
			cur_node = cur_node->next;
		}
	}
	return parity;
}

int* Topological_sort_graph(graph* g)
{
	int* order = (int*)calloc(sizeof(int) * g->count); // 0 - �� ����������

	int cur_max = g->count-1; // ������������ ��������� ��������
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
	return order;
}