#ifndef	_COMPLEX_NUMBERS_H_
#define	_COMPLEX_NUMBERS_H_

typedef struct complex_number {
	double real;
	double imaginary;
} complex_number;

complex_number* complex_create(double real, double imaginary);
void complex_square(complex_number* c); 

complex_number** get_complex_protos(complex_number* c, int count);

#endif // _COMPLEX_NUMBERS_H_