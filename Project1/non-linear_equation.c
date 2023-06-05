#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <malloc.h>
#include <memory.h>
#include <math.h> 
#include <stdarg.h>
#include "non_linear_equation.h"

void secant_method(const queue* rpn, variable** vars, int var_id, int vars_count, line_segment** solutions_ls, int count, double eps)
{
	//printf_s("\n--- Метод секущих ---\n");
	double x1, x2, f1, f2;
	double ls_length;
	int approximation_counter;
	for (int i = 0; i < count; ++i)
	{
		//print_segment(ls[i]);

		x1 = solutions_ls[i]->A;
		x2 = solutions_ls[i]->B;
		ls_length = fabs(solutions_ls[i]->B - solutions_ls[i]->A);

		approximation_counter = 1;
		while (fabs(x2 - x1) > eps)
		{

			vars[var_id]->value = x1;
			f1 = get_RPN_result(rpn, vars, vars_count);
			vars[var_id]->value = x2;
			f2 = get_RPN_result(rpn, vars, vars_count);

			if (f2 - f1 == 0) 
			{
				double xt = x2 - x1;
				printf_s("\t%d] [%.32lf, %.32lf] [%.32lf, %.32lf] [%.E %.32f]\n", approximation_counter, x1, x2, f1, f2, fabs(xt), fabs(xt));
				break;
			}
			else 
			{
				x1 = x1 - constant_p * (x2 - x1) * f1 / (f2 - f1);
				x2 = x2 - constant_p * (x1 - x2) * f2 / (f1 - f2);
			}

			//printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, x1, x2, fabs(x2 - x1));
			approximation_counter++;
			if (approximation_counter > ls_length * 10.0 / eps)
			{
				printf_s("Превышено количество итераций\n");
				break;
			}
		}

		if (x1 < x2) 
		{
			solutions_ls[i]->A = x1;
			solutions_ls[i]->B = x2;
		}
		else 
		{
			solutions_ls[i]->A = x2;
			solutions_ls[i]->B = x1;
		}
		//printf_s("\n");
	}
}


// Делим пополам и выбираем промежуток, где есть корень
void bisection_method(const queue* rpn, variable** vars, int var_id, int vars_count, line_segment** solutions_ls, int count, double eps)
{
	//printf_s("\n--- Метод бисекций ---\n");
	double fl, fc, fr, center, length;
	int approximation_counter;
	for (int i = 0; i < count; ++i) 
	{
		//print_segment(ls[i]);

		length = fabs(solutions_ls[i]->B - solutions_ls[i]->A);
		center = solutions_ls[i]->A + length / 2.0;


		vars[var_id]->value = center;
		fc = get_RPN_result(rpn, vars, vars_count);
		vars[var_id]->value = solutions_ls[i]->A;
		fl = get_RPN_result(rpn, vars, vars_count);
		vars[var_id]->value = solutions_ls[i]->B;
		fr = get_RPN_result(rpn, vars, vars_count);

		approximation_counter = 1;
		while (length > eps)
		{
			if (fl * fc < 0)
			{
				fr = fc;
				solutions_ls[i]->B = center;
			}
			else if (fc == 0)
			{
				//printf_s("\nНайдено решение в точке x = %.3f\n", center);
				//printf_s("Рекомендуется изменить число делений\n\n");
			}
			else 
			{
				fl = fc;
				solutions_ls[i]->A = center;
			}
			length = solutions_ls[i]->B - solutions_ls[i]->A;
			center = solutions_ls[i]->A + length / 2.0;

			vars[var_id]->value = center;
			fc = get_RPN_result(rpn, vars, vars_count);

			//printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, ls[i]->A, ls[i]->B, length);
			approximation_counter++;
		}
		//printf_s("\n");
	}
}

void newton_method (const queue* rpn, const queue* deriative_rpn, const queue* deriative2_rpn, variable** vars, int var_id, int vars_count, line_segment** solutions_ls, int count, double eps)
{
	//printf_s("\n---Метод Ньютона ---\n");
	double x, xp, fr, fdr, l;
	int approximation_counter;
	for (int i = 0; i < count; ++i)
	{
		//print_segment(ls[i]);

		l = fabs(solutions_ls[i]->B - solutions_ls[i]->A);

		x = solutions_ls[i]->A + l / 2; // Середина деления
		xp = solutions_ls[i]->B + l;

		approximation_counter = 1;
		while (fabs(xp - x) > eps)
		{
			vars[var_id]->value = x;
			fr = get_RPN_result(rpn, vars, vars_count);
			fdr = get_RPN_result(deriative2_rpn, vars, vars_count);

			xp = x;
			x = x - constant_p * fr / fdr;
			/*if (x < xp)
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, x, xp, fabs(xp - x));
			else
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, xp, x, fabs(xp - x));*/
			approximation_counter++;
		}

		if (x < xp)
		{
			solutions_ls[i]->A = x;
			solutions_ls[i]->B = xp;
		}
		else
		{
			solutions_ls[i]->A = xp;
			solutions_ls[i]->B = x;
		}

		//printf_s("\n");
	}
}

