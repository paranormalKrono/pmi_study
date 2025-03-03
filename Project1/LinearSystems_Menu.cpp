#include "LinearSystems.h"

#pragma execution_character_set("utf-8")

void LinearSystemsMenu()
{
	static alglib::real_1d_array tb(
		"[15546.158899, 14474.276214, 9602.209006, -7405.154103, 4611.267202]");

	static alglib::real_2d_array tA(
		"[[16.900816, -88.835622, 40.052833, -33.677305, -68.046773],\
			[-12.776071, -96.034432, -44.420625, -35.869432, -98.904174],\
			[-4.589671, -7.759125, 85.282942, 12.967231, -88.341727],\
			[-91.772491, -3.359201, 48.018954, 55.241062, 91.341082],\
			[2.026572, -70.356570, 5.382529, -33.074142, 44.125335]]");


	static alglib::real_1d_array bsym(
		"[-1494.969243, 87.849151, 1846.587445, 957.542922, -591.443503]");

	static alglib::real_2d_array Asym(
		"[[247.910139, 77.840524, -169.061201, -216.832256, 23.413512],\
				[77.840524, 154.219143, -3.588624, -95.688854, 20.020289],\
				[-169.061201, -3.588624, 193.982023, 157.190566, -66.508107],\
				[-216.832256, -95.688854, 157.190566, 258.264672, -51.575307],\
				[23.413512, 20.020289, -66.508107, -51.575307, 58.501461]]");


	static alglib::real_1d_array d(
		"[8256.726494,6655.641291,1736.160133,1117.670187,-2978.507541]");

	static alglib::real_2d_array A3diag(
		"[[-89.960035,	-12.803070,	0.000000,	0.000000,	0.000000],\
				[-78.063780,	85.640310,	-64.292650,	0.000000,	0.000000],\
				[0.000000,	-7.202268,	73.781222,	-25.019314,	0.000000],\
				[0.000000,	0.000000,	-12.407601,	-8.799258,	30.155976],\
				[0.000000,	0.000000,	0.000000,	26.949476,	-88.480170]]");

	static const int N = 5;

	static alglib::real_2d_array L, U, Ai;
	static double determinant = 1;

	alglib::real_2d_array Y("[[1, 2, 3],[4,5,6],[7,8,9]]");
	if (ImGui::Button("Вычислить определитель"))
	{
		std::cout << matrix_determinant(Y);
	}

	if (ImGui::Button("Метод простых итераций"))
	{
		alglib::real_1d_array X;
		vector_identity(X, N);

		SimpleIterationMethod(tA, tb, X, Gauss_Zeidel, 10);

		print_vector(X);

		GaussMethod(tA, tb, X, N, determinant, L, U, Ai);
	}

	if (ImGui::Button("QR алгоритм с методом обратных итераций"))
	{
		alglib::real_1d_array eigen_values;

		std::cout << std::format("\n\nMatrix A\n");
		print_matrix(A3diag);
		QRMethod(A3diag, N, 30, eigen_values);

		std::cout << std::format("\n\nEigenvalues (only real)\n");
		for (int i = 0; i < N; ++i) {
			std::cout << std::format("{:>22}+{:>22}i\n",
				eigen_values[i], 0);
		}

		alglib::real_1d_array eigen_vector;
		BackwardIterationsMethod(A3diag, N, eigen_values[0], 8, eigen_vector);
		std::cout << "Eigen vector by Backward iterations method\n";
		print_vector(eigen_vector);

		// Проверка с помощью библиотеки
		print_eigens(A3diag, N);

	}

	if (ImGui::Button("Модифицированный степенной метод"))
	{
		alglib::real_1d_array eigen_vector;
		double eigen_value = 0;

		std::cout << std::format("\n\nMatrix A\n");
		print_matrix(Asym);
		PowerMethod(Asym, N, eigen_vector, eigen_value);

		std::cout << std::format("Eigenvalue (only real) = {0}\n", eigen_value);
		std::cout << std::format("Eigenvector\n");
		print_vector(eigen_vector);

		// Проверка с помощью библиотеки
		print_eigens(Asym, N);

	}


	if (ImGui::Button("Решить трёхдиагональную матрицу методом прогонки")) 
	{
		alglib::real_1d_array X;
		PassingMethod(A3diag, d, N, X);
		std::cout << "Matrix A and vector d" << "\n";
		print_matrix_and_vector(A3diag, d, N);
		std::cout << "Vector X founded by an algorithm" << "\n";
		print_vector(X);
		std::cout << "Check by using Gauss method\n";
		
		double det;
		alglib::real_2d_array L, U, Ai;
		GaussMethod(A3diag, d, X, N, det, L, U, Ai);
	}

	if (ImGui::Button("Решить по методу Холецкого"))
	{
		alglib::real_1d_array X;

		X = CholeskyMethod(Asym, bsym, N);

	}

	if (ImGui::TreeNode("Матрица Гильберта"))
	{
		GilbertMatrix();
		ImGui::TreePop();
	}

	static bool isCalculated;
	if (ImGui::Button("Обновить матрицы"))
	{
		alglib::real_1d_array X;
		Ai.setlength(N, N);
		X.setlength(N);


		GaussMethod(tA, tb, X, N, determinant, L, U, Ai);

		// Поиск числа обусловленности
		double mu = matrix_condition_number(tA, Ai, N);

		std::cout << std::format("Matrix condition number {:>22}\n", mu);

		isCalculated = true;
	}

	static int seed{ (int)time(NULL) / 2 };
	ImGui::InputInt("Сид для рандомной генерации вектора x", &seed, 1, 5);
	static float bA{ 30 }, bB{ 90 };
	ImGui::DragFloatRange2("Промежуток", &bA, &bB, 0.1f, -FLT_MAX, FLT_MAX, "A: %.4f", "B: %.4f", ImGuiSliderFlags_AlwaysClamp);
	static int double_digits_count{ 8 };
	ImGui::DragInt("Количество знаков после запятой", &double_digits_count, 1, 0, 12);

	if (isCalculated)
	{
		if (ImGui::Button("Найти b через LU-декомпозицию с решением x"))
		{
			// Создаём вещественный произвольный вектор X с ограничениями
			alglib::real_1d_array X = random_vector(N, seed, bA, bB, double_digits_count);

			// Находим b
			LUb(tA, X, N, L, U);
			alglib::real_1d_array b(X);

			// Проверим ответ по Гауссу
			alglib::real_2d_array Ai;
			Ai.setlength(N, N);
			GaussMethod(tA, b, X, N, determinant, L, U, Ai);
		}

	}
}

