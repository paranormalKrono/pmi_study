#include "MathPMI_Debug.h"

void linear_diophantine_input() {
	printf("__LINEAR_DIOPHANTINE_INPUT__\n");

	const int n = 0;
	int* a;
	int* x;
	int d = 0;

	printf("Введите количество ax: ");
	scanf_s("%d", &n);

	// Выделение памяти
	a = (int*)malloc(n * sizeof(int));
	x = (int*)malloc(n * sizeof(int));

	printf("Введите элементы a:\n");

	// Ввод элементов массива
	for (int i = 0; i < n; i++)
	{
		printf("a[%d] = ", i);
		scanf_s("%d", &a[i]);
	}
	printf("Введите d: ");
	scanf_s("%d", &d);
	linear_diophantine_dbg(a, x, n, d);
}

void linear_diophantine_dbg(int* a, int* x, int n, int d) {
	printf("__LINEAR_DIOPHANTINE_DEBUG__\n");

	for (int i = 0; i < n; ++i) {
		printf("%d*x%d", a[i], i);
		if (i != n - 1) {
			printf(" + ");
		}
		else {
			printf(" = %d\n", d);
		}
	}

	if (linear_diophantine(a, x, n, d) == -1) {
		printf("Целочисленного решения нет\n\n");
	}
	else
	{
		for (int i = 0; i < n; ++i)
		{
			printf("%d*%d", a[i], x[i]);
			if (i != n - 1) {
				printf(" + ");
			}
			else {
				printf(" = %d\n", d);
			}
		}
		printf("Целочисленное решение есть\n\n");
	}
}