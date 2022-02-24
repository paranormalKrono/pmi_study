#include "MathPMI_Debug.h"

double legendre_time(int iterations, int* legendre(long long, long long))
{
	printf("__LEGENDRE_SYMBOL_TIME__\n");
	long long* a[5] = { 342, 201, 9, 30, 273 };
	long long* p[5] = { 354683617, 404432, 404685, 23, 404685641 };
	//int* l[5] = { -1, 1, 0, -1, 1};
	startTimer();
	printf("Время - %f секунд\n", getClock(clock1));
	for (int i = 0; i < iterations; ++i)
	{
		legendre(a[i % 5], p[i % 5]);
	}
	stopTimer();
	printf("Время - %f секунд\n", getClock(clock2));
	printf("Время выполнения %f секунд\n итераций %d\n", (float)getTimerClock(), iterations);
	return getTimerClock();
}

void legendre_symbol_input(int* legendre(long long, long long)) {
	printf("__LEGENDRE_SYMBOL_INPUT__\n");

	long long p = 37, a = 32;
	//printf("Введите p и a: ");
	//scanf_s("%d %d", &p, &a);
	//for(int a = 1; a < p; a++)
	legendre_symbol_dbg(p, a, legendre);
}

void legendre_symbol_dbg(long long p, long long a, int* legendre(long long, long long)) {
	printf("__LEGENDRE_SYMBOL_DEBUG__\n");

	printf("Символ Лежандра при p = %d и a = %d,  %d\n\n", (int)p, (int)a, legendre(p, a));
}