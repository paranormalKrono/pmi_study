#ifndef	_MATHFUNCTIONS_H_
#define	_MATHFUNCTIONS_H_
//#include <stdarg.h>
#include "MathPMI_Term1.h"
#define FUNCTIONS_COUNT 1

typedef struct math_function
{
	char* name;
	unsigned int parameters_count;
    void* func;
} math_function;

static math_function math_functs[FUNCTIONS_COUNT] = { "gcd", 2, &euclid };

int get_function_result(math_function mf, int* parameters);

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