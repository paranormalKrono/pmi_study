#ifndef	_NUMERICAL_INTEGRATION_H_
#define	_NUMERICAL_INTEGRATION_H_
#pragma once

#include "Shunting_yard.h"
#include <malloc.h>

// ¿—“ = 0
#define integration_left_rectangle(l, fA) l * fA
// ¿—“ = 0
#define integration_right_rectangle(l, fB) l * fB


// ¿—“ = 1
#define integration_middle_rectangle(l, fAB2) l * fAB2
// ¿—“ = 1
#define integration_trapezoid(l, fA, fB) l * 0.5 * (fA+fB)

// ¿—“ = 3
#define integration_simpsons(l, fA, fAB2, fB) l / 6.0 * (fA + 4 * fAB2 + fB)
// ¿—“ = 3
#define integration_38(l, fA0, fA1B, fA2B, fB3) l / 8.0 * (fA0 + 3 * (fA1B + fA2B) + fB3)

#define integration_left_theoretical_error(l2, fd1) 0.5 * l2 * fd1
#define integration_right_theoretical_error(l2, fd1) 0.5 * l2 * fd1
#define integration_middle_theoretical_error(l3, fd2) l3 / 24.0 * fd2
#define integration_trapezoid_theoretical_error(l3, fd2) l3 / 12.0 * fd2
#define integration_simpsons_theoretical_error(l5, fd4) l5 / 2880.0 * fd4
#define integration_38_theoretical_error(l5, fd4) l5 / 80.0 * fd4



//compound
// ¿—“ = 0
#define integration_compound_left_rectangle(h, W, f0) h * (f0 + W) 
// ¿—“ = 0
#define integration_compound_right_rectangle(h, W, fm) h * (fm + W)

// ¿—“ = 1
#define integration_compound_middle_rectangle(h, Q) h * Q
// ¿—“ = 1
#define integration_compound_trapezoid(h, W, f0, fm) h * 0.5 * (f0 + 2 * W +fm)

// ¿—“ = 3
#define integration_compound_simpsons(h, W, Q, f0, fm) h / 6.0 * (f0 + 2 * W + 4 * Q + fm)
// ¿—“ = 3
//#define integration_compound_38(h, W, fA1B, fA2B, fB3) h / 8.0 * (fA0 + 3 * (fA1B + fA2B) + fB3)

#define integration_compound_left_theoretical_error(l2, m, fd1)  l2 * 0.5 / (double)m * fd1
#define integration_compound_right_theoretical_error(l2, m, fd1)  l2 * 0.5 / (double)m * fd1
#define integration_compound_middle_theoretical_error(l3, m2, fd2) l3 / 24.0 / (double)m2 * fd2
#define integration_compound_trapezoid_theoretical_error(l3, m2, fd2) l3 / 12.0 / (double)m2 * fd2
#define integration_compound_simpsons_theoretical_error(l5, m4, fd4) l5 / 2880.0 / (double)m4 * fd4
//#define integration_38_theoretical_error(l5, fd4) -l5 / 80.0 * fd4

double get_integration_compound_middle_rectangle(const queue* function, variable** vars, int var_id, int vars_count, double A, double B, int divisions);
double get_integration_compound_simpsons(const queue* function, variable** vars, int var_id, int vars_count, double A, double B, int divisions);

#endif // _NUMERICAL_INTEGRATION_H_