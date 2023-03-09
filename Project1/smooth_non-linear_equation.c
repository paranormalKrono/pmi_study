#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h> 
#include <stdarg.h>
#include "computational_mathematics.h"

void secant_method(const double (*function)(double x), line_segment** ls, int count, double eps)
{
	printf_s("\n--- Метод секущих ---\n");
	double x1, x2, f1, f2;
	int approximation_counter;
	for (int i = 0; i < count; ++i)
	{
		print_segment(ls[i]);

		x1 = ls[i]->A;
		x2 = ls[i]->B;

		approximation_counter = 1;
		while (fabs(x2 - x1) > eps)
		{
			f1 = function(x1);
			f2 = function(x2);
			x1 = x1 - constant_p * (x2 - x1) * f1 / (f2 - f1);
			x2 = x2 - constant_p * (x1 - x2) * f2 / (f1 - f2);
			printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, x1, x2, fabs(x2 - x1));
			approximation_counter++;
		}

		if (x1 < x2) 
		{
			ls[i]->A = x1;
			ls[i]->B = x2;
		}
		else 
		{
			ls[i]->A = x2;
			ls[i]->B = x1;
		}
		printf_s("\n");
	}
}

// Делим пополам и выбираем промежуток, где есть корень
void bisection_method(const double (*function)(double x), line_segment** ls, int count, double eps)
{
	printf_s("\n--- Метод бисекций ---\n");
	double fl, fc, fr, center, length;
	int approximation_counter;
	for (int i = 0; i < count; ++i) 
	{
		print_segment(ls[i]);

		length = fabs(ls[i]->B - ls[i]->A);
		center = ls[i]->A + length / 2;
		fc = function(center);
		fl = function(ls[i]->A);
		fr = function(ls[i]->B);

		approximation_counter = 1;
		while (length > eps)
		{
			if (fl * fc < 0)
			{
				fr = fc;
				ls[i]->B = center;
			}
			else if (fc == 0)
			{
				printf_s("\nНайдено решение в точке x = %.3f\n", center);
				printf_s("Рекомендуется изменить число делений\n\n");
			}
			else 
			{
				fl = fc;
				ls[i]->A = center;
			}
			length = ls[i]->B - ls[i]->A;
			center = ls[i]->A + length / 2;
			fc = function(center);

			printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, ls[i]->A, ls[i]->B, length);
			approximation_counter++;
		}
		printf_s("\n");
	}
}



void newton_method(const double (*function)(double x), const double (*function_deriative)(double x), const double (*function_deriative2)(double x), line_segment** ls, int count, double eps)
{
	printf_s("\n---Метод Ньютона ---\n");
	double x, xp, fr, fdr, l;
	int approximation_counter;
	for (int i = 0; i < count; ++i)
	{
		print_segment(ls[i]);

		l = fabs(ls[i]->B - ls[i]->A);
		x = ls[i]->A + l / 2; // Середина деления
		xp = ls[i]->B + l;

		approximation_counter = 1;
		while (fabs(xp - x) > eps)
		{
			fr = function(x);
			fdr = function_deriative(x);
			xp = x;
			x = x - constant_p * fr / fdr;
			if (x < xp)
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, x, xp, fabs(xp - x));
			else
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, xp, x, fabs(xp - x));
			approximation_counter++;
		}

		if (x < xp)
		{
			ls[i]->A = x;
			ls[i]->B = xp;
		}
		else
		{
			ls[i]->A = xp;
			ls[i]->B = x;
		}

		printf_s("\n");
	}
}

