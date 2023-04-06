#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Shunting_yard.h"
#include "Tokens.h"
#include "MathFunctions.h"
#include "variables.h"
#include "Main.h"

#include <stdio.h>

#define NUMBER_SIZE 25

int get_operation_priority(const char op);
char* parse_number(const char** cur_str);

void show_stack(stack* q)
{
	stack* c = stack_clone(q);
	token* e = token_alloc();
	while (stack_pop(c, &e))
	{
		printf_s("%s ", e->name);
	}
	printf_s("\n");
}

void show_queue(queue* q)
{
	queue* c = queue_clone(q);
	token* e = token_alloc();
	while (queue_pop(c, &e))
	{
		printf("%s ", e->name);
	}
	printf("\n");
}

queue* get_Reverse_Polish_Notation(char* str)
{
	char* cur_str_index = str, *name_start_index = str;
	bool isName = false;
	token* token_temp, ** token_void;

	queue* result_queue = queue_alloc();
	stack* tokens_stack = stack_alloc();

	while (*cur_str_index) 
	{
		/*printf_s("stack: ");
		show_stack(tokens_stack);
		printf_s("queue: ");
		show_queue(result_queue);
		printf_s("\n");*/

		switch (*cur_str_index)
		{
		case '(':
			if (isName) isName = false, stack_push(tokens_stack, token_init(parse_from_to(&name_start_index, cur_str_index), Function));
			stack_push(tokens_stack, token_init("(", LPar));
			break;

		case ')':
			if (isName) isName = false, queue_push(result_queue, token_init(parse_from_to(&name_start_index, cur_str_index), Variable));
			token_temp = token_alloc();
			while (stack_pop(tokens_stack, &token_temp)
			&&(token_temp->type != LPar))
			{
				queue_push(result_queue, token_temp);
				token_temp = token_alloc();
			}
			if (stack_pop(tokens_stack, &token_temp))
				if (token_temp->type == Function) queue_push(result_queue, token_temp);
				else stack_push(tokens_stack, token_temp);
			else free(token_temp);
			//if (*cur_str_index == ',') 	stack_push(tokens_stack, token_init(",", LPar));
			break;

		case ',':
			if (isName) isName = false, queue_push(result_queue, token_init(parse_from_to(&name_start_index, cur_str_index), Variable));
			token_temp = token_alloc();
			while (stack_pop(tokens_stack, &token_temp)
				&& (token_temp->type != LPar))
			{
				queue_push(result_queue, token_temp);
				token_temp = token_alloc();
			}
			free(token_temp);
			break;

		case '*': case '/': case '+': case '-': case '^': case '|': case '!': case '\\': // Выталкиваем операции большего приоритета и добавляем в стек текущую
			if (isName) isName = false, queue_push(result_queue, token_init(parse_from_to(&name_start_index, cur_str_index), Variable));
			while ((token_void = stack_get_front(tokens_stack), token_void)
				&& get_operation_priority((*token_void)->name[0]) >= get_operation_priority(*cur_str_index))
				if (token_temp = token_alloc(), stack_pop(tokens_stack, &token_temp))
				queue_push(result_queue, token_temp);
				else free(token_temp);

			char* s_temp = (char*)malloc(sizeof(char)*2);
			s_temp[0] = *cur_str_index;
			s_temp[1] = '\0';

			stack_push(tokens_stack, token_init(s_temp, Function));
			break;

		case '0': case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9': // Разбираем число
			if (isName) isName = false, stack_push(tokens_stack, token_init(parse_from_to(&name_start_index, cur_str_index), Variable));
			unsigned char* num = parse_number(&cur_str_index); // Вынимаем числа в виде 10.001
			queue_push(result_queue, token_init(num, Number)); // Происходит переход указателя к концу числа, будет работать для вещественных чисел
			cur_str_index--;
			break;
		case ' ':
			break;
		default:
			if (!isName) name_start_index = cur_str_index;
			isName = true;
			break;
		}
		cur_str_index++;
	}
	if (isName) isName = false, stack_push(tokens_stack, token_init(parse_from_to(&name_start_index, cur_str_index), Variable));

	/*printf_s("stack: ");
	show_stack(tokens_stack);
	printf_s("queue: ");
	show_queue(result_queue);
	printf_s("\n");*/

	// Выталкиваем операции из стека
	while (stack_pop(tokens_stack, &token_temp))
		queue_push(result_queue, token_temp);

	//printf_s("result: ");
	//show_queue(result_queue);
	//printf_s("\n");

	stack_free(tokens_stack);


	return result_queue;
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

double get_RPN_result(const queue* rpn, const variable** variables, int variables_count)
{
	queue* cur_rpn = queue_clone(rpn);
	stack* variables_stack = stack_alloc();
	token* cur_token, *n1 = token_alloc(), *n2 = token_alloc();
	variable* cur_variable;
	math_function cur_function;
	int function_index;
	double res = 0, cur_res;
	double* parameters;
	while (queue_pop(cur_rpn, &cur_token) && cur_token) 
	{
		switch (cur_token->type)
		{
		case Number: 
			double num = atof(cur_token->name);
			stack_push(variables_stack, variable_init(cur_token->name, num));
			break;

		case Variable:
			int index = get_vv_index(cur_token->name, variables, variables_count);
			stack_push(variables_stack, variables[index]);
			//printf_s("\n%s = %lf\n", variables[index]->name, variables[index]->value);
			break;

		case Function:  // Получаем данные функции, забираем из стека нужное число параметров и 
			// отправляем результат неизвестной функции в стек
			function_index = get_mathfunction_index(cur_token->name);
			if (function_index == -1) break;
			cur_function = math_fns[function_index];

			parameters = (double*)malloc(cur_function.parameters_count * sizeof(double));
			if (!parameters) break;

			for (int i = 0; i < cur_function.parameters_count; ++i)
			{
				stack_pop(variables_stack, &cur_variable);
				parameters[cur_function.parameters_count - 1 - i] = cur_variable->value;
			}
			cur_res = get_mathfunction_result(cur_function, parameters);

			//printf_s("%s(%lf", cur_function.name, parameters[0]);
			//for (int i = 1; i < cur_function.parameters_count; ++i) printf_s(", %lf", parameters[i]);
			//printf_s(") = %lf\n", cur_res);

			free(parameters);

			stack_push(variables_stack, variable_init(NULL, cur_res));
			res = cur_res;
			break;

		default:
			stack_push(variables_stack, cur_token);
			break;
		}
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