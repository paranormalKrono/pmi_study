#ifndef	_MATHPMI_DEBUG_H_
#define	_MATHPMI_DEBUG_H_
#include "MathPMI.h"
#include <Windows.h>
#include "Time_Debug.h"
#include "Menu.h"
#include "Main.h"
#include "string.h"

typedef enum
{
	recursive,
	iterative
} algorithm_type;


void print_ints(char* format, int* a, int count);
int* input_nums(int* count);
unsigned char* input_chars();

void graphs_dbg();

void shunting_yard_dbg();

void poly_dbg();

void binarytrees_dbg();

void bigint_dbg();

double legendre_time(int iterations, int* legendre(long long, long long));
void legendre_symbol_input(int* legendre(long long, long long));
void legendre_symbol_dbg(long long p, long long a, int* legendre(long long, long long));

void chinese_reminder_input(algorithm_type op); 
void chinese_reminder_dbg(const int a[], const int m[], int k, algorithm_type op);

void linear_diophantine_input();
void linear_diophantine_dbg(int* a, int* x, int n, int d);

void extended_euclid_input();
void extended_euclid_dbg(int* a, int* b);

void euclid_recursion_input();
void euclid_recursion_dbg(int x, int y);

void modPow_recursion_input();
void modPow_recursion_dbg(int x, long y, int m);


#endif // _MATHPMI_DEBUG_H_