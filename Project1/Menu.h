#ifndef _MENU_H_
#define	_MENU_H_
#include "FilesWork.h"

typedef enum
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
} ConsoleColor;

void menu(); 
char* file_choice_menu(char* directory_path);
int choice_menu_h(const char* sections_text[], const int sections_count);

#endif // _MENU_H_