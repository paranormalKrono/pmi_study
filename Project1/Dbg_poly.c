#include "MathPMI_Debug.h";
#include <string.h>

poly* process_poly();
void poly_dbg()
{
	printf("Введите полиномы: \n");
	printf(">");
	char* ic = input_chars();
	char* exc;
	poly* p;
	while (*ic != '\0')
	{
		exc = "";
		p = poly_parse(ic, &exc);
		if (exc)
		{
			while (ic != exc)
			{
				printf("~");
				ic++;
			}
			printf("~^\n");
		}
		printf(">%s", poly_tostring(p));
		poly_free(p);
		printf("\n>");
		ic = input_chars();
	}
	//poly* p =  poly_parse(input_chars());

	//poly_show(p);
	// 
	/*poly* p1 = poly_parse("15+10x^10+5x^2-25");
	printf("Полином 1: %s\n", poly_tostring(p1));

	poly* p2 = poly_parse("5x^2+3x+10x+20-10-10x^2+3x^3-4x");
	printf("Полином 2: %s\n", poly_tostring(p2));*/

	printf("Полином 1:\n");
	poly* p1 = process_poly();

	printf("Полином 2:\n");
	poly* p2 = process_poly();

	//poly_addition(&p1, p2);
	//printf("Сумма полиномов: %s\n", poly_tostring(p1));

	p1 = poly_multiplication(p1, p2);
	printf("Произведение полиномов: %s\n", poly_tostring(p1));

	free(p1);
	free(p2);
}

poly* process_poly() 
{
	printf(">");
	char* ic = input_chars();
	char* exc = "";
	poly* p = poly_parse(ic, &exc);
	if (ic)
	{
		if (exc)
		{
			while (ic != exc)
			{
				printf("~");
				ic++;
			}
			printf("~^\n");
		}
		printf(">%s\n", poly_tostring(p));
		return p;
	}
	return NULL;
}