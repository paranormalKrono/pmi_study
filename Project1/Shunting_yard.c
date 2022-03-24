#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Shunting_yard.h"
#include "Tokens.h"
#include "MathFunctions.h"
#define NUMBER_SIZE 10

int get_operation_priority(const char op);
char* parse_number(const char** cur_str);
char* parse_function(const char** cur_function, const char* cur_str);

queue* get_Reverse_Polish_Notation(char* str)
{
	char* cur_str_index = str, *cur_ft_start_index = str;
	char* s;
	bool isFt = false;
	token* e;
	token** v;

	queue* result_queue = queue_alloc();
	stack* tokens_stack = stack_alloc();

	while (*cur_str_index) 
	{
		switch (*cur_str_index)
		{
		case '(':
			if (isFt) isFt = false, stack_push(tokens_stack, token_init(parse_function(&cur_ft_start_index, cur_str_index), Function));
			stack_push(tokens_stack, token_init("(", LPar));
			break;

		case ')': case ',': // Выталкиваем все операции до ( или ,
			if (isFt) isFt = false, stack_push(tokens_stack, token_init(parse_function(&cur_ft_start_index, cur_str_index), Function));
			while (e = token_alloc(), stack_pop(tokens_stack, &e)
				&& (e->type != LPar && e->type != Comma))
				queue_push(result_queue, e);
			break;

		case '*': case '/': case '+': case '-': // Выталкиваем операции большего приоритета и добавляем в стек текущую
			if (isFt) isFt = false, stack_push(tokens_stack, token_init(parse_function(&cur_ft_start_index, cur_str_index), Function));
			while ((v = stack_get_front(tokens_stack), v)
				&& get_operation_priority((*v)->name[0]) >= get_operation_priority(*cur_str_index)
				&& (e = token_alloc(), stack_pop(tokens_stack, &e)))
						queue_push(result_queue, e);
			s = (char*)malloc(sizeof(char)*2);
			s[0] = *cur_str_index;
			s[1] = '\0';
			stack_push(tokens_stack, token_init(s, Operator));
			break;

		case '0': case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9': // Разбираем число
			if (isFt) isFt = false, stack_push(tokens_stack, token_init(parse_function(&cur_ft_start_index, cur_str_index), Function));
			queue_push(result_queue, token_init(parse_number(&cur_str_index), Number));
			cur_str_index--;
			break;
		case ' ':
			break;
		default:
			if (!isFt) cur_ft_start_index = cur_str_index;
			isFt = true;
			break;
		}
		cur_str_index++;
	}

	// Выталкиваем операции из стека
	while (stack_pop(tokens_stack, &e))
		queue_push(result_queue, e);

	stack_free(tokens_stack);
	return result_queue;
}

char* parse_function(const char** cur_function, const char* cur_str) 
{
	char *ft = (char*)malloc((cur_str - *cur_function + 1) * sizeof(char));
	int i = 0;
	for (; *cur_function != cur_str; ++i, ++*cur_function) ft[i] = **cur_function;
	ft[i] = '\0';

	return ft;
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


int get_RPN_result(const queue* rpn)
{
	queue* cur_rpn = queue_clone(rpn);
	stack* numbers_stack = stack_alloc();
	token* cur_token, *tok, *n1 = token_alloc(), *n2 = token_alloc();
	char* buffer;
	math_function mf;
	int temp_result = 0, mf_index;
	int* parameters;

	while (queue_pop(cur_rpn, &cur_token) && cur_token) 
	{
		switch (cur_token->type)
		{
		case Operator:
			stack_pop(numbers_stack, &n1);
			stack_pop(numbers_stack, &n2);
			temp_result = atoi(n2->name);
			switch (cur_token->name[0])
			{
			case '-':
				temp_result -= atoi(n1->name); break;
			case '+':
				temp_result += atoi(n1->name); break;
			case '*':
				temp_result *= atoi(n1->name); break;
			case '/':
				temp_result /= atoi(n1->name); break;
			default:
				break;
			}
			tok = token_alloc();
			tok->type = Number;
			
			buffer = (char*)malloc(sizeof(char) * _MAX_U64TOSTR_BASE10_COUNT);
			_itoa_s(temp_result, buffer, _MAX_U64TOSTR_BASE10_COUNT, 10);
			tok->name = buffer;
			stack_push(numbers_stack, tok);
			break;

		case Function:
			mf_index = get_mathfunction_index(cur_token->name);
			if (mf_index != -1) 
			{
				mf = math_functs[mf_index];

				parameters = (int*)malloc(mf.parameters_count * sizeof(int));
				for (int i = 0; i < mf.parameters_count; ++i) 
				{
					stack_pop(numbers_stack, &n1);
					parameters[i] = atoi(n1->name);
				}
				temp_result = get_function_result(mf, parameters);
				free(parameters);

				tok = token_alloc();
				tok->type = Number;
				buffer = (char*)malloc(sizeof(char) * _MAX_U64TOSTR_BASE10_COUNT);
				_itoa_s(temp_result, buffer, _MAX_U64TOSTR_BASE10_COUNT, 10);
				tok->name = buffer;
				stack_push(numbers_stack, tok);
			}
			break;
		default:
			stack_push(numbers_stack, cur_token);
			break;
		}
	}

	stack_free(numbers_stack);
	token_free(n1);
	token_free(n2);
	return temp_result;
}

int get_operation_priority(const char op) 
{
	switch (op)
	{
	case '*': case '/':
		return 2;
	case '-': case '+':
		return 1;
	default:
		return 0;
		break;
	}
}