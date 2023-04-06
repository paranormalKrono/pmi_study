#ifndef	_NUMERICAL_DIFFERENTIATION_H_
#define	_NUMERICAL_DIFFERENTIATION_H_
#include "line_segment.h"

double* get_deriative_list(double h, line_segment** function_pairs, const int pairs_count);
double* get_second_deriative_list(double h, line_segment** function_pairs, const int pairs_count);


#endif // _NUMERICAL_DIFFERENTIATION_H_