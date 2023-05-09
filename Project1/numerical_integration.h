#ifndef	_NUMERICAL_INTEGRATION_H_
#define	_NUMERICAL_INTEGRATION_H_

// ÀÑÒ = 0
#define integration_left_rectangle(l, fA) l * fA
// ÀÑÒ = 0
#define integration_right_rectangle(l, fB) l * fB

#define integration_left_absolute_error(l2, fd1) -0.5 * l2 * fd1
#define integration_right_absolute_error(l2, fd1) 0.5 * l2 * fd1

// ÀÑÒ = 1
#define integration_middle_rectangle(l, fAB2) l * fAB2
// ÀÑÒ = 1
#define integration_trapezoid(l, fA, fB) l * 0.5 * (fA+fB)

#define integration_middle_absolute_error(l3, fd2) l3 / 24.0 * fd2
#define integration_trapezoid_absolute_error(l3, fd2) -l3 / 12.0 * fd2

// ÀÑÒ = 3
#define integration_simpsons(l, fA, fAB2, fB) l / 6.0 * (fA + 4 * fAB2 + fB)
// ÀÑÒ = 3
#define integration_38(l, fA0, fA1B, fA2B, fB3) l / 8.0 * (fA0 + 3 * (fA1B + fA2B) + fB3)

#define integration_simpsons_absolute_error(l5, fd4) l5 / 2880.0 * fd4
#define integration_38_absolute_error(l5, fd4) -l5 / 80.0 * fd4


//compound


 
#endif // _NUMERICAL_INTEGRATION_H_