#ifndef	_MATHFUNCTIONS_H_
#define	_MATHFUNCTIONS_H_
#include "MathPMI_Term1.h"
#include <math.h>
#define MATH_FUNCTIONS_COUNT 8

typedef struct math_function
{
	char* name;
	unsigned int parameters_count;
    double (*fn);
} math_function;

double minus(double a, double b);
double plus(double a, double b);
double division(double a, double b);
double multiplication(double a, double b);

static math_function math_fns[MATH_FUNCTIONS_COUNT] = {
	{"-", 2, &minus},
	{"+", 2, &plus},
	{"/", 2, &division},
	{"*", 2, &multiplication},
	//{"!", 1, &dfa_implement},
	//{"^", 2, &dfa_intersection},
	//{"|", 2, &dfa_union},
	//{"\\", 2, &dfa_difference},
	{"gcd", 2, &euclid},
	{"modpow", 3, &modPow},
	{"exp", 1, &exp},
	{"pow", 2, &pow}
};


double get_mathfunction_result(math_function mf, double* parameters);

int get_mathfunction_index(char* name);

#endif // _MATHFUNCTIONS_H_