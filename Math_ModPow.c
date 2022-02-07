#include "MathPMI.h"

// x^y mod m	рекурсия
long long modPow_rec(long long x, long long y, long long m)
{
	if (y & 1)
	{
		x *= x;
	}
	if (x > m)
	{
		x %= m;
	}
	x *= x;
	y >>= 1;

	//printf("%d^%d mod %d\n", x, y, m);
	return !y ? x % m : modPow_rec(x, y, m);
}


// x ^ y mod m
long long modPow(long long x, long long y, long long m)
{
	long long t = 1;
	while (y)
	{
		if (x > m)
		{
			x %= m;
		}
		if (y & 1)
		{
			t *= x;
			t %= m;
		}
		x *= x;
		y >>= 1;
	}
	return t % m;
}