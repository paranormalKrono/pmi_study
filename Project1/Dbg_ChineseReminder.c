#include "MathPMI_Debug.h"

void chinese_reminder_input(algorithm_type op)
{
	printf("__CHINESE_REMINDER_INPUT__\n");

	const int k = 0;
	int* a;
	int* m;

	printf("Введите количество x == a (mod m): ");
	scanf_s("%d", &k);

	// Выделение памяти
	a = (int*)malloc(k * sizeof(int));
	m = (int*)malloc(k * sizeof(int));

	printf("Введите элементы a:\n");

	// Ввод элементов массива
	for (int i = 0; i < k; i++)
	{
		printf("a[%d] = ", i);
		scanf_s("%d", &a[i]);
		printf("m[%d] = ", i);
		scanf_s("%d", &m[i]);
	}
	chinese_reminder_dbg(a, m, k, op);
}

void chinese_reminder_dbg(const int a[], const int m[], int k, algorithm_type op)
{
	printf("__CHINESE_REMINDER_DEBUG__\n");

	for (int i = 0; i < k; ++i) {
		printf("x == %d (mod %d)\n", a[i], m[i]);
	}

	int x;
	if (op == iterative) {
		x = chinese_reminder_iterative(a, m, k);
	}
	else
	{
		x = chinese_reminder(a, m, k);
	}
	printf("x == %d\n", x);

	if (x == -1) {
		printf("Решения нет\n\n");
	}
	else
	{
		printf("Решение есть - %d\n\n", x);
	}
}