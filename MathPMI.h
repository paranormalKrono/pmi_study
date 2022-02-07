#ifndef	_MATHPMI_H_
#define	_MATHPMI_H_
#include <stdio.h>
#include <locale.h>
#include "Big_int.h"

int legendre_symbol(long long p, long long a);
int legendre_symbol_2(long long p, long long q);
int legendre_symbol_3(long long p, long long q);

int chinese_reminder_iterative(const int a[], const int m[], int k);

int chinese_reminder(const int a[], const int m[], int k);

int linear_diophantine(const int* a, int* x, int n, int b);

int mul_inverse(int x, int m);

int extended_euclid(int a, int b, int* x, int* y);

int euclid_rec(int a, int b);

long long modPow_rec(long long x, long long y, long long m);

int euclid(int a, int b);

long long modPow(long long x, long long y, long long m);

#endif // _MATHPMI_H_