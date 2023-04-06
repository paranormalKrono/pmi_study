#include <malloc.h>
#include <memory.h>
#include <math.h> 
#include "line_segment.h"
#include <stdio.h>


line_segment* line_segment_init(double A, double B)
{
	line_segment* ls = (line_segment*)malloc(sizeof(line_segment));
	ls->A = A;
	ls->B = B;
	return ls;
}

line_segment* line_segment_copy(line_segment* ls)
{
	return line_segment_init(ls->A, ls->B);
}

int segment_check_inside(const double x, const line_segment* ls) 
{
	return x >= ls->A && x <= ls->B;
}

void free_line_segments(line_segment** ls, int count)
{
	for (int i = 0; i < count; ++i)
	{
		free(ls[i]);
	}
	free(ls);
}


void print_segment(line_segment* ls)
{
	printf_s("[%.E, %.E]", ls->A, ls->B);
}
void print_segments(line_segment** ls, int count)
{
	for (int i = 0; i < count; ++i)
	{
		print_segment(ls[i]);
		printf_s("\n");
	}
}