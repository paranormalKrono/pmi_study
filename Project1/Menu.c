#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include <direct.h>
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
	cci.bVisible = true;
	cci.dwSize = 100;
	SetConsoleCursorInfo(h, &cci);

	char answer;
	do
	{
		answer = '0';
		set_console_color(h, White, Black);
		printf_s("Выберите подпрограмму из предложенных: ");

		const int start_menu_sections_count = 7;
		char* start_menu_texts[] =
		{
			"Вычислительная математика",
			"Графы",
			"Бинарные деревья",
			"Сортировочная станция",
			"Полиномы",
			"Big int",
			"..."
		};
		const void (*methods[])() = {
			computational_mathematics_dbg,
			graphs_dbg,
			binarytrees_dbg,
			shunting_yard_dbg,
			poly_dbg,
			bigint_dbg
		};
		int choice = choice_menu(h, cci, start_menu_texts, start_menu_sections_count);
		methods[choice]();

		printf_s("%s", "Вы хотите продолжить? Y/N (Да/Нет)\n");
		//answer = input_chars();
		while (answer != 'Y' && answer != 'N') 
			scanf_s("%c", &answer, 1);
	} while (answer == 'Y');


	printf("__КОНЕЦ_ВЫПОЛНЕНИЯ__\n");
}



char* file_choice_menu(char* directory_path)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int res = _chdir(directory_path);
	int files_count;
	WIN32_FIND_DATA* datas;
	char** files_names;
	char* choosed;
	char* new_directory;
	int choice = 0;
	bool isChoosed = false;
	do
	{
		datas = get_files(&files_count);
		files_names = get_files_names(files_count, datas);
		choice = choice_menu_h(files_names, files_count);
		if (datas[choice].dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			new_directory = (char*)malloc(sizeof(char) * (strlen(datas[choice].cFileName) + 2));
			if (!new_directory) return;
			snprintf(new_directory, strlen(datas[choice].cFileName) + 2, "%s/", datas[choice].cFileName);
			directory_path = new_directory;
			free(datas);
			free(files_names);
			res = _chdir(directory_path);
		}
		else
		{
			isChoosed = true;
			choosed = files_names[choice];
			free(datas);
			free(files_names);
			return choosed;
		}
	} while (!isChoosed);

}



int choice_menu_h(const char* sections_text[], const int sections_count) 
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = true;
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
			else
				change_cursor_position(h, &point, get_point(0, end_Ypos));
			break;
		case 80: // Down
			if (point.Y < end_Ypos)
				change_cursor_position(h, &point, get_point(0, point.Y + 1));
			else
				change_cursor_position(h, &point, get_point(0, start_Ypos));
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
