#include "MathPMI_Debug.h"

void bigint_dbg()
{
	printf("__BIG_INT_DEBUG__\n");

	unsigned long long iterations = 1e5;


	/*big_int *ex1 = bigint_construct(), *ex2 = bigint_construct();
	startTimer();
	printf("Время - %f секунд\n", getClock(clock1));
	for (unsigned long long i = 0; i < iterations; ++i)
	{
		bigint_assign(ex1, b1);
		bigint_assign(ex2, b2);


		bigint_arithmetic_operation(ex1, ex2, multiply);
	}
	stopTimer();
	printf("Время - %f секунд\n", getClock(clock2));
	printf("Время выполнения %f секунд\nитераций %d\n", (float)getTimerClock(), iterations);*/


	big_int* b1 = bigint_create(input_chars()); // Создание Big_int 1
	printf("B1 = ");
	bigint_show(b1);
	printf("\n");

	big_int* b2 = bigint_create(input_chars()); // Создание Big_int 2
	printf("B2 = ");
	bigint_show(b2);
	printf("\n");

	//printf("Сумма B1 и B2\n");
	printf("Разность B1 и B2 = ");
	//printf("Произведение B1 и B2\n");
	bigint_arithmetic_operation(b1, b2, subtruct);
	bigint_show(b1);

	/*big_int* remainder = bigint_zero();
	big_int* quotient = bigint_division(b1, b2, remainder);
	printf("Деление B1 и B2 = ");
	bigint_show(quotient);
	printf(" (остаток = ");
	bigint_show(remainder);
	printf(")");*/
	//printf("Произведение B1 и B2\n");
	//bigint_arithmetic_operation(b1, b2, subtruct);
	//bigint_show(b1);

	printf("\n");
}