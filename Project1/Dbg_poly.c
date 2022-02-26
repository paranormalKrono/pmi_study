#include "MathPMI_Debug.h";
#include <string.h>

void poly_dbg()
{
	printf("¬ведите полином: ");
	poly* p = poly_get(input_chars());

	poly_show(p);

	//printf("\n%s\n", poly_tostring(p));

	free(p);
}