#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include "Menu.h"
#include "MathPMI_Examples.h"

int choice_menu(HANDLE h, CONSOLE_CURSOR_INFO cci, const char* sections_text[], const int sections_count);
void clear_all();
void set_console_color(HANDLE h, ConsoleColor text, ConsoleColor background);

void menu() 
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 100;
	SetConsoleCursorInfo(h, &cci);

	unsigned char* answer;
	do
	{
		set_console_color(h, Green, Black);
		printf_s("Выберите подпрограмму из предложенных: ");

		const int start_menu_sections_count = 6;
		char* start_menu_texts[] =
		{
			"Графы",
			"Бинарные деревья",
			"Сортировочная станция",
			"Полиномы",
			"Big int",
			"..."
		};

		switch (choice_menu(h, cci, start_menu_texts, start_menu_sections_count))
		{
		case 0:
			graphs_dbg();
			break;
		case 1:
			binarytrees_dbg();
			break;
		case 2:
			shunting_yard_dbg();
			break;
		case 3:
			poly_dbg();
			break;
		case 4:
			bigint_dbg();
			break;
		case 5:
			break;
		}
		printf_s("%s", "Вы хотите продолжить? Да/...\n");
		answer = input_chars();
	} while (!strcmp(answer, "Да"));


	printf("__КОНЕЦ_ВЫПОЛНЕНИЯ__\n");
}



char* file_choice_menu(char* directory_path)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	_chdir(directory_path);
	int files_count;
	char** files_names = get_files_names(&files_count);

	return files_names[choice_menu_h(files_names, files_count)];
}



int choice_menu_h(const char* sections_text[], const int sections_count) 
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 100;
	SetConsoleCursorInfo(h, &cci);
	return choice_menu(h, cci, sections_text, sections_count);
}

COORD get_point(int x, int y);
void change_cursor_position(HANDLE hConsoleOutput, COORD* from, COORD to);
void print_cursor_at(HANDLE hConsoleOutput, COORD* point);

int choice_menu(HANDLE h, CONSOLE_CURSOR_INFO cci,
	const char* sections_text[], const int sections_count)
{
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(h, &bi);
	COORD point = bi.dwCursorPosition;

	int start_Ypos = point.Y + 1;
	for (int i = 0; i < sections_count; ++i)
	{
		point = get_point(0, point.Y + 1);
		SetConsoleCursorPosition(h, point);
		printf_s("  %s", (sections_text[i]));

		GetConsoleScreenBufferInfo(h, &bi);
		point = bi.dwCursorPosition;
	}
	int end_Ypos = point.Y;

	point = get_point(0, start_Ypos);
	print_cursor_at(h, &point);
	while (1)
	{
		int move = _getch();
		switch (move)
		{
		case 72: // Up
			if (point.Y > start_Ypos)
				change_cursor_position(h, &point, get_point(0, point.Y - 1));
			break;
		case 80: // Down
			if (point.Y < end_Ypos)
				change_cursor_position(h, &point, get_point(0, point.Y + 1));
			break;
		case 13: // Enter
			SetConsoleCursorPosition(h, get_point(0, end_Ypos+2));
			return point.Y - start_Ypos;
		}
	}
}

void clear_all() 
{
	system("cls");
}

void change_cursor_position(HANDLE hConsoleOutput, COORD *from, COORD to)
{
	printf_s(" ");
	SetConsoleCursorPosition(hConsoleOutput, *from);
	*from = to;
	print_cursor_at(hConsoleOutput, from);
}

void print_cursor_at(HANDLE hConsoleOutput, COORD *point) 
{
	SetConsoleCursorPosition(hConsoleOutput, *point);
	printf_s(">");
	SetConsoleCursorPosition(hConsoleOutput, *point);
}

COORD get_point(int x, int y) 
{
	COORD point;
	point.X = x;
	point.Y = y;
	return point;
}

void set_console_color(HANDLE h, ConsoleColor text, ConsoleColor background)
{
	SetConsoleTextAttribute(h, (WORD)((background << 4) | text));
}

//CONSOLE_SCREEN_BUFFER_INFO bi;
//GetConsoleScreenBufferInfo(h, &bi);
//COORD point = bi.dwCursorPosition;
// 
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