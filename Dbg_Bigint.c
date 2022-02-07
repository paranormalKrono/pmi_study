#include "MathPMI_Debug.h"
#define STRING_SIZE 100

unsigned char* input_chars();

void bigint_dbg()
{
	printf("__BIG_INT_INPUT__\n");

	unsigned long long iterations = 1e5;


	/*big_int *ex1 = bigint_construct(), *ex2 = bigint_construct();
	startTimer();
	printf("����� - %f ������\n", getClock(clock1));
	for (unsigned long long i = 0; i < iterations; ++i)
	{
		bigint_assign(ex1, b1);
		bigint_assign(ex2, b2);


		bigint_arithmetic_operation(ex1, ex2, multiply);
	}
	stopTimer();
	printf("����� - %f ������\n", getClock(clock2));
	printf("����� ���������� %f ������\n�������� %d\n", (float)getTimerClock(), iterations);*/


	big_int* b1 = bigint_create(input_chars()); // �������� Big_int 1
	printf("B1 = ");
	bigint_show(b1);
	printf("\n");

	big_int* b2 = bigint_create(input_chars()); // �������� Big_int 2
	printf("B2 = ");
	bigint_show(b2);
	printf("\n");

	//printf("����� B1 � B2\n");
	printf("�������� B1 � B2 = ");
	//printf("������������ B1 � B2\n");
	bigint_arithmetic_operation(b1, b2, subtruct);
	bigint_show(b1);

	/*big_int* remainder = bigint_zero();
	big_int* quotient = bigint_division(b1, b2, remainder);
	printf("������� B1 � B2 = ");
	bigint_show(quotient);
	printf(" (������� = ");
	bigint_show(remainder);
	printf(")");*/
	//printf("������������ B1 � B2\n");
	//bigint_arithmetic_operation(b1, b2, subtruct);
	//bigint_show(b1);

	printf("\n");
}

unsigned char* input_chars()
{
	unsigned char* input_text = (unsigned char*)malloc(sizeof(unsigned char) * STRING_SIZE); // ������ ����
	unsigned char input_symbol = '0';
	unsigned int text_length = 0; // ����� ������

	// ������������ ���������� ��������
	input_symbol = getchar();
	while (input_symbol != '\n')
	{
		input_text[text_length] = input_symbol;
		text_length += 1;
		input_symbol = getchar();
		if (!(text_length & STRING_SIZE))
		{
			input_text = (unsigned char*)realloc(input_text, (text_length + STRING_SIZE) * sizeof(unsigned char));
		}
	}

	// �������������� � ������
	input_text = (unsigned char*)realloc(input_text, (text_length + 1) * sizeof(unsigned char));
	input_text[text_length] = '\0';
	return input_text;
}