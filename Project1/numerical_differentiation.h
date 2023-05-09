#ifndef	_NUMERICAL_DIFFERENTIATION_H_
#define	_NUMERICAL_DIFFERENTIATION_H_
#include "line_segment.h"
#include "Shunting_yard.h"
#include "variables.h"

#define deriative_left(h, cur, left, left2) ((3 * cur - 4 * left + left2) / (2 * h))
#define deriative_right(h, cur, right, right2) ((- 3 * cur + 4 * right - right2)/(2*h))
#define deriative_lr(h, left, right) ((right - left) / (2 * h))
#define deriative_second_lr(h, cur, left, right) ((right - 2 * cur + left) / (h * h))

double* get_deriative_list(double h, line_segment** function_pairs, const int argument_values_count);
double* get_second_deriative_list(double h, line_segment** function_pairs, const int argument_values_count);

double* get_deriative_list_without_pairs(const double h, const int argument_values_count, const queue* rpn, variable** variables, int variables_count);
double* get_second_deriative_list_without_pairs(const double h, const int argument_values_count, const queue* rpn, variable** variables, int variables_count);

#endif // _NUMERICAL_DIFFERENTIATION_H_