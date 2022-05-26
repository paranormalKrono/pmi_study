#include "Main.h"

char* parse_from_to(const char** from, const char* to)
{
	char* result = (char*)malloc((to - *from + 1) * sizeof(char));
	if (result)
	{
		int i = 0;
		for (; *from != to; ++i, ++ * from) result[i] = **from;
		result[i] = '\0';

		return result;
	}
	else
	{
		return;
	}
}