#include "Small_exercises.h"
#include <limits.h>
#include <stdio.h>


void show_repeated_int() 
{
	int n = 11;
	int a[11] = { 1, 3, 9, 7, 5, 2, 7, 6, 4, 8 };

	short b = 0;

	if (n % 2 == 1) 
	{
		for (int i = 0; i < n; ++i)
		{
			b = (b + a[i]) % (n-2);
		}
	}
	else 
	{
		for (int i = 0; i < n; ++i)
		{
			b = (b + a[i]) % (n-1);
		}
	}

	if (b == 0) 
	{
		printf_s("%d", n);
	}
	else
	{
		printf_s("%d", b);
	}
}

void show_max_value_segment() 
{
	int cnt = 9;

	//int a[9] = { 1, -3, 4, -10, 3, 8, 9, -1, 100 }; // 4 8 119
	//int a[9] = { -5, -5, -2, -4,-6, -8, -10, -12, -14 }; //  2 2 -2
	//int a[9] = { 1000, -5, 9, 3,-6, 800}; // 0 5 1801
	//int a[9] = {-1000, 200, 4, 7, 2,-8, -100, 100 }; // 1 4 213
	//int a[1] = { -100 };
	//int a[9] = {-5, 160, 5, -3, -200, 15, 100, -400, 300}; // 8 8 300
	int a[9] = { 1000, -999, 1000, -3000, 10000, -10, -5, -3, 100 };
	//int a[4] = { 1000, -1001, 1000, 1000 };

	int lmax = 0, rmax = 0, lnew = 0, smax = INT_MIN, slong = 0;

	for (int i = 0; i < cnt; ++i)
	{
		if (slong < 0)
		{
			lnew = i;
			slong = a[i];
		}
		else
			slong += a[i];

		if (slong > smax)
		{
			lmax = lnew;
			smax = slong;
			rmax = i;
		}
		printf_s("%d %d %d	 %d %d\n", lmax, rmax, lnew, smax, slong);
	}

	printf_s("%d %d %d", lmax, rmax, smax);
}