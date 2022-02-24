#include "MathPMI_Debug.h"
#define STRING_SIZE 100

unsigned char* input_chars();

void turingmachine_dbg()
{
	printf("__BIG_INT_INPUT__\n");

	printf("\n");
}
//
//unsigned char* input_chars()
//{
//	unsigned char* input_text = (unsigned char*)malloc(sizeof(unsigned char) * STRING_SIZE); // массив цифр
//	unsigned char input_symbol = '0';
//	unsigned int text_length = 0; // длина текста
//
//	// Посимвольное считывание символов
//	input_symbol = getchar();
//	while (input_symbol != '\n')
//	{
//		input_text[text_length] = input_symbol;
//		text_length += 1;
//		input_symbol = getchar();
//		if (!(text_length & STRING_SIZE))
//		{
//			input_text = (unsigned char*)realloc(input_text, (text_length + STRING_SIZE) * sizeof(unsigned char));
//		}
//	}
//
//	// Преобразование в строку
//	input_text = (unsigned char*)realloc(input_text, (text_length + 1) * sizeof(unsigned char));
//	input_text[text_length] = '\0';
//	return input_text;
//}