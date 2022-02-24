#include "MathPMI.h"

// Расширенный Эвклид
int extended_euclid(int a, int b, int* x, int* y)
{
	// ax + by = d
	// a = qb + r
	if (b == 0) // r = 0 -> return b
	{
		//printf("%d = %d\n", a, b);
		// a = qb + r
		// a = qb + 0	-> b = gcd = r
		// a = qb		-> a = gcd, b = 0 

		*x = 1;
		*y = 0;
		//printf("x = %d  y = %d\n", *x, *y);
		return a;
	}

	int q = (int)(a / b);
	//printf("%d = %d*%d + %d\n", a, q, b, a - q * b);



	int d = extended_euclid(b, a - q * b, x, y); // a = qb + r -> b  = q(a - q * b) + r | a = qb + r

	int s = *y;
	*y = *x - q * (*y);
	*x = s;
	//printf("x = %d  y = %d\n", *x, *y);

	// To do
	/*if (d < 0)
	{
		d = -d;
		*x = -*x;
		*y = -*y;
	}*/

	return d;
}

