#include "Graphs_algorithms.h"
#include <string.h>
#include <stdlib.h>  
#include <stdio.h>
#include "Main.h"

typedef enum parse_states
{
	stateS,
	stateV
} parse_states;

// «аписывает ответ как значени€ variables
void sat2_check(const graph* g, variable** variables, const int variables_count)
{
	int* condensation = Condensation_graph_transposition(g);
	if (!condensation) return NULL;

	for (int i = 0; i < g->count / 2; ++i) // провер€ем существование ответа
		if (condensation[i] == condensation[i + g->count / 2]) return NULL;

	print_ints("%3d", condensation, g->count);
	printf("\n");

	for (int i = 0; i < variables_count; ++i) // сравниваем компоненты и записываем ответ
		variables[i]->value = condensation[i] > condensation[i + g->count / 2] ? 1 : 0;
}

int get_index(const variable* v, const variable** variables, const int count);
void add_if_not_contains(variable* v, variable*** variables, int* count);

graph* sat2_graph(char* sat2, variable*** variables, int* variables_count)
{
	*variables_count = 0;

	char* cur_str_index = sat2, * cur_variable_index = sat2; // ”казатели на текущую позицию в строке и начало переменной
	stack* bool_variables = stack_alloc(); // состоит из булевских переменных с сохранением отрицани€

	variable* v;
	int is_negative = 0;

	parse_states cur_state = stateS;
	while (*cur_str_index != '\0') // (x\/y)/\(~y\/~z)/\(z\/~x)/\(x\/x)
	{
		switch (cur_state)
		{
		case stateS:
			switch (*cur_str_index)
			{
			case '(':
				++cur_str_index;
				cur_variable_index = cur_str_index; // ”становили указатель на начало названи€ переменной
				cur_state = stateV;
				break;
			default:
				++cur_str_index;
				break;
			}
			break;
		case stateV:
			switch (*cur_str_index)
			{
			case '~':
				is_negative = 1; // «апомнили отрицание
			case '/':
				++cur_str_index;
				cur_variable_index = cur_str_index; // ”становили указатель на начало названи€ переменной
				break;

			case '\\': // ѕри переходе ко второй переменной считываем первую переменную, мен€€ отрицание на противоположное
				is_negative = !is_negative;
			case ')': // —читываем вторую или первую переменную
				v = variable_init(parse_from_to(&cur_variable_index, cur_str_index), is_negative);
				add_if_not_contains(v, variables, variables_count);
				stack_push(bool_variables, v);
				is_negative = 0;
				if (*cur_str_index == ')') cur_state = stateS;
				++cur_str_index;
				break;

			default:
				++cur_str_index;
				break;
			}
			break;

		default:
			cur_state = stateS;
			break;
		}
	}

	graph* g = graph_init(*variables_count * 2); // —оздаЄм граф с вдвое большим числом вершин
	int i1, i2;
	while (stack_pop(bool_variables, &v) && v) // ¬ытаскиваем из стека 2 переменные с отрицанием, находим их индекс и добавл€ем 2 дуги
	{
		i1 = get_index(v, *variables, *variables_count);
		if (v->value) i1 += *variables_count;
		if (stack_pop(bool_variables, &v) && v)
		{
			i2 = get_index(v, *variables, *variables_count);
			if (v->value) i2 += *variables_count;
			if (i1 != -1 && i2 != -1)
			{
				add_arc(g, i2, i1);										// !x -> y
				i1 += i1 >= *variables_count ? -*variables_count : *variables_count;
				i2 += i2 >= *variables_count ? -*variables_count : *variables_count;
				add_arc(g, i1, i2);										// !y -> x
			}
		}
	}

	stack_free(bool_variables);
	return g;
}

// ¬озвращает номер вхождени€ элемента или -1 если нет
int get_index(const variable* v, const variable** variables, const int count)
{
	for (int i = 0; i < count; ++i)
		if (variable_compare_name(variables[i], v)) return i;
	return -1;
}

void add_if_not_contains(variable* v, variable*** variables, int* count)
{
	if (count)
	{
		for (int i = 0; i < *count; ++i)
			if (variable_compare_name((*variables)[i], v) == 1) return;
		variable** new_v = (variable**)realloc(*variables, sizeof(variable*) * (++ * count));
		if (new_v)
		{
			*variables = new_v;
			(*variables)[(*count) - 1] = v;
		}
	}
}