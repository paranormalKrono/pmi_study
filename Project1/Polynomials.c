#include "Polynomials.h"
#include <string.h>
#include <stdio.h>

poly* poly_parse_state0(char* cur_str_index);
poly* poly_get(const char* str) 
{
	poly* p = poly_zero();
	
	char* cur_str_index = str;
	while (*cur_str_index != '\0')
	{
		poly_addition(p, poly_parse_state0(&cur_str_index));
	}

	return p;
}

void poly_parse_state1(poly* p, char** cur_str_index);
poly* poly_parse_state0(char** cur_str_index)
{
	poly* p = poly_zero();
	switch (**cur_str_index)
	{
		case '-':
			++*cur_str_index;
			poly_parse_state1(p, cur_str_index);
			p->coeff *= -1;
			return p;
			break;
		case '+':
			++*cur_str_index;
			poly_parse_state1(p, cur_str_index);
			return p;
			break;
	default:
		poly_parse_state1(p, cur_str_index);
		return p;
		break;
	}
}

void poly_parse_state2(poly* p, char** cur_str_index);
void poly_parse_state1(poly* p, char** cur_str_index)
{
	switch (**cur_str_index)
	{
	case '0':
		if (p->coeff != 0)
		{
			p->coeff *= 10;
		}
		++*cur_str_index;
		poly_parse_state1(p, cur_str_index);
		break;

	case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9':
		p->coeff = p->coeff * 10 + (**cur_str_index - '0');
		++* cur_str_index;
		poly_parse_state1(p, cur_str_index);
		break;
	case 'x':
		if (p->coeff == 0) 
		{
			p->coeff = 1;
		}
		if (*(++*cur_str_index) == '^') 
		{
			++*cur_str_index;
			poly_parse_state2(p, cur_str_index);
		}
		break;
	default:
		break;
	}
}

void poly_parse_state2(poly* p, char** cur_str_index)
{
	switch (**cur_str_index)
	{
	case '0':
		if (p->exp != 0)
		{
			p->exp *= 10;
		}
		++* cur_str_index;
		poly_parse_state2(p, cur_str_index);
		break;
	case '1': case '2': case '3': case  '4': case  '5': case  '6': case  '7': case '8': case  '9':
		p->exp = p->exp * 10 + (**cur_str_index - '0');
		++* cur_str_index;
		poly_parse_state2(p, cur_str_index);
		break;
	default:
		break;
	}
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
	printf(poly_tostring(p));
}

char* poly_tostring(const poly* p) 
{
	char* result;
	if (p->coeff != 0) 
	{
		result = (char*)malloc();
		snprintf(result, sizeof(char)* strlen(result), "%dx^%d", p->coeff, p->exp);
	}
	else 
	{
		result = "0";
	}
	if (p->next != NULL)
	{
		if (p->next->coeff > 0) 
		{
			result += '+';
		}
		snprintf(result, sizeof(char) * 30, "%s%s", result, poly_tostring(p->next));
		return result;
	}
	return result;
}

void poly_addition(poly* left, const poly* right)
{
	if (right == NULL) // Всё прибавили
	{
		return;
	}
	if (left->exp == right->exp) // Складываем одночлены
	{
		left->coeff += right->coeff;
		poly_addition(left->next, right->next);
	}
	else if (left->next == NULL)
	{
		left->next = poly_zero();
		left->next->exp = right->exp;
		poly_addition(left->next, right);
	}
	else if (left->next->exp <= right->exp)
	{
		poly_addition(left->next, right);
	}
	else
	{
		// Если правый можно поставить между левым и следующим
		poly* l_next = left->next;
		left->next = poly_zero();
		left->next->exp = right->exp;
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