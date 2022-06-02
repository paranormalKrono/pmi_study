#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Shunting_yard.h"
#include "Tokens.h"
#include "MathFunctions.h"
#include "Variables_void.h"
#include "Main.h"

#define NUMBER_SIZE 10

int get_operation_priority(const char op);
char* parse_number(const char** cur_str);

queue* get_Reverse_Polish_Notation(char* str)
{
	char* cur_str_index = str, *st_start_index = str;
	char* s_temp;
	bool isSt = false;
	token* token_temp, ** token_void;

	queue* result_queue = queue_alloc();
	stack* tokens_stack = stack_alloc();

	while (*cur_str_index) 
	{
		switch (*cur_str_index)
		{
		case '(':
			if (isSt) isSt = false, stack_push(tokens_stack, token_init(parse_from_to(&st_start_index, cur_str_index), Function));
			stack_push(tokens_stack, token_init("(", LPar));
			break;

		case ')': case ',': // Выталкиваем все операции до ( или ,
			if (isSt) isSt = false, stack_push(tokens_stack, token_init(parse_from_to(&st_start_index, cur_str_index), Variable));
			token_temp = token_alloc();
			while (stack_pop(tokens_stack, &token_temp)
			&&(token_temp->type != LPar && token_temp->type != Comma))
			{
				queue_push(result_queue, token_temp);
				token_temp = token_alloc();
			}
			free(token_temp);
			if (*cur_str_index == ',') 	stack_push(tokens_stack, token_init(",", LPar));
			break;

		case '*': case '/': case '+': case '-': case '^': case '|': case '!': case '\\': // Выталкиваем операции большего приоритета и добавляем в стек текущую
			if (isSt) isSt = false, stack_push(tokens_stack, token_init(parse_from_to(&st_start_index, cur_str_index), Variable));
			while ((token_void = stack_get_front(tokens_stack), token_void)
				&& get_operation_priority((*token_void)->name[0]) >= get_operation_priority(*cur_str_index))
				if (token_temp = token_alloc(), stack_pop(tokens_stack, &token_temp))
				queue_push(result_queue, token_temp);
				else free(token_temp);

			s_temp = (char*)malloc(sizeof(char)*2);
			s_temp[0] = *cur_str_index;
			s_temp[1] = '\0';

			stack_push(tokens_stack, token_init(s_temp, Function));
			break;

		case '0': case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9': // Разбираем число
			if (isSt) isSt = false, stack_push(tokens_stack, token_init(parse_from_to(&st_start_index, cur_str_index), Variable));
			queue_push(result_queue, token_init(parse_number(&cur_str_index), Number));
			cur_str_index--;
			break;
		case ' ':
			break;
		default:
			if (!isSt) st_start_index = cur_str_index;
			isSt = true;
			break;
		}
		cur_str_index++;
	}
	if (isSt) isSt = false, stack_push(tokens_stack, token_init(parse_from_to(&st_start_index, cur_str_index), Variable));

	// Выталкиваем операции из стека
	while (stack_pop(tokens_stack, &token_temp))
		queue_push(result_queue, token_temp);

	stack_free(tokens_stack);
	return result_queue;
}

unsigned char* parse_number(const char** cur_str)
{
	unsigned char* result_number = (unsigned char*)malloc(sizeof(unsigned char) * NUMBER_SIZE); // массив цифр
	unsigned int text_length = 0; // длина текста

	// Посимвольная проверка
	while (strlen(*cur_str) > 0 && ('0' <= **cur_str && **cur_str <= '9'))
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


int get_vv_index(char* s, vvariable* variables, int count)
{
	for (int i = 0; i < count; ++i)
		if (strcmp(s, variables[i].name)) return i;
	return -1;
}

void* get_RPN_result(const queue const* rpn, const vvariable* vars, int count)
{
	queue* cur_rpn = queue_clone(rpn);
	stack* variables_stack = stack_alloc();
	token* cur_token, *n1 = token_alloc(), *n2 = token_alloc();
	vvariable* v;
	math_function mf;
	void* cur_res = malloc(sizeof(void*));
	int mf_index;
	void** parameters;

	while (queue_pop(cur_rpn, &cur_token) && cur_token) 
	{
		switch (cur_token->type)
		{
		case Number:
		{
			int num = atoi(cur_token->name);
			stack_push(variables_stack, vvariable_init(cur_token->name, &num));
			break;
		}

		case Variable:
			stack_push(variables_stack, vars[get_vv_index(cur_token->name, vars, count)].value);
			break;

		case Function:  // Получаем данные функции, забираем из стека нужное число параметров и 
						// отправляем результат неизвестной функции в стек
			mf_index = get_mathfunction_index(cur_token->name);
			if (mf_index != -1) 
			{
				mf = math_fns[mf_index];

				parameters = (void**)malloc(mf.parameters_count * sizeof(void*));
				for (int i = mf.parameters_count - 1; i >= 0; --i)
				{
					stack_pop(variables_stack, &v);
					parameters[i] = v;
				}
				cur_res = get_function_result(mf, parameters);
				free(parameters);

				stack_push(variables_stack, vvariable_init(NULL, cur_res));
			}
			break;
		default:
			stack_push(variables_stack, cur_token);
			break;
		}
	}

	stack_free(variables_stack);
	if (!cur_res) return NULL;
	return cur_res;
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