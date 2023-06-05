#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Shunting_yard.h"
#include "Tokens.h"
#include "MathFunctions.h"
#include "variables.h"
#include "Main.h"

#include <stdio.h>
#include <locale.h>

#define NUMBER_SIZE 32

int get_operation_priority(const char op);
char* parse_number(const char** cur_str);

void show_stack(stack* st)
{
	stack* clone = stack_clone(st);
	token* el = token_alloc();
	while (stack_pop(clone, &el))
	{
		printf_s("%s ", el->name);
	}
	printf_s("\n");
}

void show_queue(queue* q)
{
	queue* clone = queue_clone(q);
	token* el = token_alloc();
	while (queue_pop(clone, &el))
	{
		printf("%s ", el->name);
	}
	printf("\n");
}

queue* get_Reverse_Polish_Notation(const char* str)
{
	char* cur_str_index = str, *name_start_index = str;
	token* token_temp, ** token_void;
	token_type type_tmp;

	queue* rpn = queue_alloc();
	stack* tokens_stack = stack_alloc();

	while (*cur_str_index) 
	{
		//show_stack(tokens_stack);
		//show_queue(rpn);
		//printf_s("%s\n\n", cur_str_index);

		switch (*cur_str_index)
		{
		case '(':
			stack_push(tokens_stack, token_init("(", LPar));
			break;

		case ')':
			token_temp = token_alloc();

			// Выталкиваем из стека всё до открывающей скобки в rpn
			while (stack_pop(tokens_stack, &token_temp)
				&& (token_temp->type != LPar))
			{
				queue_push(rpn, token_temp);
				token_temp = token_alloc();
			}
			break;

		case ',':
			token_temp = token_alloc();

			// Выталкиваем из стека всё до открывающей скобки в rpn
			while (stack_pop(tokens_stack, &token_temp) 
				&& (token_temp->type != LPar))
			{
				queue_push(rpn, token_temp);
				token_temp = token_alloc();
			}
			// Возвращаем отрывающую скобку в стек
			stack_push(tokens_stack, token_temp);
			break;

		case '*': case '/': case '+': case '-': case '^': case '|': case '!': case '\\':

			// Создаём операцию для rpn
			char* operation_name = (char*)malloc(sizeof(char) * 2);
			operation_name[0] = *cur_str_index;
			operation_name[1] = '\0';
			type_tmp = Operator;

			// смотрим на предыдущий элемент и проверяем является ли операция унарной или бинарной
			if (token_void = queue_get_front(rpn), token_void)
			{
				if ((*(cur_str_index - 1)) == ',' || (*(cur_str_index - 1)) == '(')
					type_tmp = Prefix_function;
			}
			else type_tmp = Prefix_function;

			// Выталкиваем префиксные функции или операции большего или равного приоритета в rpn
			while ((token_void = stack_get_front(tokens_stack), token_void)
				&& ((*token_void)->type == Prefix_function
					|| (*token_void)->type == Operator && get_operation_priority((*token_void)->name[0]) >= get_operation_priority(*cur_str_index)))
				if (token_temp = token_alloc(), stack_pop(tokens_stack, &token_temp))
					queue_push(rpn, token_temp);

			stack_push(tokens_stack, token_init(operation_name, type_tmp));

			break;

		case '0': case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9': // Разбираем число
			unsigned char* num = parse_number(&cur_str_index); // Вынимаем числа в виде 10.001
			queue_push(rpn, token_init(num, Number)); // Происходит переход указателя к концу числа, будет работать для вещественных чисел
			cur_str_index--;
			break;
		case ' ':
			break;
		default:
			name_start_index = cur_str_index;
			while (*cur_str_index != ' ' && *cur_str_index != '*' && *cur_str_index != '/' && *cur_str_index != '+' && *cur_str_index != '-'
				&& *cur_str_index != '^' && *cur_str_index != '|' && *cur_str_index != '\\' && *cur_str_index != '!' && *cur_str_index != ','
				&& *cur_str_index != ')' && *cur_str_index != '(' && *cur_str_index != '\0') cur_str_index++;
			if (*cur_str_index == '(') stack_push(tokens_stack, token_init(parse_from_to(&name_start_index, cur_str_index), Prefix_function));
			else queue_push(rpn, token_init(parse_from_to(&name_start_index, cur_str_index), Variable));
			cur_str_index--;
			break;
		}
		cur_str_index++;
	}

	/*printf_s("stack: ");
	show_stack(tokens_stack);
	printf_s("queue: ");
	show_queue(rpn);
	printf_s("\n");*/

	// Выталкиваем операции из стека
	while (stack_pop(tokens_stack, &token_temp))
		queue_push(rpn, token_temp);

	printf_s("result: ");
	show_queue(rpn);
	printf_s("\n");

	stack_free(tokens_stack);


	return rpn;
}

