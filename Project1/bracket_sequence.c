#include "Small_exercises.h"
#include <limits.h>
#include <stdio.h>
#include "Lessons/sq/stack_list.h"
#include <stdbool.h>


void show_bracket_sequence()
{
	int brackets_count = 3;
	char* opening_brackets = "([{";
	char* closing_brackets = ")]}";

	char* str = "(((([][]{}{})}))))";

	printf_s("Дана такая строка: %s\n", str);

	char* cur_str_index = str;
	char cur_bracket;
	char* stack_bracket = '0';
	bool is_success = true;

	stack* brackets_stack = stack_alloc();

	int id = 0;

	while (*cur_str_index)
	{
		cur_bracket = *cur_str_index;
		for (int i = 0; i < brackets_count; ++i)
			if (cur_bracket == opening_brackets[i])
			{
				stack_push(brackets_stack, &(opening_brackets[i]));
			}
			else if (cur_bracket == closing_brackets[i])
			{
				stack_pop(brackets_stack, &stack_bracket);
				for (int j = 0; j < brackets_count; ++j)
					if (*stack_bracket == opening_brackets[j])
					{
						id = j;
						break;
					}
				if (i != id)
				{
					is_success = false;
					break;
				}
			}

		if (!is_success)
			break;
		else
			cur_str_index++;
	}

	stack_free(brackets_stack);

	printf_s("Верна ли скобочная последовательность? ");

	if (is_success)
	{
		printf_s("Да");
	}
	else
	{
		printf_s("Нет");
	}
}