void BackwardIterationsMethod(const alglib::real_2d_array& A, const int& N, 
	const double& eigen_value, const int& steps_count, alglib::real_1d_array & eigen_vector)
{
	eigen_vector.setlength(N);

	alglib::real_2d_array M;
	alglib::real_1d_array Y;
	M.setlength(N, N);
	Y.setlength(N);

	// M - характеристичекая матрица
	alglib::rmatrixcopy(N, N, A, 0, 0, M, 0, 0);
	for (int i = 0; i < N; ++i)
		M[i][i] -= eigen_value;

	for (int i = 0; i < N; ++i)
		eigen_vector[i] = 1;

	alglib::real_2d_array L, U, Ai;
	double det;

	double norm;
	for (int i = 0; i < steps_count; ++i) 
	{
		// Решаем систему вида My = x по Гауссу
		GaussMethod(M, eigen_vector, Y, N, det, L, U, Ai);
		norm = vector_norm_2(Y);
		multiply_vector_by_scalar(Y, N, 1. / norm, eigen_vector);
		print_vector(eigen_vector);
	}
}

void RotationMethod(const alglib::real_2d_array& A, const int& N, alglib::real_2d_array& Q, 
	alglib::real_2d_array& R)
{
	R.setlength(N, N);
	matrix_identity(Q, N);

	alglib::rmatrixcopy(N, N, A, 0, 0, R, 0, 0);
	
	double a1, a2, l, c, s;
	
	// Выполняем алгоритм вращений для разложения A = Q * R = Tt * TA
	for (int i = 0; i < N - 1; ++i)
	{
		for (int j = i + 1; j < N; ++j) 
		{
			if (R[j][i] == 0)
				continue;
			
			c = R[i][i];
			s = R[j][i];
			l = sqrt(pow(c, 2) + pow(s, 2));
			c /= l;
			s /= l;

			//std::cout << std::format("c^2 + s^2 = {0}\n",pow(c,2)+pow(s,2));
			
			// Домножим на матрицу элементарного преобразования. То есть T на A
			for (int column = i; column < N; ++column) 
			{
				a1 = R[i][column];
				a2 = R[j][column];

				R[i][column] = c * a1 + s * a2;
				R[j][column] = -s * a1 + c * a2;
			}
			//print_matrix(R, N);

			// Перемножим T и транспонированное Tk слева направо по-хитрому
			for (int row = 0; row < N; ++row) 
			{
				a1 = Q[row][i];
				a2 = Q[row][j];

				Q[row][i] = c * a1 + s * a2;
				Q[row][j] = - s * a1 + c * a2;
			}
			//print_matrix(Q, N);

		}
	}
}

void QRMethod(const alglib::real_2d_array& A, const int& N, const int& steps_count, 
	alglib::real_1d_array& eigen_values)
{	
	eigen_values.setlength(N);

	alglib::real_2d_array Ak, Q, R;
	Ak.setlength(N, N);
	alglib::rmatrixcopy(N, N, A, 0, 0, Ak, 0, 0);

	for (int k = 0; k < steps_count; ++k) 
	{
		RotationMethod(Ak, N, Q, R);

		// Вычисляем Ak = RQ
		multiply_matrices(R, Q, Ak);
	}

	for (int i = 0; i < N; ++i) {
		eigen_values[i] = Ak[i][i];
	}

}

