#ifndef	_NON_LINEAR_EQUATION_H_
#define	_NON_LINEAR_EQUATION_H_
#pragma once

#include "line_segment.h"
#include "Shunting_yard.h"

#define constant_p 1.0

typedef enum finding_roots_method
{
	secant,
	bisection,
	newton,
	modificated_newton
} finding_roots_method;


double* non_linear_equation(int* solutions_count, double A, double B, int segments_count, double eps, finding_roots_method frm,
	variable** vars, int var_id, int vars_count, const queue* rpn, ...);

#endif // _NON_LINEAR_EQUATION_H_