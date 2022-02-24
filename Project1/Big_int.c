#include "Big_int.h"
#include <string.h>
#include <stdlib.h>

big_int* bigint_zero();
void bigint_realloc_to_newhigherlength(big_int* b, const unsigned int new_length);

big_int* bigint_create(const unsigned char* num)
{
	big_int* b = bigint_zero();

	int t = 0, k = 0;

	int i; // ��������
	if (num[0] == '-') // ��������� ������������� �� ����� 
	{
		b->isNegative = true;
		i = 1;
	}
	else
	{
		b->isNegative = false;
		i = 0;
	}

	for (; i < strlen(num); ++i) // �������� �� 10-������� �����
	{
		k = num[i] - '0'; // ��������� ����� �� 10 � 256 ������� ���������, ������ char
		for (int j = 0; j < b->length; ++j) // �������� ������� �� 10 �� ����������
		{
			t = (unsigned int)b->number[j]; // ���� �����
			t = (t + (t << 2)) << 1; // ��������� �� 10

			b->number[j] = (unsigned char)((k + t) & 0xFF); // ��������� ������� � ���������� �� 10 
			k = (k + t) >> 8; // ����������� ������� ��� ���������� �������
		}
		if (k > 0) // ������� ��� ���������� �������
		{
			bigint_realloc_to_newhigherlength(b, b->length + 1); // ��������� ��� �������
			b->number[b->length - 1] = (unsigned char)k;
		}
	}

	return b;
}

big_int* bigint_zero() // �������� ������� big_int
{
	big_int* b = (big_int*)malloc(sizeof(big_int));
	b->number = (unsigned char*)malloc(sizeof(unsigned char*));
	b->number[0] = 0;
	b->length = 1;
	b->isNegative = false;
	return b;
}

void bigint_show(const big_int* num) // ����� ������������� Big_int
{
	if (num->isNegative)
	{
		printf("-");
	}


	printf("[");
	for (int i = num->length - 1; i >= 0; --i)
	{
		printf("%d", (int)(num->number[i]));
		if (i == 0)
			break;
		printf(" ");
	}
	printf("]	[");
	if (num->isNegative)
	{
		printf("-");
	}
	for (int i = num->length - 1; i >= 0; --i) 
	{
		for (int j = 7; j >= 0; --j)
		{
			printf("%d", ((int)(num->number[i]) & (1 << j)) >> j);
		}
		if (i == 0)
			break;
		printf(" ");
	}
	printf("]");
	printf("\n");
}

void bigint_add_bigint(big_int* b1, const big_int* b2);
void bigint_subtract_bigint(big_int* b1, const big_int* b2);
void bigint_multiply_bigint(big_int* b1, const big_int* b2);
void bigint_bitshift_left(big_int* b, const int x);


void bigint_arithmetic_operation(big_int* b1, const big_int *b2, const arithmetic_operations operation) // ��������� � b1 �������� operation � ��������� b2
{
	switch (operation) // ����� �������� � ����������� �� ��������
	{
	case add:
		if (b1->isNegative) 
		{
			if (b2->isNegative) bigint_add_bigint(b1, b2); // ����������
			else bigint_subtract_bigint(b1, b2); // ��������
		}
		else
		{
			if (b2->isNegative) bigint_subtract_bigint(b1, b2); // ��������
			else bigint_add_bigint(b1, b2); // ����������
		}
		break;
	case subtruct:
		if (b1->isNegative)
		{
			if (b2->isNegative) bigint_subtract_bigint(b1, b2); // ����������
			else bigint_add_bigint(b1, b2); // ��������
		}
		else
		{
			if (b2->isNegative) bigint_add_bigint(b1, b2); // ����������
			else bigint_subtract_bigint(b1, b2); // ��������
		}
		break;
	case multiply:
		if (b1->isNegative == b2->isNegative) // ��� ������������� ��� �������������
		{
			b1->isNegative = false;
		}
		else if (b2->isNegative)
		{
			b1->isNegative = true;
		}
		bigint_multiply_bigint(b1, b2); // ���������
		break;
	default:
		break;
	}
}