void PowerMethod(const alglib::real_2d_array& A, const int& N, 
	alglib::real_1d_array& eigen_vector, double& eigen_value, double eps)
	{
		alglib::real_1d_array Xprev, tmp, Y;
		double norm1 = 0;
		double norm2 = 0;

		eigen_vector.setlength(N);
		Xprev.setlength(N);
		tmp.setlength(N);

		for (int i = 0; i < N; ++i) {
			Xprev[i] = 0;
		}
		Xprev[0] = 1;


		// Выполнение метода с определённой точностью
		double error = DBL_MAX;
		int step_m = 100;
		while (error > eps && (--step_m, step_m > 0)) 
		{
			// шаг алгоритма
			multiply_matrix_by_vector(A, Xprev, Y);


			scalar_product(Y, Xprev, N, eigen_value);
			norm1 = vector_norm_2(Y);
			multiply_vector_by_scalar(Y, N, 1. / norm1, eigen_vector);

			/*std::cout << std::format("\n\nVector Xprev\n");
			print_vector(Xprev, N);
			std::cout << std::format("\n\nVector Y\n");
			print_vector(Y, N);
			std::cout << std::format("\nEigen value = {0}\n", eigen_value);
			std::cout << std::format("Vector Eigen\n");
			print_vector(eigen_vector, N);*/

			// Вычисление погрешности
			vector_add_vector(eigen_vector, Xprev, N, -eigen_value, tmp);
			norm1 = vector_norm_2(tmp);
			norm2 = vector_norm_2(Xprev);
			error = norm1 / norm2;
			std::cout << std::format("\nError value = {0} / {1} = {2}\n",norm1, norm2, error);

			// Перестановка векторов
			alglib::rvectorcopy(N, eigen_vector, 0, Xprev, 0);
		}
	}

// X0 should be chosen before
bool SimpleIterationMethod(const alglib::real_2d_array& A, const alglib::real_1d_array& b, 
	alglib::real_1d_array& X, const IterationsType methodType, 
	const int& iterations_count)
{
	int n = A.cols();
	if (n != A.rows()) return false;
	alglib::real_2d_array M, N;

	switch (methodType)
	{
	case Gauss_Zeidel: GetGaussZeidelMatrices(A, M, N); break;
	case Jacoby: GetJacobyMatrices(A, M, N); break;
	default:
		break;
	}

	// printf_s("AX=b"); print_matrix_and_vector(A, b, n); print_vector(X);
	// printf_s("Matrix M\n"); print_matrix(M);
	// printf_s("Matrix N\n"); print_matrix(N);

	double actualEstimate, X0norm = vector_norm_max_abs(X), Hnorm, gnorm, t1, t2;
	alglib::real_1d_array Xnew, Xprev, raz, g;
	alglib::real_2d_array MI, H;

	Xprev.setlength(n); raz.setlength(n);

	GetInversedMatrix(M, MI);

	multiply_matrix_by_vector(MI, b, g); gnorm = vector_norm_2(g);

	multiply_matrices(MI, N, H); Hnorm = matrix_norm(H);

	alglib::real_1d_array sol; sol.setlength(n); 
	alglib::rvectorcopy(n, b, 0, sol, 0); alglib::rmatrixsolvefast(A, sol);
	printf_s("Actual solution\n"); print_vector(sol);

	// printf_s("Matrix M^(-1)\n"); print_matrix(MI);
	// printf_s("Iterations start\n");

	for (int i = 0; i < iterations_count; ++i)
	{
		alglib::rvectorcopy(n, X, 0, Xprev, 0);
		printf_s("----------------\n");

		// x(k+1)=M^(-1) (b-Nx(k))
		multiply_matrix_by_vector(N, X, Xnew);

		// printf_s("NX\n"); print_vector(NewX);

		vector_add_vector(b, Xnew, n, -1., X);
		// printf_s("b - NX\n"); print_vector(X);

		multiply_matrix_by_vector(MI, X, Xnew);
		alglib::rvectorcopy(n, Xnew, 0, X, 0);

		// printf_s("M^(-1)(b - NX)\n"); print_vector(X);
		// printf_s("----------------\n");

		printf_s("Actual, priori and posteriori estimates\n");

		vector_add_vector(X, sol, n, -1., raz);
		t1 = vector_norm_2(raz);

		std::cout << std::format("{:>22}\n", t1);

		t1 = pow(Hnorm, i);
		t2 = t1 * X0norm;

		t1 = t1 * gnorm / (1 - Hnorm);
		t1 = t2 + t1;

		std::cout << std::format("{:>22}\n", t1);

		vector_add_vector(X, Xprev, n, -1., raz);
		t2 = vector_norm_2(raz);
		t1 = Hnorm * t2 / (1 - Hnorm);

		std::cout << std::format("{:>22}\n", t1);

		printf_s("----------------\n");
	}
	return true;
}

void GetJacobyMatrices(const alglib::real_2d_array& A, alglib::real_2d_array& M,
	alglib::real_2d_array& N)
{
	const int n = A.cols();
	M.setlength(n, n); // D
	N.setlength(n, n);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			N[i][j] = A[i][j];
			M[i][j] = 0;
		}
		for (int j = i + 1; j < n; ++j)
		{
			N[i][j] = A[i][j];
			M[i][j] = 0;
		}
		N[i][i] = 0;
		M[i][i] = A[i][i];
	}
}

void GetGaussZeidelMatrices(const alglib::real_2d_array& A, alglib::real_2d_array& M, 
	alglib::real_2d_array& N)
{
	const int n = A.cols();
	M.setlength(n, n); // L + D
	N.setlength(n, n);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j <= i; ++j)
		{
			N[i][j] = 0;
			M[i][j] = A[i][j];
		}
		for (int j = i+1; j < n; ++j)
		{
			N[i][j] = A[i][j];
			M[i][j] = 0;
		}
	}
}

