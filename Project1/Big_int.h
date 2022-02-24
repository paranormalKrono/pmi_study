#ifndef	_BIG_INT_H_
#define	_BIG_INT_H_

#include <stdbool.h>;

typedef struct big_int{
	unsigned int length;
	unsigned char* number;
	bool isNegative;
} big_int;

typedef enum arithmetic_operations 
{ 
	add, 
	subtruct,
	multiply,
	division
} arithmetic_operations;

big_int* bigint_create(const unsigned char* num);
void bigint_assign(big_int* b1, const big_int* b2);
bool bigint_compare(const big_int* b1, const big_int* b2);
big_int* bigint_zero();
void bigint_show(const big_int* num);
void bigint_arithmetic_operation(big_int* b1, const big_int* b2, const arithmetic_operations operation);
big_int* bigint_division(const big_int* left, const big_int* right, big_int* remainder);
void bigint_free(big_int* b);

#endif // _BIG_INT_H_