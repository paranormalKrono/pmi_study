#include "numerical_integration.h"



double get_integration_compound_simpsons(const queue* function, variable** vars, int var_id, int vars_count, double A, double B, int divisions)
{
	double h, Q, W, f0, fm;

	vars[var_id]->value = A;
	f0 = get_RPN_result(function, vars, vars_count);
	vars[var_id]->value = B;
	fm = get_RPN_result(function, vars, vars_count);

	h = (B - A) / (double)divisions;

	vars[var_id]->value = A + h * 0.5;
	Q = get_RPN_result(function, vars, vars_count);

	W = 0;
	for (int i = 1; i < divisions; ++i)
	{
		vars[var_id]->value = A + h * ((double)i);
		W += get_RPN_result(function, vars, vars_count);
		vars[var_id]->value = A + h * ((double)i + 0.5);
		Q += get_RPN_result(function, vars, vars_count);
	}

	return integration_compound_simpsons(h, W, Q, f0, fm);
}



double get_integration_compound_middle_rectangle(const queue* function, variable** vars, int var_id, int vars_count, double A, double B, int divisions)
{
	double h = (B - A) / (double)divisions;

	double Q = 0;
	for (int i = 0; i < divisions; ++i)
	{
		vars[var_id]->value = A + h * ((double)i + 0.5);
		Q += get_RPN_result(function, vars, vars_count);
	}

	return integration_compound_middle_rectangle(h, Q);
}