void PassingMethod(const alglib::real_2d_array& A, const alglib::real_1d_array& d, 
	const int& N, alglib::real_1d_array& X) 
	{
		X.setlength(N);

		//int op = 0;

		std::vector<double> a(N), b(N);
		a[0] = -A[0][1] / A[0][0];
		b[0] = d[0] / A[0][0];

		double g;
		for (int i = 1; i < N - 1; ++i) 
		{
			g = A[i][i] + A[i][i - 1] * a[i - 1];
			a[i] = -A[i][i + 1] / g;
			b[i] = (d[i]- A[i][i - 1]*b[i-1])/g;
			//op += 7;
		}
		g = A[N-1][N - 1] + A[N - 1][N - 2] * a[N - 2];
		b[N - 1] = (d[N - 1] - A[N - 1][N - 2] * b[N - 2]) / g;
		
		X[N - 1] = b[N - 1];

		for (int i = N - 2; i >= 0; --i) 
		{
			X[i] = a[i] * X[i + 1] + b[i];
			//op += 3;
		}

		//op += 7;
		//std::cout << "Operations count = " << op << "\n";
		//std::cout << "Expected = " << 8 * N << "\n";

	}

alglib::real_2d_array SquareRootsMethod(const alglib::real_2d_array& A, const int& N)
	{
		// std::cout << "\nSquare Roots Method\n";
		// int operation_count = 0;
		alglib::real_2d_array L;
		L.setlength(N, N);

		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j) L[i][j] = 0;

		double sqr_sum;

		for (int i = 0; i < N; ++i)
		{
			// Подсчитываем диагональные элементы
			sqr_sum = A[i][i];
			for (int p = 0; p < i; ++p)
				sqr_sum -= pow(L[i][p], 2); // operation_count++;

			L[i][i] = sqrt(sqr_sum); // operation_count++;

			// Находим элементы сверху диагонали
			for (int j = i + 1; j < N; ++j)
			{
				sqr_sum = A[j][i];
				for (int p = 0; p < i; ++p)
					sqr_sum -= L[i][p] * L[j][p]; // operation_count += 2;

				L[j][i] = 1. / L[i][i] * sqr_sum; // operation_count++;
			}

		}

		// std::cout << "operations count = " << operation_count << "\n";

		return L;
	}

bool CheckSymmetry(const alglib::real_2d_array& A, const int& N)
	{
		for (int row = 0; row < N; ++row) {
			for (int column = row + 1; column < N; ++column) {
				if (A[row][column] != A[column][row]) return false;
			}
		}
		return true;
	}

bool CheckPositiveCertainty(const alglib::real_2d_array& A, const int& N)
	{
		// Проверка положительной определённости по критерию Сильвестра
		for (int i = 0; i < N; ++i)
			if (alglib::rmatrixdet(A, N) < 0) return false;
		return true;
	}

alglib::real_1d_array CholeskyMethod(const alglib::real_2d_array& A, const alglib::real_1d_array& b, const int& N)
	{
		std::cout << "\n\nMatrix A and vector b\n";

		print_matrix_and_vector(A, b, N);

		std::cout << "\n\nCholesky method\n";

		bool is_positive = CheckPositiveCertainty(A, N);
		bool is_symmetrical = CheckSymmetry(A, N);

		std::cout << "Is positive - ";
		if (is_positive) std::cout << "true";
		else std::cout << "false";

		std::cout << "\nIs symmetrical - ";
		if (is_symmetrical) std::cout << "true";
		else std::cout << "false";
		std::cout << "\n";


		alglib::real_1d_array X;
		X.setlength(N);

		if (!is_positive || !is_symmetrical) return X;


		alglib::real_2d_array L = SquareRootsMethod(A, N);

		std::cout << "Matrix L\n";

		print_matrix(L);

		std::cout << "Matrix L Transposed\n";

		alglib::real_2d_array LT;
		LT.setlength(N, N);
		alglib::rmatrixtranspose(N, N, L, 0, 0, LT, 0, 0);

		print_matrix(LT);


		std::cout << "Check LL^T = A\n";

		std::cout << "Matrix A\n";

		print_matrix(A);

		std::cout << "Matrix LLT\n";

		alglib::real_2d_array LLT;
		LLT.setlength(N, N);

		multiply_matrices(L, LT, LLT);

		print_matrix(LLT);


		std::cout << "Solving LLTx=b\nLy = b\n";

		print_matrix_and_vector(L, b, N);

		// Решаем систему вида Ly = b относительно y, учитывая нижнетреугольность матрицы
		alglib::real_1d_array Y;
		Y.setlength(N);
		double sum;
		int operation_count = 0;
		for (int i = 0; i < N; ++i) 
		{
			sum = b[i];
			for (int j = 0; j < i; ++j) 
			{
				sum -= Y[j] * L[i][j]; operation_count += 2;
			}

			Y[i] = sum / L[i][i]; ++operation_count;
		}
		std::cout << "Vector y\n";

		print_vector(Y);

		std::cout << "operations count = " << operation_count << "\n";

		std::cout << "LTx = y\n";

		print_matrix_and_vector(LT, Y, N);

		// LTx = y относительно x, учитывая верхнетреугольность матрицы
		for (int i = N - 1; i >= 0; --i) {
			sum = Y[i];
			for (int j = i + 1; j < N; ++j) {
				sum -= X[j] * LT[i][j]; operation_count += 2;
			}

			X[i] = sum / L[i][i]; ++operation_count;
		}

		std::cout << "Vector x\n";

		print_vector(X);

		std::cout << "all operations count = " << operation_count << "\n";

		double det;
		alglib::real_2d_array Lu, U, Ai;
		GaussMethod(A, b, X, N, det, Lu, U, Ai);

		return X;
	}

