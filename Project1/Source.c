#include "menu.h"
#include "MathPMI_Examples.h"
#include "Small_exercises.h"

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 100;
	SetConsoleCursorInfo(h, &cci);

	//show_bracket_sequence();
	//show_max_value_segment();
	//show_repeated_int();
	//show_complex();
	//show_squares_intersection_area();
	menu();
	


	int b;
	scanf_s("%d", &b);
}