void modificated_newton_method (const queue* rpn, const queue* deriative_rpn, const queue* deriative2_rpn, variable** vars, int var_id, int vars_count, line_segment** solutions_ls, int count, double eps)
{
	//printf_s("\n--- Модифицированный метод Ньютона ---\n");
	double x, xp, fr, fdr, l;

	int approximation_counter;
	for (int i = 0; i < count; ++i)
	{
		//print_segment(ls[i]);

		l = fabs(solutions_ls[i]->B - solutions_ls[i]->A);
		x = solutions_ls[i]->A + l / 2; // Середина деления
		xp = solutions_ls[i]->B;

		vars[var_id]->value = x;
		fdr = get_RPN_result(deriative2_rpn, vars, vars_count);



		approximation_counter = 1;
		while (fabs(xp - x) > eps)
		{
			vars[var_id]->value = x;
			fr = get_RPN_result(rpn, vars, vars_count);

			xp = x;
			x = x - constant_p * fr / fdr;
			/*if (x < xp)
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, x, xp, fabs(xp - x));
			else
				printf_s("\n\t%d] [%.6f, %.6f] [%.10f]", approximation_counter, xp, x, fabs(xp - x));*/
			approximation_counter++;
		}

		if (x < xp)
		{
			solutions_ls[i]->A = x;
			solutions_ls[i]->B = xp;
		}
		else
		{
			solutions_ls[i]->A = xp;
			solutions_ls[i]->B = x;
		}

		//printf_s("\n");
	}
}


// отрезок [A,B], количество делений отрезка и необходимая точность |x* - x| < eps, функция, производные фнкции вида double deriative(double x)
double* non_linear_equation(int* solutions_count, double A, double B, int segments_count, double eps, finding_roots_method frm, 
	variable** vars, int var_id, int vars_count, const queue* rpn, ...)
{ 
	//printf_s("[%.3f, %.3f], segments_count = %d, eps = %.10f\n", A, B, segments_count, eps);

	line_segment** solutions_ls = (line_segment**)malloc(sizeof(line_segment*)),** ltmp;
	if (!solutions_ls) return NULL;

	*solutions_count = 0;

	double h = (B - A) / (double)segments_count;
	double a = A;
	double b = A + h;

	vars[var_id]->value = a;
	double f1 = get_RPN_result(rpn, vars, vars_count);

	vars[var_id]->value = b;
	double f2 = get_RPN_result(rpn, vars, vars_count);

	for (int i = 0; i < segments_count; ++i) 
	{
		// Проверка на смену знака
		if (f1 * f2 < 0) 
		{
			// Добавляем сегмент в решения
			*solutions_count += 1;
			ltmp = (line_segment**)realloc(solutions_ls, sizeof(line_segment*) * (*solutions_count));
			if (!ltmp) 
			{
				free(solutions_ls);
				//printf_s("Не удалось выделить память под отрезки");
				return NULL;
			}
			solutions_ls = ltmp;
			solutions_ls[(*solutions_count) - 1] = line_segment_init(a, b);
		} 
		/*else if (f1 * f2 == 0)
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
		}*/

		a = b;
		b += h;

		f1 = f2;
		vars[var_id]->value = b;
		f2 = get_RPN_result(rpn, vars, vars_count);
	}

	//printf_s("\nОтрезки, содержащие решение - %d\n", sc);
	//print_segments(solution_ls, sc);


	va_list deriatives_rpn_list;
	va_start(deriatives_rpn_list, rpn);

	switch (frm)
	{
	case secant:
		secant_method(rpn, vars, var_id, vars_count, solutions_ls, *solutions_count, eps);
		break;
	case bisection:
		bisection_method(rpn, vars, var_id, vars_count, solutions_ls, *solutions_count, eps);
		break;
	case newton:
		const queue* deriative_rpn_ = va_arg(deriatives_rpn_list, queue*);
		const queue* deriative_rpn2_ = va_arg(deriatives_rpn_list, queue*);

		newton_method(rpn, deriative_rpn_, deriative_rpn2_, vars, var_id, vars_count, solutions_ls, *solutions_count, eps);
		break;
	case modificated_newton:
		const queue* deriative_rpn = va_arg(deriatives_rpn_list, queue*);
		const queue* deriative_rpn2 = va_arg(deriatives_rpn_list, queue*);

		modificated_newton_method(rpn, deriative_rpn_, deriative_rpn2_, vars, var_id, vars_count, solutions_ls, *solutions_count, eps);
		break;
	}

	va_end(deriatives_rpn_list);
	

	double* solutions = (double*)malloc(sizeof(double) * (*solutions_count));
	for (int i = 0; i < *solutions_count; ++i)
	{
		solutions[i] = (solutions_ls[i]->B - solutions_ls[i]->A) / 2 + solutions_ls[i]->A;
		free(solutions_ls[i]);
	}
	free(solutions_ls);

	return solutions;
}