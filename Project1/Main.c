#include "Main.h"
#include <malloc.h>
#include <memory.h>

char* parse_from_to(const char** from, const char* to)
{
	char* result = (char*)malloc((to - *from + 1) * sizeof(char));
	if (!result) return NULL;

	int i = 0;
	for (; *from != to; ++i, ++ * from) result[i] = **from;
	result[i] = '\0';

	return result;
}