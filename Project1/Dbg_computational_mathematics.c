#include <math.h> 
#include <float.h>
#include "MathPMI_Debug.h"
#include "Menu.h"
#include "computational_mathematics.h"
#include "Time_Debug.h"


void nonlinear_equation_dbg();
void algebraic_interpolation_dbg();
void computational_mathematics_dbg()
{
	printf_s("__COMPUTATIONAL_MATHEMATICS_DEBUG__\n");

	const int text_count = 2;
	const char* Text[] = {
		"Задача алгебраического интерполирования",
		"Решение трансцендентного или алгебраического уравнения вида f(x) = 0 на [A,B]"
	};
	const void (*methods[])() = { 
		algebraic_interpolation_dbg,
		nonlinear_equation_dbg
	};

	printf_s("Выберите, что нужно сделать: ");
	int choice = choice_menu_h(Text, text_count);
	methods[choice]();
}

char* function1_view() { return "(sin(x) + cos^2(x)) / (x^2 + x)"; }
char* function2_view() { return "exp(x) - x"; }



double calculate_function2_result(double x)
{
	return exp(x) - x;
}

line_segment** get_function_pairs(line_segment* ls, int values_count, const double (*function)(double x))
{
	line_segment** values = malloc(sizeof(line_segment*) * values_count);
	double x = ls->A;
	double h = fabs(ls->B - ls->A) / (values_count - 1);
	for (int i = 0; i < values_count; ++i) 
	{
		values[i] = line_segment_init(x, function(x));
		x += h;
	}
	return values;
}

void print_function_pairs(line_segment** pairs, int count) 
{
	for (int i = 0; i < count; ++i) 
	{
		printf_s("%d] %lf %lf\n", i, pairs[i]->A, pairs[i]->B);
	}
}

int find_nearest_pair_index(line_segment** function_pairs, int values_count, int nearest_count, double x)
{
	int index = 0;
	while (function_pairs[index]->A < x) ++index;
	if (index > 0 && fabs(x - function_pairs[index]->A) > fabs(x - function_pairs[index - 1]->A))  index--;
	return index;
}

int compare_pairs(const void* v1, const void* v2)
{
	line_segment* ls1 = *(line_segment**)v1, * ls2 = *(line_segment**)v2;
	if (fabs(ls1->A) > fabs(ls2->A)) return 1;
	else if (fabs(ls1->A) < fabs(ls2->A)) return -1;
	else return 0;
}

line_segment** collect_nearest_function_pairs(line_segment** function_pairs, int values_count, int nearest_count, double x)
{
	line_segment** nearest_pairs = malloc(sizeof(line_segment*) * nearest_count);
	if (!nearest_pairs) 
	{
		printf_s("Ошибка при выделении памяти для ближних пар");
		return NULL;
	}

	int nearest_index = find_nearest_pair_index(function_pairs, values_count, nearest_count, x);

	int index_shifter; // дельта nearest_index
	if (nearest_count % 2 == 1 || x < function_pairs[nearest_index]->A) index_shifter = 0; // только для чётного случая нужен выбор крайнего левого или правого узла
	else index_shifter = 1;

	int start_index = nearest_index - (int)((float)nearest_count / 2) + index_shifter;
	if (start_index < 0)
		start_index = 0; // сдвигаемся вправо
	else if (start_index + nearest_count > values_count)
		start_index -= start_index + nearest_count - values_count; // сдвигаемся влево

	for (int i = start_index, j = 0; j < nearest_count; ++i, ++j)
		nearest_pairs[j] = function_pairs[i];

	//Сортировка
	for (int i = 0; i < nearest_count; ++i) nearest_pairs[i]->A -= x;
	qsort((void*)nearest_pairs, (size_t)nearest_count, sizeof(line_segment*), compare_pairs);
	for (int i = 0; i < nearest_count; ++i) nearest_pairs[i]->A += x;

	return nearest_pairs;
}

void free_line_segments(line_segment** ls, int count)
{
	for (int i = 0; i < count; ++i)
	{
		free(ls[i]);
	}
	free(ls);
}

void algebraic_interpolation_calculate_dbg(line_segment** nearest_pairs, double x, int degree, const double (*function)(double x));
void algebraic_interpolation_dbg() 
{
	printf_s("Функция f(x) = "); // [0,2], m + 1 = 41, n = 12
	printf_s(function2_view());
	printf_s("\n");

	printf_s("Вводите числа в виде 1 или 0,1\n");

	double A, B, x, x_prev = 1;
	line_segment* ls;
	int pairs_count, degree; // m+1 и n

	do
	{
		printf_s("Введите левую и правую границу отрезка: ");
		scanf_s("%lf%lf", &A, &B); 
	} while (B - A <= 0);
	ls = line_segment_init(A, B);

	do
	{
		printf_s("Введите количество пар аргумент-значение функции: ");
		scanf_s("%d", &pairs_count); 
	} while (pairs_count <= 1);
	printf_s("Тогда m = %d\n", pairs_count - 1);

	line_segment** function_pairs = get_function_pairs(ls, pairs_count, calculate_function2_result);

	line_segment** nearest_pairs = malloc (sizeof(line_segment*));
	char answer;
	do 
	{
		printf_s("\n\n----  ----\n");
		print_function_pairs(function_pairs, pairs_count);
		answer = '0';
		do
		{
			x = DBL_MAX;
			printf_s("Введите точку интерполирования из отрезка [%lf, %lf]: ", ls->A, ls->B);
		} while (!scanf_s("%lf", &x) || !segment_check_inside(x, ls));


		do
		{
			printf_s("Введите степень интерполяционного многочлена не больше %d: ", pairs_count - 1);
			scanf_s("%d", &degree);
		} while (degree > pairs_count - 1 || degree < 0);
		printf_s("Тогда n = %d\n", degree);

		if (x != x_prev)
		{
			x_prev = x;
			if (nearest_pairs)
				free(nearest_pairs); // Очищаем только указатели

			nearest_pairs = collect_nearest_function_pairs(function_pairs, pairs_count, degree + 1, x);
			if (!nearest_pairs)
				printf_s("Ошибка выделения памяти под ближайшие пары");

		}

		print_function_pairs(nearest_pairs, degree + 1);

		algebraic_interpolation_calculate_dbg(nearest_pairs, x, degree, calculate_function2_result);

		printf_s("%s", "Вы хотите выбрать другие x и степень интерполяционного многочлена? Y/N (Да/Нет)\n");
		while (answer != 'Y' && answer != 'N')
			scanf_s("%c", &answer, 1);
	} while (answer == 'Y');

	for (int i = 0; i < pairs_count; ++i) 
	{
		free(function_pairs[i]);
	}
}