void GilbertMatrix()
	{
		static int input_size{ 5 };
		ImGui::InputInt("", &input_size, 1, 4);

		static int size = input_size;
		static alglib::integer_2d_array G_down; // Коэффициенты снизу матрицы Гильберта
		static alglib::real_2d_array G; // Матрица Гильберта


		static bool isCalculated{ false };

		if (ImGui::Button("Вычислить матрицу Гильберта")) {
			size = input_size;
			G_down.setlength(size, size);
			G.setlength(size, size);

			for (int i = 0; i < size; ++i)
				for (int j = 0; j < size; ++j) {
					G[i][j] = 1.;
					G[i][j] /= G_down[i][j] = i + j + 1;
				}

			isCalculated = true;
		}

		static double alpha = 0.0000001;
		ImGui::InputDouble("alpha", &alpha, 0.0001, 0.01, "%.12f");

		if (ImGui::Button("Проверить матрицу Гильберта")) {
			CheckGilbert(G, size, alpha);
		}

		if (isCalculated)
		{
			if (ImGui::BeginTable("table1", size)) {
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (int row = 0; row < size; row++)
				{
					ImGui::TableNextRow();
					for (int column = 0; column < size; ++column)
					{
						ImGui::TableSetColumnIndex(column);
						ImGui::Text("1/%d", G_down[row][column]);
					}
				}
				ImGui::EndTable();
			}
			if (ImGui::BeginTable("table2", size)) {
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_None);
				ImGui::TableHeadersRow();

				for (int row = 0; row < size; row++)
				{
					ImGui::TableNextRow();
					for (int column = 0; column < size; ++column)
					{
						ImGui::TableSetColumnIndex(column);
						ImGui::Text("%.4f", G[row][column]);
					}
				}
				ImGui::EndTable();
			}
		}
	}

void GaussMethod(const alglib::real_2d_array& tA, const alglib::real_1d_array& tb, alglib::real_1d_array& X, const int& N, double& determinant, alglib::real_2d_array& L, alglib::real_2d_array& U, alglib::real_2d_array& Ai)
{
	int operations_count{ (int)(2. / 3 * pow(N, 3) + 3. / 2 * pow(N, 2) - 7. / 6 * N) };
	int operations_count_reverse{ N * N };

	// int counter = 0;

	alglib::real_2d_array A;
	alglib::real_1d_array b;

	A.setlength(N, N);
	L.setlength(N, N);
	U.setlength(N, N);
	b.setlength(N);
	X.setlength(N);

	// Копируем
	alglib::rmatrixcopy(N, N, tA, 0, 0, A, 0, 0);
	alglib::rvectorcopy(N, tb, 0, b, 0);

	// Заполняем матрицы
	for (int row = 0; row < N; ++row)
	{
		for (int column = 0; column < N; ++column)
		{
			L[row][column] = 0;
			U[row][column] = 0;
		}
	}

	matrix_identity(Ai, N);


	 /*std::cout << "Matrix A and vector b";
	 print_matrix_and_vector(A, b, N);

	 std::cout << "Inverse matrix A";
	 print_matrix(Ai, N);*/

	// Прямой ход
	double main_value, cur_value, larger_main, tmp;
	bool isLarger = false;
	determinant = 1;
	int id;
	for (int step = 0; step < N; ++step)
	{
		larger_main = 0;
		id = step;
		//// Ищем наибольший по модулю ведущий элемент
		//for (int row = step + 1; row < N; ++row)
		//	if (abs(A[row][step]) > larger_main) {
		//		isLarger = true;
		//		larger_main = abs(A[row][step]);
		//		id = row;
		//	}

		//// Если нашёлся больший элемент
		//if (isLarger)
		//{
		//	isLarger = false;
		//	// Переставляем строки i и id в матрице A
		//	for (int column = i; column < N; ++column)
		//	{
		//		tmp = A[i][column];
		//		A[i][column] = A[id][column];
		//		A[id][column] = tmp;
		//	}

		//	//Переставляем элементы в векторе b
		//	tmp = b[i];
		//	b[i] = b[id];
		//	b[id] = tmp;
		//}

		//std::cout << "Swapped " << step << " with " << id << "\n";

		main_value = A[step][step];

		// Умножаем детерминант на ведущий элемент
		determinant *= main_value;

		// Делим строку на ведущий элемент и заполняем U
		b[step] /= main_value;
		matrix_row_multiplication(Ai, N, step, 1. / main_value);
		for (int row = N - 1; row >= step; --row)
		{
			U[step][row] = A[step][row];
			A[step][row] /= main_value;
			// counter++;
		}

		// Вычитаем из следующих строк i-ую, умноженную на самый левый коэффициент
		for (int row = step + 1; row < N; ++row) // строки
		{
			cur_value = A[row][step];
			A[row][step] = 0;

			// Вкладываем коэффициент в L
			L[row][step] = cur_value / main_value;

			// Вычитаем из строк > step строку step 
			matrix_add_row(Ai, N, row, step, -cur_value);

			for (int columns = step + 1; columns < N; ++columns)
			{
				A[row][columns] -= A[step][columns] * cur_value; // counter += 2;
			}
			b[row] -= b[step] * cur_value; // counter += 2;
		}
	}

	//printf_s("Upper triangular matrix A and b");
	//print_matrix_and_vector(A, b, N);

	//printf_s("Not complete inverse matrix A");
	//print_matrix_and_vector(Ai, b, N);

	// Выводим количество операций
	//std::cout << "Operations of forward stroke = " << counter << "\n";
	//std::cout << "Expected = " << operations_count - operations_count_reverse << "\n";

	// Обратный ход
	for (int row_main = N - 1; row_main >= 0; --row_main) {
		for (int row_cur = 0; row_cur < row_main; ++row_cur) {

			// Вносим изменения в обратную матрицу
			matrix_add_row(Ai, N, row_cur, row_main, -A[row_cur][row_main]);

			b[row_cur] -= b[row_main] * A[row_cur][row_main];
			A[row_cur][row_main] = 0;
			// counter += 2;
		}
		X[row_main] = b[row_main];
	}

	//printf_s("Matrix A after Gauss and b"); print_matrix_and_vector(A, b, N);
	//printf_s("Inverse matrix A"); print_matrix(Ai, N);
	//printf_s("Matrix A"); print_matrix(tA, N);

	//printf_s("Inverse matrix check AA^-1");
	//alglib::real_2d_array An;
	//An.setlength(N, N);
	//multiply_matriсes(tA, Ai, N, An);
	//print_matrix(An, N);


	// Заполняем диагональ матрицы L
	for (int i = 0; i < N; ++i)
		L[i][i] = 1;

	//printf_s("Matrix L"); print_matrix(L, N);
	//printf_s("Matrix U"); print_matrix(U, N);

	// Делаем проверку перемножением матриц и выводим LU
	// multiply_matriсes(L, U, N, An);
	//printf_s("Matrix LU");
	//print_matrix(An, N);
	//std::cout << std::format("\n\nDet(L) = 1\nDet(U)=Det(A)={:>22}\n\n", determinant);

	//// Найдём матрицы L и U через alglib
	//alglib::integer_1d_array piv;
	//alglib::rmatrixcopy(N, N, tA, 0, 0, An, 0, 0);
	//alglib::rmatrixlu(An, piv);
	//printf_s("Matrix LU by alglib"); print_matrix(An, N);

	// Решаем через библиотеку
	alglib::rvectorcopy(N, tb, 0, b, 0);
	alglib::rmatrixsolvefast(tA, N, b); // В b записывается решение


	// Выводим
	//printf_s("Vector X founded by an algorithm and by library Alglib");
	//for (int rows = 0; rows < N; ++rows) {
	//	printf_s("\n");
	//	std::cout << std::format("{:>22}\t|{:>22}", X[rows], b[rows]);
	//}
	//std::cout << "\n";

	// Выводим количество операций
	//std::cout << "\n\nOperations complete = " << counter << "\n";
	//std::cout << "Expected = " << operations_count << "\n\n";
}

