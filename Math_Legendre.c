#include "MathPMI.h"

// ��������� �����, �������� 1) � 2)
// 1) �������� ������ � 2 ������
// 2) ������������ ����� ���������� � ��������� ������� (��������� ��������) 
// <10 ������
// 10**7 �� ������������������

// ������ ��������
// 0, ���� a ������� �� p
// 1, ���� � ���������� ����� �� ������ p (���������� x**2==a(mod p), (a,p) = 1)
// -1, ���� � ���������� ������� �� (mod p)
int legendre_symbol(long long a, long long p)
{
	// L == a^{(p-1)/2} (mod p)
	long long L = modPow_rec(a, ((p - 1) >> 1), p);
	if (L == 0 || L == 1) {
		return L;
	}
	else {
		return -1;
	}
}

// ������������ ����� ���������� p, q - �������, �������, ������, �����
// p/q = q/p * (-1)**((p-1)/2)((q-1)/2)
int legendre_symbol_2(long long p, long long  q)
{
	// p/q = L
	int t = 0, L = 1;
	while (p > 1)
	{
		// p - ������
		while (!p)
		{
			if ((q * q - 1) >> 3) {
				L = -L;
			}
			p >>= 1;
		}
		if (p >= q)
		{
			p %= q;
		}
		else
		{
			if ((p & 3) == (q & 3) == 3)
			{
				L = -L;
			}
			t = p;
			p = q;
			q = t;
		}
	}
	if (q != 1 && p != 2)
	{
		if (p == -1 && (p - 1) >> 1) {
			L = -L;
		}
	}
	return L;
}

int legendre_symbol_3(long long p, long long  q)
{
	// p/q = L
	int t = 0, L = 1;
	while (p > 2)
	{
		if (p >= q)
		{
			p %= q;
		}
		else
		{
			if (((p & 3) == 3) && ((q & 3) == 3))
			{
				L = -L;
			}
			t = p;
			p = q;
			q = t;
		}
	}
	if (q != 1)
	{
		if (p == 2)
		{
			if ((p * p - 1) >> 3) {
				L = -L;
			}
		}
		else if (p == -1 && (p - 1) / 2 % 2 == 1) {
			L = -L;
		}
	}
	return L;
}