#include "MathPMI_Debug.h"

void euclid_recursion_input() {
	printf("__EUCLID_RECURSION_INPUT__\n");

	int x = 0, y = 0;
	printf("¬ведите x и y: ");
	scanf_s("%d %d", &x, &y);
	euclid_recursion_dbg(x, y);
}

void euclid_recursion_dbg(int x, int y) {
	printf("__EUCLID_RECURSION_DEBUG__\n");
	printf("gcd(%d,%d) = %d\n\n", x, y, euclid_rec(x, y)); // gcd(x, y) = d
}