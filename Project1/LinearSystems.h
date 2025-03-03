#ifndef	__LINEAR_SYSTEMS_H__
#define	__LINEAR_SYSTEMS_H__
#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

#include "MainApp.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <imgui_stdlib.h>
#include <solvers.h>
#include <format>

enum IterationsType
{
	Gauss_Zeidel,
	Jacoby
};

void print_vector(const alglib::real_1d_array& X);
void print_matrix(const alglib::real_2d_array& A);
void print_matrix_and_vector(const alglib::real_2d_array& A, const alglib::real_1d_array& b, const  int& N);
void print_eigens(const alglib::real_2d_array& A, const int& N);

void CheckGilbert(const alglib::real_2d_array& G, const int& N, const double& alpha);

void multiply_vector_by_scalar(const alglib::real_1d_array& X, const int N, const double& scalar, alglib::real_1d_array& res);
void vector_add_vector(const alglib::real_1d_array& X, const alglib::real_1d_array& Y, const int& N, const double& coeff, alglib::real_1d_array& res);
void scalar_product(const alglib::real_1d_array& X, const alglib::real_1d_array& Y, const int& N, double& res);
void multiply_matrices(const alglib::real_2d_array& A, const alglib::real_2d_array& B, alglib::real_2d_array& C);
void multiply_matrix_by_vector(const alglib::real_2d_array& A, const alglib::real_1d_array& V, alglib::real_1d_array& C);

void matrix_row_multiplication(alglib::real_2d_array& A, const int& N, const int& r1, const double& coeff);
void matrix_add_row(alglib::real_2d_array& A, const int& N, const int& r1, const int& r2, const double& coeff);

alglib::real_1d_array random_vector(const int& N, const int& seed, const double& bound_A, const double& bound_B, const int double_digits_count);
double matrix_condition_number_by_eigenvalues(const alglib::real_2d_array& A, const int& N);
double vector_norm_2(const alglib::real_1d_array& X);
double vector_norm_max_abs(const alglib::real_1d_array& X);
double matrix_norm(const alglib::real_2d_array& A);
double matrix_condition_number(const alglib::real_2d_array& A, const alglib::real_2d_array& Ai, const int& N);
double matrix_determinant(const alglib::real_2d_array& A);

void matrix_identity(alglib::real_2d_array& A, const int& N);
void vector_identity(alglib::real_1d_array& V, const int& N);

alglib::real_2d_array SquareRootsMethod(const alglib::real_2d_array& A, const int& N);
alglib::real_1d_array CholeskyMethod(const alglib::real_2d_array& A, const alglib::real_1d_array& b, const int& N);
void GilbertMatrix();
void GaussMethod(const alglib::real_2d_array& tA, const alglib::real_1d_array& tb, alglib::real_1d_array& X, const int& N, double& determinant, alglib::real_2d_array& L, alglib::real_2d_array& U, alglib::real_2d_array& Ai);
void LUb(const alglib::real_2d_array& tA, alglib::real_1d_array& X, const int& N, alglib::real_2d_array& L, alglib::real_2d_array& U);
bool GetInversedMatrix(const alglib::real_2d_array& tA, alglib::real_2d_array& AI);
bool SimpleIterationMethod(const alglib::real_2d_array& A, const alglib::real_1d_array& b, alglib::real_1d_array& X, const IterationsType methodType = Gauss_Zeidel, const int& iterations_count = 10);
void GetJacobyMatrices(const alglib::real_2d_array& A, alglib::real_2d_array& M, alglib::real_2d_array& N);
void GetGaussZeidelMatrices(const alglib::real_2d_array& A, alglib::real_2d_array& M, alglib::real_2d_array& N);
void PassingMethod(const alglib::real_2d_array& A, const alglib::real_1d_array& d, const int& N, alglib::real_1d_array& X);
void PowerMethod(const alglib::real_2d_array& A, const int& N, alglib::real_1d_array& eigen_vector, double& eigen_value, double eps = 1.0E-12);
void RotationMethod(const alglib::real_2d_array& A, const int& N, alglib::real_2d_array& Q, alglib::real_2d_array& R);
void QRMethod(const alglib::real_2d_array& A, const int& N, const int& steps_count, alglib::real_1d_array& eigen_values);
void BackwardIterationsMethod(const alglib::real_2d_array& A, const int& N, const double& eigen_value, const int& steps_count, alglib::real_1d_array& eigen_vector);


#endif // __LINEAR_SYSTEMS_H__