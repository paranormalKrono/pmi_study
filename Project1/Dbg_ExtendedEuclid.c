#include "MathPMI_Debug.h"

void extended_euclid_input() {
	printf("__EXTENDED_EUCLID_INPUT__\n");

	int a = 0, b = 0;
	printf("¬ведите a и b: ");
	scanf_s("%d %d", &a, &b);
	extended_euclid_dbg(&a, &b);
}

void extended_euclid_dbg(int* a, int* b) {
	printf("__EXTENDED_EUCLID_DEBUG__\n");

	int x = 0, y = 0;
	printf("%dx + %dy = gcd(a,b)\n", *a, *b);
	int d = extended_euclid(*a, *b, &x, &y);
	printf("%d*%d + %d*%d = %d\n\n", *a, x, *b, y, d);
}