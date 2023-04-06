#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h> 
#include "algebraic_interpolation.h"


int compare_segments_by_A(const void* v1, const void* v2)
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
	qsort((void*)nearest_pairs, (size_t)nearest_count, sizeof(line_segment*), compare_segments_by_A);
	for (int i = 0; i < nearest_count; ++i) nearest_pairs[i]->A += x;

	return nearest_pairs;
}

int find_nearest_pair_index(line_segment** function_pairs, int values_count, int nearest_count, double x)
{
	int index = 0;
	while (function_pairs[index]->A < x) ++index;
	if (index > 0 && fabs(x - function_pairs[index]->A) > fabs(x - function_pairs[index - 1]->A))  index--;
	return index;
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

double* Newton_coeffs(line_segment** nearest_pairs, int degree)
{
	double** fk = malloc(sizeof(double*) * (degree + 1));
	double* res = malloc(sizeof(double) * (degree + 1));
	if (!fk)
	{
		printf_s("Память под fk не была выделена");
		return 0;
	}
	fk[0] = malloc(sizeof(double) * (degree + 1));
	for (int i = 0; i < degree + 1; ++i)
	{
		fk[0][i] = nearest_pairs[i]->B;
	}
	res[0] = nearest_pairs[0]->B;

	int bound;
	for (int k = 1; k < degree + 1; ++k)
	{
		bound = degree + 1 - k; // Каждый новый столбец имеет на 1 элемент меньше, чем предыдущий
		fk[k] = malloc(sizeof(double) * bound); // Выделяем память под элементы столбцов
		for (int i = 0; i < bound; ++i)
		{
			fk[k][i] = (fk[k - 1][i] - fk[k - 1][i + 1]) / (nearest_pairs[i]->A - nearest_pairs[i + k]->A); // Вычисляем значение элементов в столбцах
		}
		res[k] = fk[k][0];
	}
	for (int i = 0; i < degree + 1; ++i) free(fk[i]);
	free(fk);
	return res;
}

double Newton(line_segment** nearest_pairs, double x, int degree, double* fk)
{
	double res = fk[0], factor = 1;

	for (int k = 1; k < degree + 1; ++k)
	{
		factor *= (x - nearest_pairs[k - 1]->A);

		res += factor * fk[k];
	}

	return res;
}

