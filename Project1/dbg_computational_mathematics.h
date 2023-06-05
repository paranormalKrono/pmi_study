#ifndef	_DBG_COMPUTATIONAL_MATHEMATICS_H_
#define	_DBG_COMPUTATIONAL_MATHEMATICS_H_
#include "MathPMI_Debug.h"

void function1_view();
double calculate_function1_result(double x); 

char* function3_view();

void print_solutions(double* solutions, int count);

void print_function_pairs(line_segment** pairs, int count);
line_segment* input_segment();
int input_pairs_count(); 
double input_epsilon();

void numerical_differentiation_dbg();
void reverse_algebraic_interpolation_dbg();
void algebraic_interpolation_dbg();
void computational_mathematics_dbg();

#endif // _DBG_COMPUTATIONAL_MATHEMATICS_H_