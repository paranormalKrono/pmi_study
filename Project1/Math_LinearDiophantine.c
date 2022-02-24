#include "MathPMI.h"

// Линейное диофантовое уравнение
int linear_diophantine(const int* a, int* x, int n, int b)
{
	// a_{0}x_{0} + a_{1}x_{1} + ... + a_{n-2}x_{n-2} + a_{n-1}x_{n-1} = b

	int gcd = extended_euclid(a[0], a[1], &x[0], &x[1]);
	int t = 1;
	for (int i = 1; i < n - 1; ++i)
	{
		gcd = extended_euclid(gcd, a[i + 1], &t, &x[i + 1]); // Переходим к виду gcd*t + ... + a_{n-1}*x_{n-1} = b

		for (int j = 0; j <= i; ++j) // Домножаем предыдущие решения x
		{
			x[j] *= t;
		}
	}
	if (gcd == 1 || euclid_rec(gcd, b) != 1) // Есть ли решение
	{
		t = (int)(b / gcd);
		for (int j = 0; j <= n - 1; ++j) // Домножаем всё общее решение
		{
			x[j] *= t;
		}
		return 1;
	}
	else
	{
		return -1;
	}
}