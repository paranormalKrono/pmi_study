#ifndef	_MATHFUNCTIONS_H_
#define	_MATHFUNCTIONS_H_
//#include <stdarg.h>
#include "MathPMI_Term1.h"
#define MATH_FUNCTIONS_COUNT 6

typedef struct math_function
{
	char* name;
	unsigned int parameters_count;
    void* fn;
} math_function;

float plus(float a, float b);
float minus(float a, float b);
float division(float a, float b);
float multiplication(float a, float b);

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
	{"modpow", 3, &modPow} 
};


void* get_function_result(math_function mf, void** parameters);

int get_mathfunction_index(char* name);

//int sum(int n, ...)
//{
//    int result = 0;
//    va_list factor;         //указатель va_list
//    va_start(factor, n);    // устанавливаем указатель
//    for (int i = 0; i < n; i++)
//    {
//        result += va_arg(factor, int);  // получаем значение текущего параметра типа int
//    }
//    va_end(factor); // завершаем обработку параметров
//    return result;
//}
#endif // _MATHFUNCTIONS_H_