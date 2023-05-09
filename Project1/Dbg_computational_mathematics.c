#include <math.h> 
#include <float.h>
#include "computational_mathematics.h"
#include "dbg_computational_mathematics.h"
#include "Menu_c.h"


void computational_mathematics_dbg()
{
	printf_s("__COMPUTATIONAL_MATHEMATICS_DEBUG__\n");

	const int text_count = 4;
	const char* Text[] = {
		"Нахождение производных таблично-заданной функции по формулам численного дифференцирования",
		"Задача обратного интерполирования",
		"Задача алгебраического интерполирования",
		"Решение трансцендентного или алгебраического уравнения вида f(x) = 0 на [A,B]"
	};
	const void (*methods[])() = {
		numerical_differentiation_dbg,
		reverse_algebraic_interpolation_dbg,
		algebraic_interpolation_dbg,
		nonlinear_equation_dbg
	};

	printf_s("Выберите, что нужно сделать: ");
	int choice = choice_menu_h(Text, text_count);
	methods[choice]();
}

void print_solutions(double* solutions, int count)
{
	for (int i = 1; i < count + 1; ++i)
	{
		printf_s("%4d] %20.E\n", i, solutions[i]);
	}
}

void print_function_pairs(line_segment** pairs, int count)
{
	for (int i = 0; i < count; ++i)
	{
		printf_s("%4d] %20lf %20lf\n", i, pairs[i]->A, pairs[i]->B);
	}
}

line_segment* input_segment()
{
	double A, B;
	do
	{
		printf_s("Введите левую и правую границу отрезка: ");
		scanf_s("%lf%lf", &A, &B);
	} while (B - A <= 0);
	return line_segment_init(A, B);
}

int input_pairs_count()
{
	int argument_values_count = 0;
	do
	{
		printf_s("Введите количество пар аргумент-значение функции: ");
		scanf_s("%d", &argument_values_count);
	} while (argument_values_count <= 1);
	return argument_values_count;
}

double input_epsilon() 
{
	double eps;
	do
	{
		eps = 0;
		printf_s("Введите точность поиска решения: ");
	} while (!scanf_s("%lf", &eps) && eps <= 0);
	return eps;
}