void modificated_newton_method(const double (*function)(double x), const double (*function_deriative)(double x), const double (*function_deriative2)(double x), line_segment** ls, int count, double eps)
{
	printf_s("\n--- Модифицированный метод Ньютона ---\n");
	double x, xp, fr, fdr, l;

	int approximation_counter;
	for (int i = 0; i < count; ++i)
	{
		print_segment(ls[i]);

		l = fabs(ls[i]->B - ls[i]->A);
		x = ls[i]->A + l / 2; // Середина деления
		xp = ls[i]->B;
		fdr = function_deriative(x);



		approximation_counter = 1;
		while (fabs(xp - x) > eps)
		{
			fr = function(x);
			xp = x;
			x = x - constant_p * fr / fdr;
			if (x < xp)
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, x, xp, fabs(xp - x));
			else
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, xp, x, fabs(xp - x));
			approximation_counter++;
		}

		if (x < xp)
		{
			ls[i]->A = x;
			ls[i]->B = xp;
		}
		else
		{
			ls[i]->A = xp;
			ls[i]->B = x;
		}

		printf_s("\n");
	}
}

//// Если достаточное условие сходимости метода Ньютона не выполнено
//if (fdr == 0 || fr * function_deriative2(x) <= 0)
//{
//	x = ls[i]->A + l / 101 * (rand() % 101); // один из концов 101 деления
//	fdr = function_deriative(x);
//	fr = function(x);
//	printf_s("\nНе выполнено условие сходимости, x изменён\n");
//}



// отрезок [A,B], количество делений отрезка и необходимая точность |x* - x| < eps, функция, производные фнкции вида double deriative(double x)
line_segment** non_linear_equation(int* solutions_count, double A, double B, int segments_count, double eps, finding_roots_method frm, const double (*function)(double x), ...)
{ 
	printf_s("[%.3f, %.3f], segments_count = %d, eps = %.10f\n", A, B, segments_count, eps);

	line_segment** solution_ls = (line_segment**)malloc(sizeof(line_segment*)),** ltmp;
	if (!solution_ls) return NULL;

	int sc = 0;

	double h = (B - A) / (double)segments_count;
	double a = A;
	double b = A + h;

	double f1 = function(a), f2 = function(b);

	for (int i = 0; i < segments_count; ++i) 
	{
		// Проверка на смену знака
		if (f1 * f2 < 0) 
		{
			// Добавляем сегмент в решения
			sc += 1;
			ltmp = (line_segment**)realloc(solution_ls, sizeof(line_segment*) * sc);
			if (!ltmp) 
			{
				free(solution_ls);
				printf_s("Не удалось выделить память под отрезки");
				return NULL;
			}
			solution_ls = ltmp;
			solution_ls[sc - 1] = line_segment_init(a, b);
		} 
		else if (f1 * f2 == 0)
		{
			if (f1 == 0)
			{
				printf_s("Найдено решение в точке x = %.3f\n", a);
				if (f2 == 0) 
				{
					printf_s("Найдено решение в точке x = %.3f\n", b);
				}
			}
			else printf_s("Найдено решение в точке x = %.3f\n", b);
			printf_s("Рекомендуется изменить число делений\n");
		}

		a = b;
		b += h;

		f1 = f2;
		f2 = function(b);
	}

	//printf_s("\nОтрезки, содержащие решение - %d\n", sc);
	//print_segments(solution_ls, sc);


	va_list deriatives;
	va_start(deriatives, function);

	switch (frm)
	{
	case secant:
		secant_method(function, solution_ls, sc, eps);
		break;
	case bisection:
		bisection_method(function, solution_ls, sc, eps);
		break;
	case newton:
		const double (*function_deriative_) (double x) = va_arg(deriatives, void*);
		const double(*function_deriative2_) (double x) = va_arg(deriatives, void*);
		newton_method(function, function_deriative_, function_deriative2_, solution_ls, sc, eps);
		break;
	case modificated_newton:
		const double (*function_deriative) (double x) = va_arg(deriatives, void*);
		const double(*function_deriative2) (double x) = va_arg(deriatives, void*);
		modificated_newton_method(function, function_deriative, function_deriative2, solution_ls, sc, eps);
		break;
	}

	va_end(deriatives);
	
	*solutions_count = sc;
	return solution_ls;
}