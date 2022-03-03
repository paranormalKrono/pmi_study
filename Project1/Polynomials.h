#ifndef	_POLYNOMIALS_H_
#define	_POLYNOMIALS_H_

typedef struct poly {
	int coeff;
	int exp;
	struct poly* next;
} poly;

poly* poly_parse(const char* str);
poly* poly_monomial(int coeff, int exp);
void poly_show(const poly* p);
char* poly_tostring(poly* p);
void poly_addition(const poly** left, const poly* right);
poly* poly_multiplication(const poly* left, const poly* right);
void poly_free(poly* pol);

#endif // _POLYNOMIALS_H_