#include "DFA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_binary(const int x);

// �������, ����������� ����� � ��������������� ������ {0}*{number}
dfa* dfa_int(int number)
{
	char* number_r2 = get_binary(number);

	dfa* d = dfa_init(strlen(number_r2) + 2); // ������ DFA �� �����, �������� ������
	char* cur_digit = number_r2;

	// check another (d->count) leading zeros of number
	// �������������� ���� ��� ��������� �������������� �����
	dfa_add_arc(d, DFA_START_VERTEX, DFA_START_VERTEX, 0);
	dfa_add_arcs(d, DFA_START_VERTEX, DFA_START_VERTEX + 1, (int)*(cur_digit++) - '0');
	int i = DFA_START_VERTEX + 1; 
	for (; *cur_digit != '\0'; ++i, ++cur_digit)
	{
		dfa_add_arcs(d, i, i + 1, (int)*cur_digit - '0'); // i -> i + 1 �� ������� ����� 0 ��� 1 � � ������
	}

	d->adj_states[i].state = state_end;
	
	return d;
}

int dfa_check_int(dfa* d, const int n) 
{
	char* number_r2 = get_binary(n);
	return dfa_check(d, number_r2);
}

// �������� 101, �� ����� �������� {0}*101
int dfa_check(dfa* d, const char* str) 
{
	char* cur_str = str;
	while (cur_str == '0') ++cur_str;

	dnode* cur_node = d->adj_states[1].head;
	int head_vertex = 0;
	int value; // �������� ��� �����
	while (cur_node)
	{
		value = *(cur_str++) - '0';
		if (value != cur_node->value) // ��������� ��������� �� �������� ���� �� ��������� �������
		{
			cur_node = cur_node->next;
			if (!cur_node || value != cur_node->value) return 0;
		}

		head_vertex = cur_node->vertex;
		cur_node = d->adj_states[head_vertex].head;

		if (d->adj_states[head_vertex].state == state_end && *cur_str == '\0') return 1;
	}
	return 0;
}

void int_add_if_not_contains(int x, int** ints, int* count);
int sort(int* a, int n);

// ������� ��� ����� �� 0 �� number
void dfa_print_range(dfa* d, int number)
{
	char* number_r2 = get_binary(number); 
	//printf_s("number_r2 = %s\n", number_r2);

	dnode* cur_node;
	int res = 0, prev_res = -1, steps = 0, new_step = INT_MAX; // ����������, ���� ������������
	int* results = (int*)malloc(sizeof(int));
	int res_count = 0;

	while (res != prev_res) // ������ ������� ����� �� �������� �������� ������
	{
		//printf_s("result node new_step step = %d\n", steps);
		prev_res = res;
		cur_node = d->adj_states[DFA_START_VERTEX].head;
		res = 0;
		for (int i = 0; cur_node; ++i)
		{
			if (cur_node->vertex == DFA_CORKSCREW_VERTEX) // ���� ������� ���� ���� � ������, ������ �����
			{
				cur_node = cur_node->next;
				if (!cur_node) break;
			}
			else if (cur_node->next && cur_node->next->vertex != DFA_CORKSCREW_VERTEX) // ���� ������������ ����� ����� 0 � 1
			{
				if (!(steps & (1 << i))) // ���� ��� �� ��� ������������, �������� ������� ����� � ���������� ��������� ����� ������������
				{
					if (cur_node->value > cur_node->next->value)
						cur_node = cur_node->next;
					new_step = i;
				}
			}

			res = cur_node->value + (res << 1); // ���������� ��������� ����� � ���������
			//printf_s("%6d %2d|%d %8d\n", res, cur_node->vertex, cur_node->value, new_step);

			if (res > number) break; // ����� �� ��������

			if (d->adj_states[cur_node->vertex].state == state_end)
				int_add_if_not_contains(res, &results, &res_count);

			cur_node = d->adj_states[cur_node->vertex].head; // �������� �� �����
		}

		//printf_s("_________\n");
		
		// ����������� ����� ������������
		steps += (1 << new_step) - (steps >> new_step << new_step); // 01101|01 -> 00000|11
	}

	sort(results, res_count);
	char* binary;
	for (int i = 0; i < res_count; ++i)
	{
		binary = get_binary(results[i]);
		printf_s("%10d = %10s\n", results[i], binary);
		free(binary);
	}
	free(results);
}

char* get_binary(const int x)
{
	char* x_r2 = (char*)malloc(sizeof(char) * _MAX_U64TOSTR_BASE2_COUNT);
	if (!x_r2) return NULL;
	_itoa_s(x, x_r2, _MAX_U64TOSTR_BASE2_COUNT, 2); // ��������� ����� � �������� �������
	return x_r2;
}

void int_add_if_not_contains(int x, int** ints, int* count)
{
	if (count)
	{
		for (int i = 0; i < *count; ++i)
			if ((*ints)[i] == x) return;
		int* new_ints = (int*)realloc(*ints, sizeof(int) * (++ * count));
		if (new_ints)
		{
			*ints = new_ints;
			(*ints)[(*count) - 1] = x;
		}
	}
}

int sort(int* a, int n) 
{
	for (int i = 0; i < n - 1; i++) 
	{
		// ���������� ��� �������� ��������.
		for (int j = 0; j < n - i - 1; j++) 
		{
			if (a[j] > a[j + 1]) {
				// ���� ��� ���� � ������������ �������, ��  
				//  ������ �� �������. 
				int tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
	}
}