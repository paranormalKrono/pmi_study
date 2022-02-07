#include "MathPMI_Examples.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

void test_bigint_create(void) 
{
	unsigned char num[5] = { '1', '3', '6', '5', '\0'}; // 10101010101 = 101 01010101
	unsigned char b_num[2] = { (unsigned char)5, (unsigned char)85 }; // 5 85 = 101 01010101

	big_int* b1 = bigint_create(num); // Создание Big_int 1
	big_int* b2 = bigint_zero();
	b2->length = 2;
	b2->number = b_num;
	CU_ASSERT(bigint_compare(b1, b2));
}

// -- 6 - 1,2 -- 
void legendre_symbol_example(int* legendre(long long p, long long q)) 
{
	printf("--LEGENDRE_SYMBOL_EXAMPLE--\n");
	int p = 273, q = 23; // -1
	// 273 404685641 1
	legendre_symbol_dbg(p, q, legendre);
}

// -- 5 - 1,2 -- 
void chinese_reminder_example(algorithm_type op) {
	printf("--CHINESE_REMINDER_EXAMPLE--\n");

	// 1 2 3 4 5 mod 2 3 5 7 11
	// 1523

	const int k = 5;
	int a[5] = { 1, 2, 3, 4, 5 };
	int m[5] = { 2, 3, 5, 7, 11 };

	chinese_reminder_dbg(&a, &m, k, op);
}

// -- 4 -- a_{0}x_{0} + a_{1}x_{1} + ... + a_{n-2}x_{n-2} + a_{n-1}x_{n-1} = b
void linear_diophantine_example() {
	printf("--LINEAR_DIOPHANTINE_EXAMPLE--\n");

	// 6x + 15y + 10z = 1
	const int n = 3; 
	int a[3] = { 6, 15, 10 };
	int x[3] = { 0, 0, 0 };
	int d = 1;
	
	linear_diophantine_dbg(&a, &x, n, d);
}


// -- 3.2 -- 
void mul_inverse_example() {
	printf("--MUL_INVERSE_EXAMPLE--\n");
	int x = -7, m = -10;
	printf("Обратный %d в %d = %d\n\n", x, m, mul_inverse(x, m)); // -7a + -10b = -1   a = 3
}

// -- 3.1 -- xa + yb = gcd(a, b)
void extended_euclid_example() {
	printf("--EXTENDED_EUCLID_EXAMPLE--\n");
	int a = -15, b = -6;
	printf("a = %d  b = %d\n", a, b);
	extended_euclid_dbg(&a, &b);
}


// -- 2 -- gcd(x, y)
void euclid_recursion_example() {
	printf("--EUCLID_REC_EXAMPLE--\n");
	int x = 99, y = 78;
	euclid_recursion_dbg(x, y); // gcd(99, 78) = 3
}


// -- 1 -- x ^ y mod m
void modPow_recursion_example() {
	printf("--MOD_POW_REC_EXAMPLE--\n");
	int x = 2, y = 10, m = 5;
	printf("%d ^ %d mod %d = %d\n\n", x, y, m, modPow(x, y, m));
}
