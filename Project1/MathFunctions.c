#include "MathFunctions.h"
#include <string.h>

int get_mathfunction_index(char* name)
{
	for (int i = 0; i < FUNCTIONS_COUNT; ++i)
	{
		if (strcmp(name, math_functs[i].name) == 0) return i;
	}
	return -1;
}

int get_function_result(math_function mf, int* parameters)
{
	int (*mff)(int*) = mf.func;
	switch (mf.parameters_count)
	{
	case 1:
		return mff(parameters[0]);
	case 2:
		return mff(parameters[0], parameters[1]);
	default:
		break;
	}
}