#include "DFA.h"

bool dfa_check_binary_sum(const char* x, const char* y, const char* z, const int count) 
{
	int c = 0;
	for (int i = 0; i < count; ++i)
	{
		c += x[i] + y[i] - ('0' * 2);
		switch (c) 
		{
		case 1:
			if (z[i] != 1) 
			{
				return false;
			}
			break;
		case 2:

		default:
			break;
		}
	}
}