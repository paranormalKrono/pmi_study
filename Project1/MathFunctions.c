#include "MathFunctions.h"
#include <string.h>

int get_mathfunction_index(char* name)
{
	for (int i = 0; i < MATH_FUNCTIONS_COUNT; ++i)
	{
		if (strcmp(name, math_fns[i].name) == 0) return i;
	}
	return -1;
}

double get_mathfunction_result(math_function mf, double* parameters)
{
	double (*mff)(double, ...) = mf.fn;
	switch (mf.parameters_count)
	{
	case 1:
		return mff(parameters[0]);
	case 2:
		return mff(parameters[0], parameters[1]);
	case 3:
		return mff(parameters[0], parameters[1], parameters[2]);
	case 4:
		return mff(parameters[0], parameters[1], parameters[3], parameters[4]);
	default:
		break;
	}
}

double plus(double a, double b)
{
	return a + b;
}
double minus(double a, double b)
{
	return a - b;
}
double division(double a, double b)
{
	return a / b;
}
double multiplication(double a, double b)
{
	return a * b;
}