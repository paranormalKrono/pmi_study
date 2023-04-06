#ifndef	_COMPUTATIONAL_MATHEMATICS_H_
#define	_COMPUTATIONAL_MATHEMATICS_H_
#include "line_segment.h"
#include "non_linear_equation.h"
#include "algebraic_interpolation.h"
#include "numerical_differentiation.h"
#include "Shunting_yard.h"

line_segment** get_function_pairs(line_segment* ls, int values_count, const double (*function)(double x));
line_segment** get_RPN_function_pairs(int coordinate_index, line_segment* segment, int values_count, const queue const* rpn, variable** variables);

line_segment** get_reverse_function_pairs(line_segment* ls, int values_count, const double (*function)(double x));

line_segment** reverse_function_pairs(line_segment** ls, int values_count);


#endif // _COMPUTATIONAL_MATHEMATICS_H_