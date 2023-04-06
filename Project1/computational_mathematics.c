#include "computational_mathematics.h"
#include <malloc.h>
#include <memory.h>
#include <math.h> 

line_segment** get_function_pairs(line_segment* ls, int values_count, const double (*function)(double x))
{
	line_segment** values = malloc(sizeof(line_segment*) * values_count);
	double x = ls->A;
	double h = fabs(ls->B - ls->A) / (values_count - 1);
	for (int i = 0; i < values_count; ++i)
	{
		values[i] = line_segment_init(x, function(x));
		x += h;
	}
	return values;
}

line_segment** get_RPN_function_pairs(int variable_index, line_segment* segment, int values_count, const queue const* rpn, variable** variables)
{
	line_segment** values = malloc(sizeof(line_segment*) * values_count);

	double h = fabs(segment->B - segment->A) / (values_count - 1); 
	double tmp = variables[variable_index]->value;
	variables[variable_index]->value = segment->A;
	for (int i = 0; i < values_count; ++i)
	{
		values[i] = line_segment_init(variables[variable_index]->value, get_RPN_result(rpn, variables, 1));
		variables[variable_index]->value += h;
	}
	variables[variable_index]->value = tmp;
	return values;
}

line_segment** get_reverse_function_pairs(line_segment* ls, int values_count, const double (*function)(double x))
{
	line_segment** values = malloc(sizeof(line_segment*) * values_count);
	double x = ls->A;
	double h = fabs(ls->B - ls->A) / (values_count - 1);
	for (int i = 0; i < values_count; ++i)
	{
		values[i] = line_segment_init(function(x), x);
		x += h;
	}
	return values;
}

line_segment** reverse_function_pairs(line_segment** ls, int values_count)
{
	line_segment** values = malloc(sizeof(line_segment*) * values_count);
	for (int i = 0; i < values_count; ++i)
		values[i] = line_segment_init(ls[i]->B, ls[i]->A);
	return values;
}