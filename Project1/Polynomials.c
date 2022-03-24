#include "Polynomials.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum parse_states
{
	stateS,
	stateX,
	stateD,
	stateE
} parse_states;

poly* poly_parse(const char* str, char** errorp) // +Exception
{
	char* cur_str_index = str;
	*errorp = NULL;
	poly* p = poly_monomial(0, 0);
	parse_states cur_state = stateS;
	bool isNegative = false;
	int c = 0, e = 0;

	while (*cur_str_index != '\0' || c) 
	{
		switch (cur_state)
		{
		case stateS:
			if (c)
			{
				if (isNegative) 
				{
					isNegative = false;
					c = -c;
				}
				poly_addition(&p, poly_monomial(c, e));
				c = e = 0;
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
				*errorp = cur_str_index;
				return p;
			}
			break;
		case stateX:
			if (c == 0) c = 1;
			e = 1;
			++cur_str_index;
			if (*cur_str_index == '^')
				cur_state = stateE;
			else
			{
				cur_state = stateS;
			}
			break;
		case stateE:
			++cur_str_index;
			cur_state = stateD;
			break;
		case stateD:
			if (c > 0) 
				e = strtol(cur_str_index, &cur_str_index, 10);
			else
				c = strtol(cur_str_index, &cur_str_index, 10);

			if (*cur_str_index == 'x')
				cur_state = stateX;
			else
				cur_state = stateS;
			break;
		default:
			++cur_str_index;
			break;
		}
	}
	return p;
}

poly* poly_monomial(int coeff, int exp) 
{
	poly* p = (poly*)malloc(sizeof(poly));
	if (p) 
	{
		p->coeff = coeff;
		p->exp = exp;
		p->next = NULL;
	}
	return p;
}

void poly_show(const poly* p) 
{
	printf("%s\n", poly_tostring(p));
}

char* poly_tostring(const poly* p) 
{
	char* current, *result = NULL, *new_result;
	poly* cur_poly = p;
	while (cur_poly)
	{
		current = NULL;
		if (cur_poly->coeff)
		{
			if (cur_poly->exp == 0)
			{
				current = (char*)malloc(sizeof(char) * _MAX_U64TOSTR_BASE10_COUNT);
				snprintf(current, strlen(current), "%d", cur_poly->coeff);
			}
			else if (cur_poly->exp == 1)
			{
				if (cur_poly->coeff == 1)
					current = "x";
				else
				{
					current = (char*)malloc(sizeof(char) * (_MAX_U64TOSTR_BASE10_COUNT + 1));
					snprintf(current, strlen(current), "%dx", cur_poly->coeff);
				}
			}
			else
			{
				current = (char*)malloc(sizeof(char) * (_MAX_U64TOSTR_BASE10_COUNT + 3));
				snprintf(current, strlen(current), "%dx^%d", cur_poly->coeff, cur_poly->exp);
			}
		}
		else
			current = "";

		if (result && result != "")
		{
			new_result = (char*)malloc(sizeof(char) * (strlen(result) + strlen(current) + 3));

			if (cur_poly->coeff > 0)
				if (current == "0")
					snprintf(new_result, strlen(new_result), "%s", current);
				else
					snprintf(new_result, strlen(new_result), "%s+%s", result, current);
			else
				snprintf(new_result, strlen(new_result), "%s%s", result, current);
			result = new_result;
			new_result = NULL;
		}
		else
			result = current;

		cur_poly = cur_poly->next;
	}

	if (!result || result == "")
		result = "0";
	return result;

}

/// <summary>
/// Poly in exp-increasing order
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void poly_addition(const poly** left, const poly* right)
{
	if (*left == NULL) 
	{
		*left = poly_monomial(0, 0);
	}
	poly* left_cur = *left, *right_cur = right, *left_prev = NULL;

	while (right_cur)
	{
		if (left_cur == NULL) // Копируем элементы из right в конец left 
		{
			while (right_cur)
			{
				left_prev->next = left_cur = poly_monomial(right_cur->coeff, right_cur->exp);
				right_cur = right_cur->next;
			}
			break;
		}
		else if (left_cur->exp == right_cur->exp)
		{
			left_cur->coeff += right_cur->coeff;
			if (!left_cur->coeff) 
			{
				if (left_prev) // Соединяем левый моном с правым и очищаем текущий
				{
					left_prev->next = left_cur->next;
					left_cur->next = NULL;
					poly_free(left_cur);
					left_cur = left_prev->next;
				}
				else // Меняем начало полинома
				{
					*left = (*left)->next;
					left_cur->next = NULL;
					poly_free(left_cur);
					left_cur = *left;
				}
			}
			else // Если коеффициенты не противоположны, складываем одночлены
			{
				left_prev = left_cur;
				left_cur = left_cur->next;
			}
			right_cur = right_cur->next;
		}
		else if (left_cur->exp < right_cur->exp) // Переходим к следующим мономам
		{
			left_prev = left_cur;
			left_cur = left_cur->next;
		}
		else // Вставляем моном слева
		{
			if (left_prev) // Вставляем между левым и текущим
			{
				left_prev->next = poly_monomial(right_cur->coeff, right_cur->exp);
				left_prev->next->next = left_cur;
				left_prev = left_prev->next;
			}
			else // Меняем начало полинома
			{
				left_prev = poly_monomial(right_cur->coeff, right_cur->exp);
				*left = left_prev;
				left_prev->next = left_cur;
			}
			right_cur = right_cur->next;
		}
	}
}

poly* poly_multiplication(const poly* left, const poly* right) 
{
	poly* left_cur = left, *right_cur = right, * r_polys = NULL;
	poly *result = poly_monomial(0, 0), *addition = poly_monomial(0, 0);
	int right_polys_count = 0;

	while (right_cur)
	{
		++right_polys_count;
		r_polys = (poly*)realloc(r_polys, sizeof(poly) * (right_polys_count));
		r_polys[right_polys_count - 1] = *right_cur;
		right_cur = right_cur->next;
	}

	while (left_cur)
	{
		for (int r = 0; r < right_polys_count; ++r)
		{
			addition->coeff = left_cur->coeff * r_polys[r].coeff;
			if (addition->coeff) 
			{
				addition->exp = left_cur->exp + r_polys[r].exp;
				poly_addition(&result, addition);
			}
		}
		left_cur = left_cur->next;
	}

	free(addition);
	free(r_polys);
	return result;
}

void poly_free(poly* p)
{
	poly* temp;
	while (p)
	{
		temp = p;
		p = p->next;
		free(temp);
	}
}