// По X ищется b через LU-декомпозицию, в конце в X содержится найденный вектор
void LUb(const alglib::real_2d_array& tA, alglib::real_1d_array& X, const int& N, alglib::real_2d_array& L, alglib::real_2d_array& U)
	{

		alglib::real_1d_array Y, b;
		Y.setlength(N);
		b.setlength(N);

		// Применяем к X верхнетреугольную U
		multiply_matrix_by_vector(U, X, Y);

		std::cout << "\nResult of operation UX";
		print_vector(Y);

		// Применяем к Y нижнетреугольную L
		multiply_matrix_by_vector(L, Y, X);

		std::cout << "\nResult of operation LUX";
		print_vector(X);

	}

bool GetInversedMatrix(const alglib::real_2d_array& tA, alglib::real_2d_array& AI)
{
	const int n = tA.cols();
	if (n != tA.rows()) return false;

	alglib::real_2d_array A;
	A.setlength(n,n);
	alglib::rmatrixcopy(n, n, tA, 0, 0, A, 0, 0);

	matrix_identity(AI, n);

	double cur_value, main_value;
	for (int step = 0; step < n; ++step)
	{
		main_value = A[step][step];

		matrix_row_multiplication(AI, n, step, 1. / main_value);
		for (int row = n - 1; row >= step; --row)
			A[step][row] /= main_value;

		for (int row = step + 1; row < n; ++row)
		{
			cur_value = A[row][step];
			A[row][step] = 0;

			matrix_add_row(AI, n, row, step, -cur_value);

			for (int columns = step + 1; columns < n; ++columns)
				A[row][columns] -= A[step][columns] * cur_value;
		}
	}

	for (int row_main = n - 1; row_main >= 0; --row_main)
	{
		for (int row_cur = 0; row_cur < row_main; ++row_cur)
		{
			matrix_add_row(AI, n, row_cur, row_main, -A[row_cur][row_main]);

			A[row_cur][row_main] = 0;
		}
	}

	alglib::real_2d_array Ch;
	multiply_matrices(tA, AI, Ch);
	printf_s("Check A*AI = E\n"); print_matrix(Ch);
	
	return true;
}

