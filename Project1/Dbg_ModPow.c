#include "MathPMI_Debug.h"

void modPow_recursion_input() {
	printf("__MODPOW_RECURSION_INPUT__\n");

	int x = 0, m = 0;
	long y = 0;
	printf("¬ведите x, y и m: ");
	scanf_s("%d %d %d", &x, &y, &m);
	modPow_recursion_dbg(x, y, m);
}

void modPow_recursion_dbg(int x, long y, int m) {
	printf("__MODPOW_RECURSION_DEBUG__\n");
	printf("%d^%d mod %d = %d\n\n", x, y, m, modPow_rec(x, y, m)); // x^y mod m = modPow
}