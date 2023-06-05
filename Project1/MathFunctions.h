#ifndef	_MATHFUNCTIONS_H_
#define	_MATHFUNCTIONS_H_
#include "MathPMI_Term1.h"
#include <math.h>
#include "Tokens.h"
#define MATH_FUNCTIONS_COUNT 15

typedef struct math_function
{
	token_type type;
	char* name;
	unsigned int parameters_count;
    double (*fn);
} math_function;

double minus(double a, double b);
double minus_prefix(double a);
double plus(double a, double b);
double division(double a, double b);
double multiplication(double a, double b);

static math_function math_fns[MATH_FUNCTIONS_COUNT] = {
	{Prefix_function, "-", 1, &minus_prefix},
	{Operator, "-", 2, &minus},
	{Operator, "+", 2, &plus},
	{Operator, "/", 2, &division},
	{Operator, "*", 2, &multiplication},
	{Prefix_function, "gcd", 2, &euclid},
	{Prefix_function, "modpow", 3, &modPow},
	{Prefix_function, "exp", 1, &exp},
	{Prefix_function, "pow", 2, &pow},
	{Prefix_function, "ln", 1, &log},
	{Prefix_function, "log", 1, &log},
	{Prefix_function, "sin", 1, &sin},
	{Prefix_function, "cos", 1, &cos},
	{Prefix_function, "abs", 1, &fabs},
	{Prefix_function, "sqrt", 1, &sqrt}
	//{"!", 1, &dfa_implement},
	//{"^", 2, &dfa_intersection},
	//{"|", 2, &dfa_union},
	//{"\\", 2, &dfa_difference},
};



double get_mathfunction_result(math_function mf, double* parameters);

int get_mathfunction_index(token* t);

#endif // _MATHFUNCTIONS_H_