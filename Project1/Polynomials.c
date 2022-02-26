#include "Polynomials.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

poly* poly_parse(const char* str);
poly* poly_get(const char* str)
{
	return poly_parse(str);
}

typedef enum parse_states
{
	stateS,
	stateX,
	stateD,
	stateE
} parse_states;

poly* poly_parse(const char* str)
{
	char* cur_str_index = str;
	poly* p = poly_zero();
	parse_states cur_state = stateS;
	bool isNegative = false;
	bool is_cur_poly_parsed = false;

	poly* cur_p = poly_zero();

	while (*cur_str_index != '\0' || is_cur_poly_parsed) 
	{
		switch (cur_state)
		{
		case stateS:
			if (is_cur_poly_parsed)
			{
				if (isNegative) 
				{
					isNegative = false;
					cur_p->coeff = -cur_p->coeff;
				}
				poly_addition(p, cur_p);
				is_cur_poly_parsed = false;
				cur_p = poly_zero();
			}
			switch (*cur_str_index)
			{
			case '-':
				isNegative = !isNegative;
				++cur_str_index;
				break;
			case '+':
				++cur_str_index;
				break;
			case 'x':
				cur_state = stateX;
				break;
			case '0': case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9':
				cur_state = stateD;
				break;
			case '\0':
				continue;
			default:
				++cur_str_index;
				break;
			}
			break;
		case stateX:
			if (cur_p->coeff == 0) cur_p->coeff = 1;
			cur_p->exp = 1;
			++cur_str_index;
			if (*cur_str_index == '^')
				cur_state = stateE;
			else
			{
				cur_state = stateS;
				is_cur_poly_parsed = true;
			}
			break;
		case stateE:
			++cur_str_index;
			cur_state = stateD;
			break;
		case stateD:
			if (cur_p->coeff > 0) 
				cur_p->exp = strtol(cur_str_index, &cur_str_index, 10);
			else
				cur_p->coeff = strtol(cur_str_index, &cur_str_index, 10);

			if (*cur_str_index == 'x')
				cur_state = stateX;
			else
			{
				cur_state = stateS;
				is_cur_poly_parsed = true;
			}
				break;
		default:
			++cur_str_index;
			break;
		}
	}
	poly_free(cur_p);
	return p;
}

poly* poly_zero() 
{
	poly* p = (poly*)malloc(sizeof(poly));
	p->coeff = 0;
	p->exp = 0;
	p->next = NULL;
	return p;
}

void poly_show(const poly* p) 
{
	printf("%s\n", poly_tostring(p));
}

char* poly_tostring(const poly* p) 
{
	char* current;
	if (p->coeff)
	{
		if (p->exp == 0)
		{
			current = (char*)malloc(sizeof(char) * 17);
			snprintf(current, sizeof(char) * strlen(current), "%d", p->coeff);
		}
		else if (p->exp == 1)
		{
			if (p->coeff == 1) 
			{
				current = "x";
			}
			else
			{
				current = (char*)malloc(sizeof(char) * 18);
				snprintf(current, sizeof(char) * strlen(current), "%dx", p->coeff);
			}
		}
		else 
		{
			current = (char*)malloc(sizeof(char) * 37);
			snprintf(current, sizeof(char) * strlen(current), "%dx^%d", p->coeff, p->exp);
		}
	}
	else
	{
		current = "";
	}

	if (p->next)
	{
		char* next = poly_tostring(p->next);
		char* result = (char*)malloc(sizeof(char) * (strlen(current) + strlen(next) + 1));
		if (p->next->coeff > 0)
			if (current == "0")
				snprintf(result, strlen(result), "%s", next);
			else
				snprintf(result, strlen(result), "%s+%s", current, next);
		else
			snprintf(result, strlen(result), "%s%s", current, next);
		return result;
	}

	if (current == "") 
		current = "0";
	return current;

}

void poly_addition(poly* left, const poly* right)
{
	if (right == NULL) // Всё прибавили
		return;

	if (left->exp == right->exp) // Складываем одночлены и идём дальше
	{
		left->coeff += right->coeff;
		poly_addition(left->next, right->next);
	}
	else if (left->next == NULL) // Вставляем right 
	{
		left->next = poly_zero();
		left->next->exp = right->exp;
		poly_addition(left->next, right);
	}
	else if (left->next->exp < right->exp) // Идём дальше
		poly_addition(left->next, right);
	else if (left->next->exp == right->exp) // Смотрим коеффициенты
	{
		if (left->next->coeff == -right->coeff) // Если они противоположные
		{
			if (left->next->next == NULL)
			{
				poly_free(left->next);
				left->next = NULL;
			}
			else
			{
				poly* l_next = left->next->next;
				left->next->next = NULL;
				poly_free(left->next);
				left->next = l_next;
			}
			poly_addition(left->next, right->next);
		}
		else
			poly_addition(left->next, right);
	}
	else
	{
		// Если правый можно поставить между левым и следующим
		poly* l_next = left->next;
		left->next = poly_zero();
		left->next->exp = right->exp;
		left->next->coeff = right->coeff;
		left->next->next = l_next;
		poly_addition(left->next, right->next);
	}
}

poly* poly_multiplication(poly* left, const poly* right) 
{
	return left;
}

void poly_free(poly* p)
{
	if (p == NULL)
	{
		return;
	}
	if (p->next != NULL)
	{
		poly_free(p->next);
	}
	free(p);
}