
#include <Python.h>

#include "menu.h"
#include "MathPMI_Examples.h"
#include "Small_exercises.h"

#include "python_gui.h"
#include "Burnside.h"


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

	printf_s("@paranormalKrono\n\n");

	//burnside_dbg();
	//create_window();
	menu();
	
	int kk;
	scanf_s("%d", &kk);
}
