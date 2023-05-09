#include "MathPMI_Debug.h"
#include "computational_mathematics.h"
#include "dbg_computational_mathematics.h"
#include <math.h> 
#include <float.h>

void function1_view()
{
	printf_s("Функция f(x) = "); // [0,2], m + 1 = 41, n = 12
	printf_s("exp(x) - x");
	printf_s("\n");
}

double calculate_function1_result(double x)
{
	return exp(x) - x;
}

double reverse_algebraic_interpolation_calculate_dbg(line_segment** nearest_pairs, double F, int degree, const double x, const double (*f)(double))
{
	double xL = Lagrange(nearest_pairs, F, degree);
	printf_s("Искомое по многочлену Лагранжа: PnL(F) = xL = %.E = %.16lf\n", xL, xL);
	double PnR = fabs(x - xL);
	printf_s("Абсолютная фактическая погрешность для формы Лагранжа: |x - xL| = %.E = %.16lf\n", PnR, PnR);

	double* NC = Newton_coeffs(nearest_pairs, degree);
	double xN = Newton(nearest_pairs, F, degree, NC);
	free(NC);
	
	printf_s("Искомое по многочлену Ньютона: PnN(F) = xN = %.E = %.16lf\n", xN, xN);

	PnR = fabs(x - xN);
	printf_s("Абсолютная фактическая погрешность для формы Ньютона: |x - xN| = %.E = %.16lf\n", PnR, PnR);

	PnR = fabs(xL - xN);
	printf_s("|xL - xN| = %.E = %.16lf\n\n", PnR, PnR);

	PnR = fabs(f(xN) - F);
	printf_s("|F - f(xN)| = %.E = %.16lf\n\n", PnR, PnR);

	PnR = fabs(f(xL) - F);
	printf_s("|F - f(xL)| = %.E = %.16lf\n\n", PnR, PnR);

	return xN;
}

double algebraic_interpolation_calculate_dbg(line_segment** nearest_pairs, double x, int degree, const double F)
{
	double PnL = Lagrange(nearest_pairs, x, degree);
	printf_s("Искомое по многочлену Лагранжа: PnL = %lf\n", PnL);
	double PnR1 = fabs(F - PnL);
	printf_s("Абсолютная фактическая погрешность для формы Лагранжа: |F - PnL(x)| = %.E = %.16lf\n", PnR1, PnR1);

	double* NC = Newton_coeffs(nearest_pairs, degree);
	double PnN = Newton(nearest_pairs, x, degree, NC);
	free(NC);
	double PnR2 = fabs(F - PnN);
	printf_s("Искомое по многочлену Ньютона: PnN = %lf\n", PnN);
	printf_s("Абсолютная фактическая погрешность для формы Ньютона: |F - PnN(x)| = %.E = %.16lf\n", PnR2, PnR2);

	double PnR3 = fabs(PnL - PnN);
	printf_s("|PnN(x) - PnL(x)| = %.E = %.16lf\n\n", PnR3, PnR3);

	printf_s("%20s\t %20s\t %20s\t %20s\t %20s\n",
		"PnL", "PnN",
		"|f(x) - PnL(x)|", "|f(x) - PnN(x)|",
		"|PnN(x) - PnL(x)|");
	printf_s("%20lf\t %20lf\t %20.E\t %20.E\t %20.E\n", PnL, PnN, PnR1, PnR2, PnR3);
	return PnL;
}

double input_interpolation_point(const line_segment* ls) 
{
	double x;
	do
	{
		x = DBL_MAX;
		printf_s("Введите точку интерполяции из отрезка [%lf, %lf]: ", ls->A, ls->B);
	} while (!scanf_s("%lf", &x) || !segment_check_inside(x, ls));
	return x;
}

int input_interpolation_polinom_degree(int argument_values_count) 
{
	int degree;
	do
	{
		printf_s("Введите степень интерполяционного многочлена не больше %d: ", argument_values_count - 1);
		scanf_s("%d", &degree);
	} while (degree > argument_values_count - 1 || degree < 0);
	return degree;
}


// P(x) - F = 0
void secant_equation(double* fk, double F, line_segment** nearest_pairs, int degree, line_segment* ls, double eps)
{
	//printf_s("\n--- Метод секущих ---\n");
	double x1, x2, f1, f2;
	int approximation_counter;
	x1 = ls->A;
	x2 = ls->B;

	double ls_length = fabs(ls->B - ls->A);
	approximation_counter = 1;
	while (fabs(x2 - x1) > eps)
	{
		f1 = Newton(nearest_pairs, x1, degree, fk) - F;
		f2 = Newton(nearest_pairs, x2, degree, fk) - F;
		x1 = x1 - constant_p * (x2 - x1) * f1 / (f2 - f1);
		x2 = x2 - constant_p * (x1 - x2) * f2 / (f1 - f2);

		approximation_counter++;
		if (approximation_counter > ls_length * 10 / eps) 
		{
			printf_s("Превышено количество итераций\n");
			break;
		}
	}

	if (x1 < x2)
	{
		ls->A = x1;
		ls->B = x2;
	}
	else
	{
		ls->A = x2;
		ls->B = x1;
	}
}


