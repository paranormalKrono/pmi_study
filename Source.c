#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "MathPMI_Examples.h"


void main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	//modPow_recursion_example();
	//euclid_recursion_example();
	//extended_euclid_example();
	//mul_inverse_example();
	//linear_diophantine_example();
	//chinese_reminder_example(iterative);
	/*legendre_symbol_example(legendre_symbol);
	legendre_symbol_example(legendre_symbol_2);*/

	//modPow_recursion_input();
	//euclid_recursion_input();
	//extended_euclid_input();
	//linear_diophantine_input();
	//chinese_reminder_input(iterative);
	//legendre_symbol_input(legendre_symbol);
	//legendre_symbol_input(legendre_symbol_2);

	//int iterations = 10e7;
	//double t1 = legendre_time(iterations, legendre_symbol);
	////legendre_time(iterations, legendre_symbol_2);
	//double t2 = legendre_time(iterations, legendre_symbol_3);
	//printf("Разница во времени выполнения: %f\n\n", (float)(t1 - t2));

	bigint_dbg();

	printf("__КОНЕЦ_ВЫПОЛНЕНИЯ__");
	scanf_s("%d");
}
