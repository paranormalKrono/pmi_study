#include "MathPMI.h"

// Ёвклид рекурси€
int euclid_rec(int a, int b)
{
	return !b ? abs(a) : euclid_rec(b, a % b);
}


int euclid(int a, int b)
{
	int t = 0;
	while (b)
	{
		t = b;
		b = a % b;
		a = t;
	}
	return abs(a);
}
