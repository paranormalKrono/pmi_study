#include "MathPMI.h"

// Обратный элемент
int mul_inverse(int x, int m)
{
	if (euclid_rec(x, m) > 1) { // gcd(x, m) > 1
		return 0;
	}
	else
	{
		int k = 0, t;
		extended_euclid(x, m, &k, &t); // xk + mt
		k %= m;
		if (k < 0)
		{
			k += m;
		}
		return k;
	}
}