void print_matrix_and_vector(const alglib::real_2d_array& A, const alglib::real_1d_array& b, const  int& N)
{
	for (int row = 0; row < N; ++row)
	{
		printf_s("\n");
		for (int column = 0; column < N; ++column)
		{
			std::cout << std::format("{:>22}", A[row][column]);
		}
		std::cout << std::format("\t|{:>22}", b[row]);
	}
	printf_s("\n\n");
}

void print_vector(const alglib::real_1d_array& X)
{
	for (int row = 0; row < X.length(); ++row)
	{
		printf_s("\n");
		std::cout << std::format("\t{:>22}", X[row]);
	}
	printf_s("\n\n");
}

void print_matrix(const alglib::real_2d_array& A) {
		for (int row = 0; row < A.rows(); ++row) {
			printf_s("\n");
			for (int column = 0; column < A.cols(); ++column) {
				std::cout << std::format("{:>22}", A[row][column]);
			}
		}
		printf_s("\n\n");
	}

void print_eigens(const alglib::real_2d_array& A, const int & N) 
	{
		std::cout << std::format("\n\nAlglib output\n");
		alglib::real_1d_array eigenvalues_real, eigenvalues_imaginery;
		eigenvalues_real.setlength(N);
		eigenvalues_imaginery.setlength(N);
		alglib::real_2d_array eigenvectors_real, eigenvectors_imaginery;
		eigenvectors_real.setlength(N, N);
		eigenvectors_imaginery.setlength(N, N);

		alglib::rmatrixevd(A, N, 3, eigenvalues_real, eigenvalues_imaginery,
			eigenvectors_real, eigenvectors_imaginery);

		std::cout << std::format("Eigenvalues\n");
		for (int i = 0; i < eigenvalues_real.length(); ++i) {
			std::cout << std::format("{:>22}+{:>22}i\n",
				eigenvalues_real[i], eigenvalues_imaginery[i]);
		}

		std::cout << std::format("\nEigenvectors\n");
		std::cout << std::format("Real part\n");
		print_matrix(eigenvectors_real);
		std::cout << std::format("Imaginery part\n");
		print_matrix(eigenvectors_imaginery);
	}

void CheckGilbert(const alglib::real_2d_array& G, const int& N, const double& alpha) {
		alglib::real_1d_array X, b;
		alglib::real_2d_array L, U;
		X.setlength(N);
		b.setlength(N);
		L.setlength(N, N);
		U.setlength(N, N);

		for (int i = 0; i < N; ++i)
			X[i] = 1;

		// Выводим X
		std::cout << std::format("Vector X");
		print_vector(X);

		// Находим b
		multiply_matrix_by_vector(G, X, b);

		// Выводим b
		std::cout << std::format("Vector AX = b");
		print_vector(b);

		double determinant;

		alglib::real_2d_array Ai;
		Ai.setlength(N, N);

		// Проверяем решение по методу Гаусса
		GaussMethod(G, b, X, N, determinant, L, U, Ai);

		// Поиск числа обусловленности
		double mu = matrix_condition_number(G, Ai, N);

		std::cout << std::format("Matrix condition number {:>22}\n", mu);

		// Приближение по формулам Эйлера
		// Находим b

		for (int i = 0; i < N; ++i)
			X[i] = 1;
		
		alglib::real_1d_array z;
		z.setlength(N);
		multiply_matrix_by_vector(G, X, z);
		multiply_matrix_by_vector(G, z, X);

		alglib::real_2d_array A2;
		multiply_matrices(G, G, A2);

		for (int i = 0; i < N; ++i)
			A2[i][i] += alpha;



		GaussMethod(A2, X, z, N, determinant, L, U, Ai);


	}

void matrix_row_multiplication(alglib::real_2d_array& A, const int& N, const int& r1, const double& coeff) {
		for (int column = 0; column < N; ++column)
			A[r1][column] *= coeff;
	}

// A.cols == B.rows
void multiply_matrices(const alglib::real_2d_array& A, const alglib::real_2d_array& B, alglib::real_2d_array& C)
{
	//assert(A.cols() == B.rows());
	C.setlength(A.rows(), B.cols());
	double sum;
	for (int row = 0; row < A.rows(); ++row)
	{
		for (int column = 0; column < B.cols(); ++column)
		{
			sum = 0;
			for (int i = 0; i < A.cols(); ++i)
			{
				sum += A[row][i] * B[i][column];
				//std::cout << A[row][i] << "\t*\t" << B[i][column] << "\n";
			}
			//std::cout << "sum = " << tmp << "\n";
			C[row][column] = sum;
		}
	}
}

// A.cols == B.rows
void multiply_matrix_by_vector(const alglib::real_2d_array& A, const alglib::real_1d_array& V, alglib::real_1d_array& C)
{
	//assert(A.rows() == V.length());
	C.setlength(V.length());
	double sum;
	for (int row = 0; row < A.rows(); ++row)
	{
		sum = 0;
		for (int column = 0; column < A.cols(); ++column)
			sum += A[row][column] * V[column];
		C[row] = sum;
	}
}

// К r1 добавить r2, умноженное на коэффициент
void matrix_add_row(alglib::real_2d_array& A, const int& N, const int& r1, const int& r2, const double& coeff) {
		for (int column = 0; column < N; ++column)
			A[r1][column] += A[r2][column] * coeff;
	}

void vector_add_vector(const alglib::real_1d_array& X, const alglib::real_1d_array& Y, const int& N, const double& coeff, alglib::real_1d_array& res) {
		for (int i = 0; i < N; ++i)
			res[i] = X[i] + Y[i] * coeff;
	}

