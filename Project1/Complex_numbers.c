#include "Complex_numbers.h"
#include <malloc.h>
#include <math.h>

#define M_PI 3.14159265358979323846

complex_number* complex_create(double real, double imaginary)
{
	complex_number* c = (complex_number*)malloc(sizeof(complex_number));
	if (c) 
	{
		c->real = real;
		c->imaginary = imaginary;
	}
	return c;
}

void complex_square(complex_number* c) // Возведение в квадрат
{
	double r = c->real;
	c->real = c->real * c->real - c->imaginary * c->imaginary;
	c->imaginary = pow(r * c->imaginary, 2);
}

complex_number** get_complex_protos(complex_number* c, int count)
{
	double radius = sqrt(pow(c->real, 2) + pow(c->imaginary, 2));
	double new_radius = pow(radius, 1/(double)count);
	double angle = atan(c->imaginary / c->real);
	if (c->imaginary < 0) angle = -angle;
	double new_angle;

	complex_number** cnews = (complex_number*)malloc(sizeof(complex_number) * count);
	for (int k = 0; k < count; ++k) 
	{
		new_angle = (angle + 2 * k * M_PI) / 2;
		cnews[k] = complex_create(new_radius * cos(new_angle), new_radius * sin(new_angle));
	}

	return cnews;
}