unsigned char* parse_number(const char** cur_str)
{
	unsigned char* result_number = (unsigned char*)malloc(sizeof(unsigned char) * NUMBER_SIZE); // массив цифр
	unsigned int text_length = 0; // длина текста

	// Посимвольная проверка
	while (strlen(*cur_str) > 0 && (('0' <= **cur_str && **cur_str <= '9') || **cur_str == '.'))
	{
		result_number[text_length] = **cur_str;
		text_length++;
		if (!(text_length & NUMBER_SIZE))
		{
			result_number = (unsigned char*)realloc(result_number, (text_length + NUMBER_SIZE) * sizeof(unsigned char));
		}
		++* cur_str;
	}

	// Преобразование в строку
	result_number = (unsigned char*)realloc(result_number, (text_length + 1) * sizeof(unsigned char));
	result_number[text_length] = '\0';
	return result_number;
}


int get_vv_index(char* s, const variable** variables, int count)
{
	for (int i = 0; i < count; ++i)
		if (!strcmp(s, variables[i]->name))
		{
			return i;
		}
	return -1;
}

double get_RPN_result(const queue* rpn, variable** variables, int variables_count)
{
	queue* cur_rpn = queue_clone(rpn);
	stack* variables_stack = stack_alloc();
	token* cur_token;
	variable* cur_variable;
	math_function cur_function;
	int function_index;
	double res = 0, cur_res;
	double* parameters;

	_locale_t lt = _create_locale(LC_ALL, "en-US");
	while (queue_pop(cur_rpn, &cur_token) && cur_token) 
	{
		switch (cur_token->type)
		{
		case Number: 
			double num = _atof_l(cur_token->name, lt);
			stack_push(variables_stack, variable_init(cur_token->name, num));
			break;

		case Variable:
			int index = get_vv_index(cur_token->name, variables, variables_count);
			stack_push(variables_stack, variables[index]);
			//printf_s("\n%s = %lf\n", variables[index]->name, variables[index]->value);
			break;

		case Operator: case Function: case Prefix_function:  
			// Получаем данные функции, забираем из стека нужное число параметров и 
			// отправляем результат неизвестной функции в стек
			function_index = get_mathfunction_index(cur_token);
			if (function_index == -1) {
				//printf_s("%s - данной функции нет в программе, обратитесь к разработчику, желательно с пистолетом в руке\n", cur_token->name);
				break;
			}
			cur_function = math_fns[function_index];

			parameters = (double*)malloc(cur_function.parameters_count * sizeof(double));
			if (!parameters) break;

			for (int i = 0; i < (int)cur_function.parameters_count; ++i)
			{
				stack_pop(variables_stack, &cur_variable);
				parameters[cur_function.parameters_count - 1 - i] = cur_variable->value;
			}
			cur_res = get_mathfunction_result(cur_function, parameters);

			/*printf_s("%s(%.64lf", cur_function.name, parameters[0]);
			for (int i = 1; i < cur_function.parameters_count; ++i) printf_s(" %.64lf", parameters[i]);
			printf_s(") = %.64lf\n", cur_res);*/

			free(parameters);

			stack_push(variables_stack, variable_init(NULL, cur_res));
			break;

		default:
			stack_push(variables_stack, cur_token);
			break;
		}
	}
	res = 0;
	while (stack_pop(variables_stack, &cur_variable)) 
	{
		res += cur_variable->value;
	}

	stack_free(variables_stack);
	return res;
}

int get_operation_priority(const char op) 
{
	switch (op)
	{
	case '!':
		return 5;
	case '^':
		return 4;
	case '|': case '\\':
		return 3;
	case '*': case '/':
		return 2;
	case '-': case '+':
		return 1;
	default:
		return 0;
		break;
	}
}