void bigint_assign(big_int* b1, const big_int* b2)
{
	b1->length = b2->length;
	b1->isNegative = b2->isNegative;
	free(b1->number); 
	b1->number = (unsigned char*)malloc(b1->length);
	memcpy_s(b1->number, b1->length * sizeof(unsigned char), b2->number, b2->length * sizeof(unsigned char));
}

// true, ���� �����
bool bigint_compare(const big_int* b1, const big_int* b2)
{
	if (b1->length != b2->length || b1->isNegative != b2->isNegative) 
	{
		return false;
	}
	for (int i = 0; i < b1->length; ++i) 
	{
		if (b1->number[i] != b2->number[i]) 
		{
			return false;
		}
	}
	return true;
}


//-1: left < right 
// 0: �����
// 1: left > right
int bigint_compare_number(const big_int* left, const big_int* right)
{
	if (left->length > right->length)
	{
		return 1;
	}
	else if (left->length < right->length)
	{
		return -1;
	}
	else
	{
		for (int i = left->length; i >= 0; --i)
		{
			if (left->number[i] > right->number[i])
			{
				return 1;
			}
			else if (left->number[i] < right->number[i])
			{
				return -1;
			}
		}
	}
	return 0;
}

// �������� ����� big_int
unsigned int bigint_binary_length(const big_int* bigint)
{
	unsigned int binary_length = (bigint->length - 1) * 8; // �����

	int i = 0;
	for (i = 7; i >= 0; --i)
	{
		if (((int)(bigint->number[bigint->length - 1]) & (1 << i)) >> i)
		{
			binary_length += i + 1; // ����
			break;
		}
	}
}

void bigint_add_bigint(const big_int* left, const big_int* right)
{
	unsigned int carry = 0; // ����� �������
	unsigned int rank = 0; // ������� ������

	bigint_realloc_to_newhigherlength(left, right->length); // ��������� left �� ������� right
	while (rank < left->length - 1) // ���������, ���� ������� ������ � �������� �����
	{
		carry += left->number[rank] + right->number[rank];// ���������� � carry ������� �� left � right

		// ��������� carry �� �����, ������� ��� � ������� ������ � ���������
		left->number[rank] = carry & 0xFF;
		carry >>= 8;

		rank++; // ��������� � ���������� �������
	}

	if (carry != 0) // ���� ���-�� ��������
	{
		bigint_realloc_to_newhigherlength(left, left->length + 1); // ��������� left �� 1 ������
		left->number[rank] = carry;
	}
}

void bigint_subtract_bigint(big_int* left, const big_int* right) // left ������� right
{
	if (bigint_compare_number(left, right) <= 0) // ���� left <= right, �� left: number = 0, length = 1
	{
		bigint_free(left);
		left = bigint_zero();
		return;
	}

	// left > right
	int carry = 0, difference = 0;
	for (unsigned int rank = 0; rank < left->length; ++rank) // �������� ��� �������
	{
		// ���������� ������� � ������� rank � left � right � �� �������� carry
		difference = left->number[rank] - carry;
		if (rank < right->length) 
		{
			difference -= right->number[rank];
		}
		carry = difference < 0;
		left->number[rank] = difference + (carry << 8);
	}

	// ������� ����
	int i;
	for (i = left->length - 1; i >= 0; --i)
	{
		if (left->number[i])
		{
			break;
		}
	}
	left->number = (unsigned char*)realloc(left->number, sizeof(unsigned char) * (++i));
	left->length = i;
}

void bigint_multiply_bigint(big_int *b1, const big_int *b2)
{
	// ���������
	big_int* b_result = bigint_zero();
	b_result->isNegative = b1->isNegative; // ��������� ����

	int bitshift = 0; // ����� �� ������� ������� b1

	// �������� ��� �������� ������� b2, �� ������� ����� ��������
	for (int i = 0; i < b2->length; ++i) 
	{
		for (int j = 0; j < 8; j++)
		{
			// ���� �������� ������ b2 ����� �������, �� 
			// ��������� � ���������� b1 ��������� �� i * 8 + j ��������
			if ((b2->number[i] & (1 << j)) >> j == 1) 
			{
				bigint_bitshift_left(b1, i * 8 + j - bitshift);
				bitshift = i * 8 + j;
				bigint_add_bigint(b_result, b1);
			}
		}
	}
	bigint_assign(b1, b_result); // ���������� ��������� � b1
	bigint_free(b_result);
}

