#include "DFA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lessons/sq/stack_list.h"

char* get_binary(const int x);

// Автомат, проверяющий числа с предшествующими нулями {0}*{number}
dfa* dfa_int(int number)
{
	char* number_r2 = get_binary(number);

	dfa* d = dfa_init(strlen(number_r2) + 2); // Создаём DFA по дугам, учитывая штопор
	char* cur_digit = number_r2;

	// check another (d->count) leading zeros of number
	// Дополнительные дуги для обработки предшествующих нулей
	dfa_add_arc(d, DFA_START_VERTEX, DFA_START_VERTEX, 0);
	dfa_add_arcs(d, DFA_START_VERTEX, DFA_START_VERTEX + 1, (int)*(cur_digit++) - '0');
	int i = DFA_START_VERTEX + 1;
	for (; *cur_digit != '\0'; ++i, ++cur_digit)
	{
		dfa_add_arcs(d, i, i + 1, (int)*cur_digit - '0'); // i -> i + 1 по текущей цифре 0 или 1 и в штопор
	}
	dfa_add_arcs(d, i, 0, 0); // 

	d->adj_states[i].state = state_end;
	
	return d;
}

int dfa_check_int(dfa* d, const int n) 
{
	char* number_r2 = get_binary(n);
	int result = dfa_check(d, number_r2);
	free(number_r2);
	return result;
}

// проверит 101, но не 0101
int dfa_check(dfa* d, const char* str) 
{
	char* cur_str = str;

	dnode* cur_node = d->adj_states[d->init_state].head;
	int head_vertex = 0;
	int value; // двоичный бит числа
	while (cur_node)
	{
		value = *(cur_str++) - '0';
		if (value != cur_node->value) // Проверяем совпадает ли значение дуги со значением разряда
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

void str_add_if_not_contain(char* x, char*** strs, int* count);
int sort_binary_strs(char** a, int n);

void dfa_print_range2(dfa* d, int number)
{
	for (int i = 0; i < number; ++i) 
	{
		if (dfa_check_int(d, i)) 
		{
			printf_s("%d\n", i);
		}
	}
}

// Вывести все строки, которые может распознать автомат от 0 до number
void dfa_print_range(dfa* d, int number)
{
	char* number_r2 = get_binary(number);
	const int len = strlen(number_r2) + 1;
	free(number_r2);

	dnode* cur_node = d->adj_states[d->init_state].head;
	int res = 0, res_count = 0;
	
	char* res_str;
	char** results = (char**)malloc(sizeof(char*));
	char* res_binary = (char*)malloc(sizeof(char)*len);
	char* cur_bin_pos = res_binary;

	stack* path = stack_alloc(); // Обратный путь
	while (cur_node) 
	{
		if (cur_node->value + (res << 1) <= number && cur_bin_pos - res_binary < len) // если результат меньше предельного, проходим по ребру, иначе меняем ребро
		{
			*cur_bin_pos = (char)(cur_node->value + '0');
			++cur_bin_pos;
			res = cur_node->value + (res << 1); // прибавляем стоимость ребра и переходим
			if (d->adj_states[cur_node->vertex].state == state_end)
			{
				*cur_bin_pos = '\0';
				res_str = (char*)malloc((cur_bin_pos - res_binary + 1) * sizeof(char));
				memcpy_s(res_str, (cur_bin_pos - res_binary + 1) * sizeof(char), res_binary, (cur_bin_pos - res_binary + 1) * sizeof(char));
				str_add_if_not_contain(res_str, &results, &res_count);
			}

			stack_push(path, cur_node); // добавляем текущую в путь
			cur_node = d->adj_states[cur_node->vertex].head; // переходим к первой связанной вершине
		}
		else
		{
			while (!cur_node->next && (res >>= 1, --cur_bin_pos, stack_pop(path, &cur_node))); // Возвращаемся по вершинам пути
			cur_node = cur_node->next;
		}
	}

	sort_binary_strs(results, res_count);
	int prev_res = -1;
	for (int i = 0; i < res_count && results[i]; ++i)
	{
		res = strtol(results[i], NULL, 2);
		if (prev_res != res)
		{
				printf_s("\n   %-10d = %10s", res, results[i]);
			prev_res = res;
		}
		else
		{
			printf_s(" %10s", results[i]);
		}
	}
	printf_s("\n");
	free(results);
}

char* get_binary(const int x)
{
	char* x_r2 = (char*)malloc(sizeof(char) * _MAX_U64TOSTR_BASE2_COUNT);
	if (!x_r2) return NULL;
	_itoa_s(x, x_r2, _MAX_U64TOSTR_BASE2_COUNT, 2); // Переводим число в двоичную систему
	return x_r2;
}


void str_add_if_not_contain(char* x, char*** strs, int* count)
{
	if (count)
	{
		for (int i = 0; i < *count; ++i)
			if (strcmp((*strs)[i], x) == 0) return;
		char** new_strs = (char**)realloc(*strs, sizeof(char*) * (++ * count));
		if (new_strs)
		{
			*strs = new_strs;
			(*strs)[(*count) - 1] = x;
		}
	}
}

int sort_binary_strs(char** a, int n) 
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (strcmp(a[j], a[j + 1]) < 0)
			{
				int tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
	}
	for (int i = 0; i < n - 1; i++) 
	{
		for (int j = 0; j < n - i - 1; j++) 
		{
			if (strtol(a[j], NULL, 2) > strtol(a[j + 1], NULL, 2)) 
			{
				int tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
	}
}