void reverse_algebraic_interpolation_dbg() 
{
	function1_view();

	printf_s("Вводите числа в виде 1 или 0,1\n");

	line_segment* ls = input_segment();
	if (!ls)
	{
		printf_s("Ошибка выделения памяти под отрезок");
	}
	int argument_values_count = input_pairs_count();
	printf_s("Тогда m = %d\n", argument_values_count - 1);

	line_segment** value_argument_pairs = get_reverse_function_pairs(ls, argument_values_count, calculate_function1_result);

	line_segment** nearest_reversed_pairs = malloc(sizeof(line_segment*)), ** nearest_pairs;
	double F, fx_prev = DBL_MAX;
	char answer;
	do
	{
		answer = '0';

		printf_s("\n\n----  ----\n");
		printf_s("f(x) | x\n");
		print_function_pairs(value_argument_pairs, argument_values_count);
		printf_s("\n");

		printf_s("Введите значение функции: ");
		scanf_s("%lF", &F);

		int degree = input_interpolation_polinom_degree(argument_values_count); // n
		printf_s("Тогда n = %d\n", degree);

		if (F != fx_prev)
		{
			fx_prev = F;
			if (nearest_reversed_pairs)
				free(nearest_reversed_pairs); // Очищаем только указатели

			nearest_reversed_pairs = collect_nearest_function_pairs(value_argument_pairs, argument_values_count, degree + 1, F);
			if (!nearest_reversed_pairs)
				printf_s("Ошибка выделения памяти под ближайшие пары\n");

		}

		printf_s("f(x) | x\n");
		print_function_pairs(nearest_reversed_pairs, degree + 1);
		printf_s("\n");

		// сунуть RPN в метод секущих

		int index = 1;
		// Ищем промежуток, в котором находится fx
		if (value_argument_pairs[1]->A - value_argument_pairs[0]->A > 0) 
			for (; value_argument_pairs[index]->A < F && index < argument_values_count; ++index);
		else
			for (; value_argument_pairs[index]->A > F && index < argument_values_count; ++index);

		line_segment* res_ls = line_segment_init(value_argument_pairs[index-1]->B, value_argument_pairs[index]->B);
		
		// Обращаем список
		nearest_pairs = reverse_function_pairs(nearest_reversed_pairs, degree + 1);

		double* NC = Newton_coeffs(nearest_pairs, degree);	// Находим коэффициенты в полиноме Ньютона для вида x f(x)

		secant_equation(NC, F, nearest_pairs, degree, res_ls, input_epsilon());	// Ищем решение методом секущих fx с помощью NC с degree на промежутке ls с точностью eps
		free(NC);	// Удаляем полином

		//printf_s("Итоговый промежуток: %20.15lf %20.15lf\n", ls->A, ls->B);
		double x = fabs(res_ls->B - res_ls->A) / 2 + res_ls->A;
		printf_s("Результат решения уравнения PnN(x) - F = 0 по методу секущих: x = %.E = %.16lf\n", x, x);

		double aproximate_result = calculate_function1_result(x);
		double value_residue = fabs(F - aproximate_result);
		printf_s("Проверка: F = %.E = %.16lf, f(x) = %.E = %.16lf, |F - f(x)| = %.E = %.16lf\n\n", F, F, aproximate_result, aproximate_result, value_residue, value_residue);

		double PnN = reverse_algebraic_interpolation_calculate_dbg(nearest_reversed_pairs, F, degree, x, calculate_function1_result);

		printf_s("%s", "Вы хотите выбрать другие f(x) = F и степень интерполяционного многочлена? Y/N (Да/Нет)\n");
		while (answer != 'Y' && answer != 'N')
			scanf_s("%c", &answer, 1);
	} while (answer == 'Y');

	free(nearest_pairs);
	free(nearest_reversed_pairs);
	free(value_argument_pairs);

}

void algebraic_interpolation_dbg()
{
	function1_view();

	printf_s("Вводите числа в виде 1 или 0,1\n");

	line_segment* ls = input_segment();
	int argument_values_count = input_pairs_count();
	printf_s("Тогда m = %d\n", argument_values_count - 1);

	line_segment** function_pairs = get_function_pairs(ls, argument_values_count, calculate_function1_result);

	line_segment** nearest_pairs = malloc(sizeof(line_segment*));
	double x, x_prev = 1;
	int degree; // n
	char answer;
	do
	{
		answer = '0';

		printf_s("\n\n----  ----\n");
		printf_s("x | f(x)\n");
		print_function_pairs(function_pairs, argument_values_count);
		printf_s("\n");

		x = input_interpolation_point(ls);
		degree = input_interpolation_polinom_degree(argument_values_count);
		printf_s("Тогда n = %d\n", degree);

		if (x != x_prev)
		{
			x_prev = x;
			if (nearest_pairs)
				free(nearest_pairs); // Очищаем только указатели

			nearest_pairs = collect_nearest_function_pairs(function_pairs, argument_values_count, degree + 1, x);
			if (!nearest_pairs)
				printf_s("Ошибка выделения памяти под ближайшие пары");

		}

		printf_s("x | f(x)\n");
		print_function_pairs(nearest_pairs, degree + 1);
		printf_s("\n");

		algebraic_interpolation_calculate_dbg(nearest_pairs, x, degree, calculate_function1_result(x));

		printf_s("%s", "Вы хотите выбрать другие x и степень интерполяционного многочлена? Y/N (Да/Нет)\n");
		while (answer != 'Y' && answer != 'N')
			scanf_s("%c", &answer, 1);
	} while (answer == 'Y');

	for (int i = 0; i < argument_values_count; ++i)
	{
		free(function_pairs[i]);
	}
}
