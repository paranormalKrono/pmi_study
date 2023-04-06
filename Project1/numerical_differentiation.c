#include "numerical_differentiation.h"
#include "Shunting_yard.h"
#include "variables.h"
#include <malloc.h>
#include "line_segment.h"



#define deriative_left(h, cur, left, left2) ((3 * cur - 4 * left + left2) / (2 * h))
#define deriative_right(h, cur, right, right2) ((- 3 * cur + 4 * right - right2)/(2*h))
#define deriative_lr(h, left, right) ((right - left) / (2 * h))
#define deriative_second_lr(h, cur, left, right) ((right - 2 * cur + left) / (h * h))

// ”честь, что все пары значений равноотсто€т друг от друга
double* get_second_deriative_list(double h, line_segment** function_pairs, const int pairs_count)
{
	if (pairs_count < 3) return NULL;
	double* second_deriatives = malloc(sizeof(double) * pairs_count);
	if (!second_deriatives) return;

	double prev, cur, next;
	prev = function_pairs[0]->B;
	cur = function_pairs[1]->B;
	next = function_pairs[2]->B;

	second_deriatives[0] = 0;

	for (int i = 1; i < pairs_count - 2; ++i)
	{
		second_deriatives[i] = deriative_second_lr(h, cur, prev, next);
		prev = cur;
		cur = next;
		next = function_pairs[i + 2]->B;
	}
	second_deriatives[pairs_count - 2] = deriative_second_lr(h, cur, prev, next);

	second_deriatives[pairs_count - 1] = 0;

	return second_deriatives;
}

// ”честь, что все пары значений равноотсто€т друг от друга
double* get_deriative_list(double h, line_segment** function_pairs, const int pairs_count)
{
	if (pairs_count < 3) return NULL;

	double* first_deriatives = malloc(sizeof(double) * pairs_count);
	if (!first_deriatives) return;
	double prev, cur, next;

	prev = function_pairs[0]->B;
	cur = function_pairs[1]->B;
	next = function_pairs[2]->B;

	first_deriatives[0] = deriative_right(h, prev, cur, next);
	// 5 узлов
	for (int i = 1; i < pairs_count - 2; ++i) // 1...3
	{
		first_deriatives[i] = deriative_lr(h, prev, next);

		prev = cur;
		cur = next;
		next = function_pairs[i + 2]->B;
	}
	first_deriatives[pairs_count - 2] = deriative_lr(h, prev, next);

	first_deriatives[pairs_count - 1] = deriative_left(h, next, cur, prev); // 3 * f(x5) - 4 * f(x4) + f(x3)

	return first_deriatives;
}


void add_to_variables(const double h, variable** variables, int variables_count)
{
	for (int i = 0; i < variables_count; ++i) variables[i]->value += h;
}

// ”честь, что variables наход€тс€ в x0
double* get_second_deriative_list_without_pairs(const double h, const int pairs_count, const queue* rpn, variable** variables, int variables_count) 
{
	if (pairs_count < 3) return NULL;

	double* second_deriatives = malloc(sizeof(double) * pairs_count);
	if (!second_deriatives) return;

	double prev, cur, next;
	prev = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(h, variables, variables_count);
	cur = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(h, variables, variables_count);
	next = get_RPN_result(rpn, variables, variables_count);

	second_deriatives[0] = 0;
	for (int i = 1; i < pairs_count - 2; ++i) 
	{
		second_deriatives[i] = deriative_second_lr(h, cur, prev, next);
		prev = cur;
		cur = next;
		add_to_variables(h, variables, variables_count);
		next = get_RPN_result(rpn, variables, variables_count);

	}
	second_deriatives[pairs_count - 2] = deriative_second_lr(h, cur, prev, next);

	second_deriatives[pairs_count - 1] = 0;
	add_to_variables(- pairs_count * h, variables, variables_count);

	return second_deriatives;
}

// ”честь, что variables наход€тс€ в x0
double* get_deriative_list_without_pairs(const double h, const int pairs_count, const queue* rpn, variable** variables, int variables_count)
{
	if (pairs_count < 3) return NULL;

	double* first_deriatives = malloc(sizeof(double) * pairs_count);
	if (!first_deriatives) return;
	double prev, cur, next;

	prev = get_RPN_result(rpn, variables, variables_count); // x0
	add_to_variables(h, variables, variables_count);
	cur = get_RPN_result(rpn, variables, variables_count); // x1
	add_to_variables(h, variables, variables_count);
	next = get_RPN_result(rpn, variables, variables_count); // x2

	first_deriatives[0] = deriative_right(h, prev, cur, next);
	// 5 узлов
	for (int i = 1; i < pairs_count - 2; ++i) // 1...2
	{
		first_deriatives[i] = deriative_lr(h, prev, next);

		prev = cur;
		cur = next;
		add_to_variables(h, variables, variables_count);
		next = get_RPN_result(rpn, variables, variables_count); // x3...x5
	}
	first_deriatives[pairs_count - 2] = deriative_lr(h, prev, next);

	first_deriatives[pairs_count - 1] = deriative_left(h, next, cur, prev); // 3 * f(x5) - 4 * f(x4) + f(x3) 

	add_to_variables(-pairs_count * h, variables, variables_count); // ¬озвращаем в начальное состо€ние

	return first_deriatives;
}

double get_second_deriative_by_lr(const double h, const queue* rpn, variable** variables, int variables_count)
{
	double cur = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(-h, variables, variables_count);
	double left = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(2 * h, variables, variables_count);
	double right = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(-h, variables, variables_count);

	return deriative_second_lr(h,cur, left, right);
}

double get_deriative_by_lr(const double h, const queue* rpn, variable** variables, int variables_count)
{
	add_to_variables(-h, variables, variables_count);
	double left = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(2*h, variables, variables_count);
	double right = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(-h, variables, variables_count);

	return deriative_lr(h, left, right);
}

double get_deriative_by_right(const double h, const queue* rpn, variable** variables, int variables_count)
{
	double cur =  get_RPN_result(rpn, variables, variables_count);
	add_to_variables(h, variables, variables_count);
	double right = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(h, variables, variables_count);
	double right2 = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(-2 * h, variables, variables_count);

	return deriative_right(h, cur, right, right2);
}

double get_deriative_by_left(const double h, const queue* rpn, variable** variables, int variables_count)
{
	double cur = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(-h, variables, variables_count);
	double left = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(-h, variables, variables_count);
	double left2 = get_RPN_result(rpn, variables, variables_count);
	add_to_variables(2 * h, variables, variables_count);

	return deriative_left(h, cur, left, left2);
}