void multiply_vector_by_scalar(const alglib::real_1d_array& X, const int N, const double & scalar, alglib::real_1d_array& res) 
	{
		for (int i = 0; i < N; ++i) {
			res[i] = X[i] * scalar;
		}
	}

void scalar_product(const alglib::real_1d_array& X, const alglib::real_1d_array& Y, const int& N, double& res) {
		res = 0;
		for (int i = 0; i < N; ++i) {
			res += X[i] * Y[i];
		}
	}

double matrix_determinant(const alglib::real_2d_array& A)
{
	const int n = A.rows();
	if (n == 0 || n != A.cols()) return -1;

	std::vector<int> it, curSign;
	std::vector<bool> isOccupied;

	it.resize(n);
	isOccupied.resize(n);
	curSign.resize(n);

	double res = 0, cur = 1;
	for (int i = 0; i < n; ++i)
	{
		it[i] = i;
		isOccupied[i] = true;
		curSign[i] = 1;
		cur *= A[i][it[i]];
	}
	res += cur;

	int i;
	while (it[0] < n)
	{
		for (i = n - 1; i >= 0; --i)
		{
			cur /= A[i][it[i]] * curSign[i];
			isOccupied[it[i]] = false;

			do ++it[i];
			while (it[i] < n && isOccupied[it[i]]);

			if (it[i] == n) continue;

			curSign[i] *= -1;
			cur *= A[i][it[i]] * curSign[i];
			isOccupied[it[i]] = true;
			break; // начинаем перевычисления с этого момента
		}

		if (i == -1) break;
		++i;

		for (; i < n; ++i)
		{	
			it[i] = 0;

			while (isOccupied[it[i]]) ++it[i];

			curSign[i] = 1;
			cur *= A[i][it[i]];
			isOccupied[it[i]] = true;
		}

		res += cur;
	}

	return res;
}


alglib::real_1d_array random_vector(const int& N, const int& seed, const double& bound_A, const double& bound_B, const int double_digits_count)
	{
		alglib::real_1d_array X;
		X.setlength(N);

		srand(seed);

		double double_part;
		int int_part;

		std::cout << "\n\n";

		for (int i = 0; i < N; ++i) {
			int_part = double_part = (double)(rand()) * time(0) * (pow(10, -double_digits_count));
			double_part -= int_part;
			std::cout << "Random number\n";
			std::cout << int_part << "\t \t" << double_part << "\n";
			X[i] = int_part % (int)(bound_B - bound_A) + bound_A + double_part;
		}

		// Выводим X
		std::cout << "\nRandomly generated vector X";
		print_vector(X);

		return X;
	}

double matrix_condition_number_by_eigenvalues(const alglib::real_2d_array& A, const int& N)
	{
		alglib::real_1d_array eigenvalues_real, eigenvalues_imaginery;
		eigenvalues_real.setlength(N);
		eigenvalues_imaginery.setlength(N);
		alglib::real_2d_array eigenvectors_real, eigenvectors_imaginery;

		alglib::rmatrixevd(A, N, 0, eigenvalues_real, eigenvalues_imaginery,
			eigenvectors_real, eigenvectors_imaginery);

		std::cout << std::format("\n\nEigenvalues\n");
		for (int i = 0; i < eigenvalues_real.length(); ++i) {
			std::cout << std::format("{:>22}+{:>22}i\n",
				eigenvalues_real[i], eigenvalues_imaginery[i]);
		}

		std::cout << std::format("\nRadii\n");
		double min = DBL_MAX, max = 0, r = 0;
		for (int i = 0; i < eigenvalues_real.length(); ++i)
		{
			r = sqrt(pow(eigenvalues_real[i], 2) + pow(eigenvalues_imaginery[i], 2));
			std::cout << std::format("{:>22}\n", r);
			if (r < min) min = r;
			if (r > max) max = r;
		}

		return max / min;
	}

double vector_norm_2(const alglib::real_1d_array& X)
{
	double res = 0;
	for (int i = 0; i < X.length(); ++i)
		res += pow(X[i], 2);
	return pow(res, 0.5);
}

double vector_norm_max_abs(const alglib::real_1d_array& X) {
		double max = 0;

		for (int i = 0; i < X.length(); ++i)
			if (max > abs(X[i]))
				max = abs(X[i]);

		return max;
	}

double matrix_norm(const alglib::real_2d_array& A) {
		double max = 0, sum;
		for (int row = 0; row < A.rows(); ++row)
		{
			sum = 0;
			for (int column = 0; column < A.cols(); ++column)
				sum += abs(A[row][column]);

			if (sum > max) max = sum;
		}
		return max;
	}

double matrix_condition_number(const alglib::real_2d_array& A, const alglib::real_2d_array& Ai, const int& N)
	{
		return matrix_norm(A) * matrix_norm(Ai);
	}


void matrix_identity(alglib::real_2d_array& A, const int& N)
{
	A.setlength(N, N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < i; ++j)
			A[i][j] = 0;
		A[i][i] = 1;
		for (int j = i+1; j < N; ++j)
			A[i][j] = 0;
	}
}

void vector_identity(alglib::real_1d_array& V, const int& N)
{
	V.setlength(N);
	for (int i = 0; i < N; ++i)
		V[i] = 1;
}