double Lagrange(line_segment** nearest_pairs, double x, int degree) 
{
	double res = 0, lkn;

	for (int k = 0; k < degree + 1; ++k) 
	{
		lkn = 1;
		for (int j = 0; j < degree + 1; ++j) 
		{
			if (j != k) lkn *= (x - nearest_pairs[j]->A) / (nearest_pairs[k]->A - nearest_pairs[j]->A);
		}

		res += lkn * nearest_pairs[k]->B;
	}

	return res;
}

double Newton(line_segment** nearest_pairs, double x, int degree)
{
	double** fk = malloc(sizeof(double*) * (degree + 1)); // f(xi, ..., xi+k)
	if (!fk)
	{
		printf_s("Память под fk не была выделена");
		return 0;
	}
	//printf_s("\n");
	fk[0] = malloc(sizeof(double) * degree + 1);
	for (int i = 0; i < degree + 1; ++i)
	{
		fk[0][i] = nearest_pairs[i]->B; // 1 столбец f(x_0) ... f(x_degree)
		//printf_s("[0, %d] | f(x%d) = %lf\n", i, i, fk[0][i]);
	}
	//printf_s("\n");

	int bound;
	for (int k = 1; k < degree + 1; ++k) 
	{
		bound = degree + 1 - k; // Каждый новый столбец имеет на 1 элемент меньше, чем предыдущий
		fk[k] = malloc(sizeof(double) * bound); // Выделяем память под элементы столбцов

		for (int i = 0; i < bound; ++i) 
		{
			fk[k][i] = (fk[k - 1][i] - fk[k - 1][i + 1]) / (nearest_pairs[i]->A - nearest_pairs[i+k]->A); // Вычисляем значение элементов в столбцах
			//printf_s("[%d, %d] | f(x%d,...,x%d) | [%d, %d] - [%d, %d] | x%d - x%d\n", k, i, i, i + k, k - 1, i+1, k - 1, i, i, i + k);
		}
		//printf_s("\n");
	} 
	//printf_s("\n");


	double res = fk[0][0], factor = 1;
	free(fk[0]);

	for (int k = 1; k < degree + 1; ++k) 
	{
		factor *= (x - nearest_pairs[k-1]->A);

		res += factor * fk[k][0];

		free(fk[k]);
	}

	free(fk);
	return res;
}

void algebraic_interpolation_calculate_dbg(line_segment** nearest_pairs, double x, int degree, const double (*function)(double x))
{
	double PnL = Lagrange(nearest_pairs, x, degree);
	printf_s("Значение функции по многочлену Лагранжа: PnL = %lf\n", PnL);
	printf_s("Абсолютная фактическая погрешность для формы Лагранжа: |f(x) - PnL(x)| = %.E = %.16lf\n", fabs(function(x) - PnL), fabs(function(x) - PnL));

	double PnN = Newton(nearest_pairs, x, degree);
	printf_s("Значение функции по многочлену Ньютона: PnN = %lf\n", PnN);
	printf_s("Абсолютная фактическая погрешность для формы Ньютона: |f(x) - PnN(x)| = %.E = %.16lf\n", fabs(function(x) - PnN), fabs(function(x) - PnN));

	printf_s("|PnN(x) - PnL(x)| = %.E = %.16lf\n\n", fabs(PnL - PnN), fabs(PnL - PnN));
}


double calculate_function1_result(double x)
{
	return (sin(x) + pow(cos(x), 2)) / (pow(x, 2) + x);
}

double calculate_f1_deriative_result(double x)
{
	// ((cos x-2sin(x)cos x)(x^2+x)-(2x+1)(sin x+cos^2x))/(x^2+x)^2
	double s = sin(x), c = cos(x), te = pow(x, 2) + x;
	return ((c - 2 * s * c) * te - (2 * x + 1) * (s + pow(c, 2))) / pow(te, 2);
}

double calculate_f1_deriative2_result(double x)
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
	printf_s(function1_view());
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

	do
	{
		eps = 0;
		printf_s("Введите точность поиска решения: ");
	} while (!scanf_s("%lf", &eps) && eps <= 0);

	printf_s("Выберите метод поиска решения: ");
	int choice = choice_menu_h(Text, text_count);
	finding_roots_method frm = (finding_roots_method)choice;

	int solutions_count = 0;
	startTimer();
	line_segment** line_segments = non_linear_equation(&solutions_count, A, B, segments_count, eps, frm, calculate_function1_result, calculate_f1_deriative_result, calculate_f1_deriative2_result);
	stopTimer();

	printf_s("\nВремя выполнения - %lf\n", getTimerClock());
	printf_s("\nОтвет:\nКоличество решений - %d\n", solutions_count);
	print_segments(line_segments, solutions_count);
}