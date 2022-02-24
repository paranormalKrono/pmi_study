#include "MathPMI.h"

// p-1 в виде 2^k * h (h - нечЄтное)
//  ритерий Ёйлера

// (r, b)
// (k-1,1) -> (0, b)
//a^{2^r * h} * b^{2^{r+1}} == 1 (p)
//r' - наимененьшее: 1,2,...,r
//a^{2^r' * h} * b^{2^{r'+1}} == 1 (p)
int Quadratic_comparison(long long a, long long p)
{
	long long x = modPow_rec(a, ((p - 1) >> 1), p);
	if (x == 0 || x == 1) {
		return x;
	}
	else {
		return -1;
	}
}