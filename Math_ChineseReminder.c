#include "MathPMI.h"

// ��������� ������� �� ��������, �����������
int chinese_reminder_iterative(const int a[], const int m[], int k)
{
	// �������� �� ���������� ��������
	// c � d
	// m[0]c + m[1]d = 1
	// a[1]m[0]c + a[0]m[1]d �������� � a[0] mod m[0] � � a[1] mod m[1]

	for (int i = 0; i < k - 1; ++i)
	{
		for (int j = i + 1; j < k; ++j) {
			if (euclid_rec(m[i], m[j]) != 1) {
				return -1;
			}
		}
	}

	int A = a[0], M = m[0], c, d;

	// M*c + m[1]*d = 1 
	// a[1]*M*c + A*m[1]*d
	for (int i = 1; i < k; ++i) // ������� A � M
	{
		c = 0, d = 0;
		extended_euclid(M, m[i], &c, &d);
		A = a[i] * M * c + A * m[i] * d;
		M *= m[i];
	}

	A %= M;
	return A < 0 ? A + M : A;
}


//��������� ������� �� ��������
int chinese_reminder(const int a[], const int m[], int k)
{
	// �������� �� {x = a[i](mod m[i]) | i=0..k-1}
	// -1 ���� ������ �� ������� ������� �������
	// ����� ������� �� [0, M)
	// M = ������������ m[i]

	int M = 1;
	for (int i = 0; i < k - 1; ++i)
	{
		//for (int j = i + 1; j < k; ++j) {
		if (euclid_rec(m[i], M) != 1) {
			return -1;
		}
		//}
		M *= m[i];
	}
	M *= m[k - 1];

	int x = 0, Mi = 0;
	for (int i = 0; i < k; ++i)
	{
		Mi = M / m[i];
		x += a[i] * Mi * mul_inverse(Mi, m[i]);
	}

	//printf("x == %d (mod %d)\n", x, M);

	return x % M;
}