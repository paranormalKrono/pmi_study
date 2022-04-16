#include "MathPMI_Debug.h"
#define STRING_SIZE 100

void print_ints(char* format, int* a, int count)
{
	for (int i = 0; i < count; ++i)
		printf(format, a[i]);
}

int* input_nums(int* count)
{
	const int k = 0;

	printf("������� ���������� �����: ");
	scanf_s("%d", &k);

	// ��������� ������
	int* nums = (int*)malloc(k * sizeof(int));

	printf("������� ��������: ");

	// ���� ��������� �������
	for (int i = 0; i < k; i++)
	{
		scanf_s("%d", &nums[i]);
	}

	*count = k;

	return nums;
}

unsigned char* input_chars()
{
	unsigned char* input_text = (unsigned char*)malloc(sizeof(unsigned char) * STRING_SIZE); // ������ ����
	unsigned char input_symbol = '0';
	unsigned int text_length = 0; // ����� ������

	// ������������ ����������
	input_symbol = getchar();
	while (input_symbol != '\n')
	{
		input_text[text_length] = input_symbol;
		text_length++;
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
