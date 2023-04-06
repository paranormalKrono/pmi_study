#include "computational_mathematics.h"
#include <stdio.h>
#include <math.h> 
#include "Time_Debug.h"
#include "dbg_computational_mathematics.h"

char* function2_view() { return "(sin(x) + cos^2(x)) / (x^2 + x)"; }

double calculate_function2_result(double x)
{
	return (sin(x) + pow(cos(x), 2)) / (pow(x, 2) + x);
}

double calculate_f2_deriative_result(double x)
{
	// ((cos x-2sin(x)cos x)(x^2+x)-(2x+1)(sin x+cos^2x))/(x^2+x)^2
	double s = sin(x), c = cos(x), te = pow(x, 2) + x;
	return ((c - 2 * s * c) * te - (2 * x + 1) * (s + pow(c, 2))) / pow(te, 2);
}

double calculate_f2_deriative2_result(double x)
{
	// Сделано при помощи калькулятора и текстовых замен
	// 
	//((2*x^4+4*x^3+2*x^2)*sin(x)^2+((8*x^3+12*x^2+4*x)*cos(x)-x^4-2*x^3+5*x^2+6*x+2)*sin(x)+((-2*x^4)-4*x^3+4*x^2+6*x+2)*cos(x)^2+((-4*x^3)-6*x^2-2*x)*cos(x))/(x^6+3*x^5+3*x^4+x^3)
	return ((2 * pow(x, 4) + 4 * pow(x, 3) + 2 * pow(x, 2)) * pow(sin(x), 2) + ((8 * pow(x, 3) + 12 * pow(x, 2) + 4 * x) * cos(x) - pow(x, 4) - 2 * pow(x, 3) + 5 * pow(x, 2) + 6 * x + 2) * sin(x) +
		((-2 * pow(x, 4)) - 4 * pow(x, 3) + 4 * pow(x, 2) + 6 * x + 2) * pow(cos(x), 2) + ((-4 * pow(x, 3)) - 6 * pow(x, 2) - 2 * x) * cos(x)) / (pow(x, 6) + 3 * pow(x, 5) + 3 * pow(x, 4) + pow(x, 3));
}

void nonlinear_equation_dbg()
{
	const int text_count = 4;
	const char* Text[] = {
		"Метод секущих",
		"Метод бисекций",
		"Метод Ньютона",
		"Модифицированный метод Ньютона"
	};

	printf_s("Функция f(x) = ");
	printf_s(function2_view());
	printf_s("\n");

	double A, B, eps;
	int segments_count;

	printf_s("Вводите числа в виде 1 или 0,1\n");
	do
	{
		printf_s("Введите левую и правую границу отрезка поиска решения: ");
		scanf_s("%lf%lf", &A, &B);
	} while (B - A <= 0);

	do
	{
		printf_s("Введите количество разделений отрезка: ");
		scanf_s("%d", &segments_count);
	} while (segments_count <= 0);

	eps = input_epsilon();

	printf_s("Выберите метод поиска решения: ");
	int choice = choice_menu_h(Text, text_count);
	finding_roots_method frm = (finding_roots_method)choice;

	int solutions_count = 0;
	startTimer();
	double* solutions = non_linear_equation(&solutions_count, A, B, segments_count, eps, frm, calculate_function2_result, calculate_f2_deriative_result, calculate_f2_deriative2_result);
	stopTimer();

	printf_s("\nВремя поиска решений - %lf\n", getTimerClock());
	printf_s("\nОтвет:\nКоличество решений - %d\n", solutions_count);
	print_solutions(solutions, solutions_count);
}