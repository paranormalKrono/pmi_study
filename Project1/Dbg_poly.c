#include "MathPMI_Debug.h";
#include <string.h>

void poly_dbg()
{
	printf("������� ��������: \n");
	//poly* p = poly_get(input_chars());

	//poly_show(p);
	// 
	poly* p1 = poly_parse("15+10x^10+5x^2-25");
	printf("������� 1: %s\n", poly_tostring(p1));

	poly* p2 = poly_parse("5x^2+3x+10x+20-10-10x^2+3x^3-4x");
	printf("������� 2: %s\n", poly_tostring(p2));

	//poly_addition(&p1, p2);
	//printf("����� ���������: %s\n", poly_tostring(p1));

	p1 = poly_multiplication(p1, p2);
	printf("������������ ���������: %s\n", poly_tostring(p1));

	//printf("\n%s\n", poly_tostring(p));

	//free(p);
}