#ifndef	_ALGEBRAIC_INTERPOLATION_H_
#define	_ALGEBRAIC_INTERPOLATION_H_
#include "line_segment.h"

line_segment** collect_nearest_function_pairs(line_segment** function_pairs, int values_count, int nearest_count, double x);
line_segment** get_reverse_function_pairs(line_segment* ls, int values_count, const double (*function)(double x));
line_segment** get_function_pairs(line_segment* ls, int values_count, const double (*function)(double x));
int find_nearest_pair_index(line_segment** function_pairs, int values_count, int nearest_count, double x);

double Lagrange(line_segment** nearest_pairs, double x, int degree);
double* Newton_coeffs(line_segment** nearest_pairs, int degree);
double Newton(line_segment** nearest_pairs, double x, int degree, double* fk);

#endif // _ALGEBRAIC_INTERPOLATION_H_