#include "Shunting_yard.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define FUNCTION_NAME_MAX_LENGTH 5



char* get_Reverse_Polish_Notation(char* str)
{
	char* cur_str_index = str;
	char* result;

	while (*cur_str_index) 
	{
		switch (*cur_str_index)
		{
		case '(':

			break;
		case ')':

			break;
		case '*':

			break;
		case '/':

			break;
		case '-':

			break;
		case '+':

			break;
		default:
			break;
		}
	}
}

int get_RPN_result(char* rpn) 
{

}