void bigint_bitshift_left(big_int* b, const int x) // big_int, �����
{ 
	// 45032 �� 18 ��������
	// 262144
	// 
	// 00000001
	// 00000010
	// 
	// 00000000 00000000 00000000 [101][01111] [111][01000]
	// 00[101] [01111][111] [01000]000 00000000 00000000
	int last_length = b->length; // ���������� ������� ����� big_int

	// ��������� ����������� �������
	if (b->number[b->length - 1] << (x & 7) > 255)
	{
		bigint_realloc_to_newhigherlength(b, b->length + (x >> 3) + 1); // � ����������� ��������
		b->number[b->length - 1] = 0;
	}
	else 
	{
		bigint_realloc_to_newhigherlength(b, b->length + (x >> 3));
		b->number[b->length - 1] = b->number[b->length - 1 - (x >> 3)] << (x & 7);
	}

	// ��������� ������ ���� = 0, ���� ��� ���� �����
	int i = b->length - 2;
	for (; i >= x >> 3; --i)  // �������� �� ����� ��������� �������� ������ Big_int
	{
		b->number[i + 1] += b->number[i - (x >> 3)] >> (8 - (x & 7)); // ��������� ������� [00000]000 -> 000[00000]
		b->number[i] = (b->number[i - (x >> 3)] << (x & 7)) & 255; // ������ ������� 00000[000] -> [000]00000
	}
	for (; i >= 0; --i) // �������� ���������� �������
	{
		b->number[i] = 0;
	}
}

big_int* bigint_division(const big_int* left, const big_int* right, big_int* remainder)
{

	big_int* quotient = bigint_zero(); // �������
	remainder = bigint_zero();

	// ���� �����, ������� = 0, �������� = 1
	if (bigint_compare_number(left, right) == 0) 
	{
		quotient->number[0] = 1;
		return quotient;
	}

	// ����� ���������� �������
	bigint_assign(remainder, left); 

	// � ���� left ������ right, ������� = left, � �������� ����� 0
	if (bigint_compare_number(left, right) == -1) 
	{
		return quotient;
	}

	// ����� ���� ������� � �������

	big_int* divider = bigint_zero(); // ��������


	// ���������� �������� ����� ��������
	unsigned int right_binary_length = bigint_binary_length(right);

	// ����� ������ ������� ���� � �������� ������������� ����� �������� � ��������� ����� ������� ��������� �� ��������

	int binary_length_difference = 0; 
	while (bigint_compare_number(remainder, right) > 0) // ���� ������� ������ �������� 
	{
		bigint_assign(divider, right);

		// ���������� ������� ���� � �������� ������������� ������� � ��������
		binary_length_difference = (int)bigint_binary_length(remainder) - (int)right_binary_length;

		// ���� ������� ���� ������ ��� ����� �� ����� ��������
		if (binary_length_difference <= 0) 
			break;

		// �������� ����� ��������
		bigint_bitshift_left(divider, binary_length_difference);

		// �������� �� ������� ������������ ��������
		bigint_subtract_bigint(remainder, divider);

		quotient += 1 << binary_length_difference;
	}

	// ���� ������� � ����� ����� 0, �������� ��������, ���� �� ������ ��� ����� �������
	if (binary_length_difference == 0)
	{
		while (bigint_compare_number(remainder, right) >= 0)
		{
			bigint_subtract_bigint(remainder, right);
			quotient += 1;
		}
	}
	return quotient;
}

void bigint_realloc_to_newhigherlength(big_int* b, unsigned int new_length)
{
	if (b->length < new_length)
	{
		unsigned char* new_n = (unsigned char*)realloc(b->number, sizeof(unsigned char) * (new_length));
		if (new_n == NULL) 
		{
			new_n = (unsigned char*)malloc(new_n, sizeof(unsigned char) * (new_length));
			memcpy_s(new_n, new_length * sizeof(unsigned char), b->number, new_length * sizeof(unsigned char));
		}
		b->number = new_n;

		for (unsigned int i = b->length; i < new_length; ++i)
		{
			b->number[i] = 0;
		}
		b->length = new_length;
	}
}

void bigint_free(big_int* b) // ���������� ������
{
	free(b->number);
	free(b);
}