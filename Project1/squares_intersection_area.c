#include "Small_exercises.h"
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

typedef struct square
{
	float centerX;
	float centerY;
	float length;
} square;

void init_square(square s, float center, float a, float b) 
{
	s.centerX = center;
	s.centerY = a;
	s.length = b;
}

float get_square_area(square s)
{
	return s.length * s.length;
}


float get_intersection_area(square s1, square s2) 
{
	float area = 0;

	float a = fabs(s1.centerX - s2.centerX) - fabs(s1.length + s2.length) / 2;
	float b = fabs(s1.centerY - s2.centerY) - fabs(s1.length + s2.length) / 2;

	if (a > 0 || b > 0) return 0;
	else 
	{
		a = fabs(a);
		b = fabs(b);
	}

	if (a > s1.length) a = s1.length;
	if (b > s1.length) b = s1.length;

	return a * b;
}

void show_squares_intersection_area() 
{
	int count = 4;
	square squares[4] = { {1,1,2}, {1,2,2}, {3, 3, 4}, {0, 0, 5} };
	
	float area = 0;
	square s1;

	for (int i = 0; i < count; ++i) 
	{
		s1 = squares[i];
		printf_s("square %d = (%.2f, %.2f, %.2f)\n", i, s1.centerX, s1.centerY, s1.length);

		for (int j = i + 1; j < count; ++j)
		{
			area += get_square_area(squares[i]) - get_intersection_area(s1, squares[j]);
			printf_s("area %d = %.2f\n", i, area);
		}
	}

	printf_s("Итоговая площадь = %f", area);
}