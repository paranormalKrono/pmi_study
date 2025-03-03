#include "IntegrationSystems.h"

#pragma execution_character_set("utf-8")

void quadrature_formulas(double end, double begin,
	variable** variables, queue* main_function, queue* antiderivative_function);

void compound_quadrature_formulas(double end, double begin,
	variable** variables, queue* main_function, queue* antiderivative_function);

void quadrature_meller(double A, double B, variable** variables, queue* main_function);
void quadrature_gauss(double A, double B, variable** variables, queue* main_function, queue* antiderivative_function, std::string main_function_name);
void quadrature_weigth(double fixed_A, double fixed_B, variable** variables, queue* main_function);

// Для работы требуется 1 первообразная и функция
void IntegrationMenu(double end, double begin,
	variable** variables, std::vector<queue*> function_rpns, std::vector<std::string> function_names, int* main_function_index)
{

	if (*main_function_index > 0 && ImGui::TreeNode("Интегрирование с первой первообразной"))
	{
		if (ImGui::TreeNode("Интегрирование по Гауссу"))
		{
			quadrature_gauss(begin, end, variables, function_rpns.at(*main_function_index), function_rpns.at(*main_function_index - 1), function_names.at(*main_function_index));

			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Интегрирование по СКФ"))
		{
			compound_quadrature_formulas(end, begin, variables, function_rpns.at(*main_function_index), function_rpns.at(*main_function_index - 1));

			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Интегрирование по КФ"))
		{
			quadrature_formulas(end, begin, variables, function_rpns.at(*main_function_index), function_rpns.at(*main_function_index - 1));
			ImGui::TreePop();
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Интегрирование КФ Меллера"))
	{
		quadrature_meller(begin, end, variables, function_rpns.at(*main_function_index));

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Интегрирование ИКФ с весом"))
	{
		quadrature_weigth(begin, end, variables, function_rpns.at(*main_function_index));

		ImGui::TreePop();
	}


}

void quadrature_meller(double A, double B, variable** variables, queue* main_function)
{

	static int arguments_count_input = 4;
	if (arguments_count_input < 2) return;

	static std::vector<double> arguments;
	static std::vector<double> function_values;
	static double Ak;
	static int arguments_count = 0;

	static double integral_value_by_Meller;

	ImGui::InputInt("Введите количество узлов", &arguments_count_input, 1, 10);

	ImGui::Separator();

	if (ImGui::Button("Обновить размеры массивов"))
	{
		arguments_count = arguments_count_input;
		arguments.resize(arguments_count);
		function_values.resize(arguments_count);
	}

	if (ImGui::Button("Вычислить узлы и коэффициент"))
	{
		Ak = (double)M_PI / arguments_count;
		printf_s("\nCalculated coefficient: %lf\n", Ak);
		printf_s("Calculated arguments:\n");
		for (int i = 0; i < arguments_count; ++i)
		{
			arguments.at(i) = cos((2.0 * (i + 1) - 1.0) * (double)M_PI / (2.0 * arguments_count));
			printf_s("\t%d]\t%.64lf\n", i, arguments.at(i));
		}

	}

	ImGui::Separator();

	if (ImGui::Button("Вычислить значения функции в узлах \n(зависит от узлов)"))
	{
		for (int i = 0; i < arguments_count; ++i)
		{
			variables[0]->value = arguments[i];
			function_values[i] = get_RPN_result(main_function, variables, 1);
		}
	}



	if (ImGui::Button("Вычислить значение интеграла"))
	{
		integral_value_by_Meller = 0;
		for (int i = 0; i < arguments_count; ++i)
			integral_value_by_Meller += function_values[i];
		integral_value_by_Meller *= Ak;
	}

	ImGui::Separator();

	ImGui::Text("Коэффициент\t-\t%.24f\nЗначение по формуле Меллера на [-1, 1]\t-\t %.32f",
		Ak, integral_value_by_Meller);

	ImGui::Separator();

	if (ImGui::TreeNode("Сводные данные"))
	{
		static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
		if (ImGui::BeginTable("table6", 2, table_flags))
		{
			ImGui::TableSetupColumn("Узлы на [-1, 1]", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Значения функции на [-1, 1]", ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();
			for (int row = 0; row < arguments_count; row++)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%.8f", arguments[row]);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.E %.8f", function_values[row], function_values[row]);
			}
			ImGui::EndTable();
		}

		ImGui::TreePop();
	}
}


void quadrature_gauss(double A, double B, variable** variables, queue* main_function, queue* antiderivative_function, std::string main_function_name)
{

	static int arguments_count_input = 4;
	if (arguments_count_input < 2) return;

	static std::vector<double> function_values;
	static std::vector<double> function_values_AB;
	static std::vector<double> arguments;
	static std::vector<double> arguments_AB;
	static std::vector<double> Ak;
	static std::vector<double> Ak_AB;
	static int arguments_count = 0;

	static int divisions_count = 1000;
	static int secant_divisions_count = arguments_count_input * 200;
	static double secant_error = 1.0E-12;

	static double integral_real_value, integral_real_value_AB;
	static double integral_value_by_Gauss, integral_value_AB_by_Gauss;
	static double integral_error_by_Gauss, integral_error_AB_by_Gauss;

	ImGui::InputInt("Введите количество узлов", &arguments_count_input, 1, 10);
	ImGui::InputInt("Количество делений промежутка для СКФ", &divisions_count);
	ImGui::InputInt("Количество делений промежутка для метода секущих", &secant_divisions_count);
	ImGui::InputDouble("Погрешность нахождения решения методом секущих", &secant_error, 1.0E-11, 1.0E-6, "%.E");

	ImGui::Separator();

	if (ImGui::Button("Обновить размеры массивов"))
	{
		arguments_count = arguments_count_input;
		arguments.resize(arguments_count);
		arguments_AB.resize(arguments_count);
		function_values.resize(arguments_count);
		function_values_AB.resize(arguments_count);
		Ak.resize(arguments_count);
		Ak_AB.resize(arguments_count);
	}

	if (ImGui::Button("Вычислить ортогональные полиномы Лежандра, узлы и коэффициенты"))
	{
		printf_s("Calculated orthogonal polinoms:\n");

		std::vector<double> C(arguments_count + 1);
		C.at(0) = 1;
		C.at(1) = 1;
		printf_s("\t0] 1\n\t1] x\n");
		double q, d;
		for (int n = 2; n <= arguments_count; ++n)
		{
			q = (2.0 * n - 1.0) / n;
			d = (1.0 - n) / n;

			for (int k = n - 2; k >= 0; k -= 2)
				C.at(k) = d * C.at(k);

			for (int k = n; k > 0; k -= 2)
				C.at(k) += q * C.at(k - 1);

			printf_s("\t%d] ", n);
			for (int i = n; i >= 0; i -= 2)
			{
				printf_s("%+.2fx^%d ", C.at(i), i);
			}
			printf_s("\n");
		}


		std::string polinom = "";
		for (int i = arguments_count; i >= 0; i -= 2)
			polinom += std::format("{0:+.32f}*pow(x,{1})", C.at(i), i);
		printf_s("\n\nResult polinom: %s\n", polinom.data());

		queue* legendre_polinom_rpn = get_Reverse_Polish_Notation(polinom.data());

		int solutions_count;
		double* xs = non_linear_equation(&solutions_count, -1.0, 1.0, secant_divisions_count, secant_error, bisection, variables, 0, 1, legendre_polinom_rpn);

		printf_s("\nFounded %d arguments by bisection method\n", solutions_count);
		if (solutions_count > arguments_count) printf_s("[ERROR]");
		if (solutions_count <= arguments_count)
		{
			for (int i = 0; i < solutions_count; ++i)
			{
				arguments[i] = xs[i];
				printf_s("\t%+.32lf\n", arguments.at(i));
			}
			for (int i = solutions_count; i < arguments_count; ++i)
				arguments[i] = 0;
		}
		else
		{
			for (int i = 0; i < arguments_count; ++i)
			{
				arguments[i] = xs[i];
				printf_s("\t%+.64lf\n", arguments.at(i));
			}
		}

		std::string polinom_prev = "";
		for (int i = arguments_count - 1; i >= 0; i -= 2)
			polinom_prev += std::format("{0:+.32f}*pow(x,{1})", C.at(i), i);

		printf_s("\nPolinom for finding coefficients: %s\n", polinom_prev.data());
		queue* legendre_polinom_prev_rpn = get_Reverse_Polish_Notation(polinom_prev.data());

		double x, sum = 0;
		for (int i = 0; i < arguments_count; ++i)
		{
			variables[0]->value = arguments[i];
			x = get_RPN_result(legendre_polinom_prev_rpn, variables, 1);
			sum += Ak[i] = 2 * (1 - pow(arguments[i], 2)) / pow(arguments_count * x, 2);
		}

		printf_s("Check coefficients sum equals length of [-1, 1]: 2 = {sum = %.16lf}\n", sum);

	}

	ImGui::Separator();

	if (ImGui::Button("Вычислить узлы и коэффициенты на [A,B]"))
	{
		double sum = 0;
		for (int i = 0; i < arguments_count; ++i)
		{
			arguments_AB[i] = (B - A) / 2 * arguments[i] + (B + A) / 2;
			sum += Ak_AB[i] = Ak[i] / 2 * (B - A);
		}

		printf_s("Check coefficients sum equals length of [A,B]: %lf = {sum = %.16lf}\n", B - A, sum);
	}

	ImGui::Separator();

	if (ImGui::Button("Вычислить значения функции в узлах \n(зависит от узлов)"))
	{
		for (int i = 0; i < arguments_count; ++i)
		{
			variables[0]->value = arguments[i];
			function_values[i] = get_RPN_result(main_function, variables, 1);
			variables[0]->value = arguments_AB[i];
			function_values_AB[i] = get_RPN_result(main_function, variables, 1);
		}
	}



	if (ImGui::Button("Вычислить значение интеграла"))
	{
		double Ak_sum = 0;
		for (int i = 0; i < arguments_count; ++i)
		{
			Ak_sum += Ak[i];
		}

		variables[0]->value = -1;
		double QFA = get_RPN_result(antiderivative_function, variables, 1);
		variables[0]->value = 1;
		double QFB = get_RPN_result(antiderivative_function, variables, 1);
		integral_real_value = QFB - QFA;

		variables[0]->value = A;
		QFA = get_RPN_result(antiderivative_function, variables, 1);
		variables[0]->value = B;
		QFB = get_RPN_result(antiderivative_function, variables, 1);
		integral_real_value_AB = QFB - QFA;

		integral_value_by_Gauss = 0;
		for (int i = 0; i < arguments_count; ++i)
			integral_value_by_Gauss += function_values[i] * Ak[i];

		integral_value_AB_by_Gauss = 0;
		for (int i = 0; i < arguments_count; ++i)
			integral_value_AB_by_Gauss += function_values_AB[i] * Ak_AB[i];


		integral_error_by_Gauss = abs(integral_real_value - integral_value_by_Gauss);
		integral_error_AB_by_Gauss = abs(integral_real_value_AB - integral_value_AB_by_Gauss);
	}

	ImGui::Separator();

	ImGui::Text("Реальное значение интеграла на [-1, 1]\t-\t%.24f\nЗначение по формуле Гаусса на [-1, 1]\t-\t %.32f\nАбсолютная погрешность по формуле Гаусса на [-1, 1]\t-\t%.E %.32f\n\n"
		"Реальное значение интеграла на [A, B]\t-\t%.24f\nЗначение по формуле Гаусса на [A, B]\t-\t %.32f\nАбсолютная погрешность по формуле Гаусса на [A, B]\t-\t%.E %.32f",
		integral_real_value, integral_value_by_Gauss, integral_error_by_Gauss, integral_error_by_Gauss,
		integral_real_value_AB, integral_value_AB_by_Gauss, integral_error_AB_by_Gauss, integral_error_AB_by_Gauss);

	ImGui::Separator();

	if (ImGui::TreeNode("Сводные данные"))
	{
		static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
		if (ImGui::BeginTable("table5", 6, table_flags))
		{
			ImGui::TableSetupColumn("Узлы на [-1, 1]", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Коэффициенты КФ Гаусса на [-1, 1]", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Значения функции на [-1, 1]", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Узлы на [A, B]", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Коэффициенты КФ Гаусса на [A, B]", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Значения функции на [A, B]", ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();
			for (int row = 0; row < arguments_count; row++)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%.8f", arguments[row]);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.E %.8f", Ak[row], Ak[row]);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%.E %.8f", function_values[row], function_values[row]);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%.8f", arguments_AB[row]);
				ImGui::TableSetColumnIndex(4);
				ImGui::Text("%.E %.8f", Ak_AB[row], Ak_AB[row]);
				ImGui::TableSetColumnIndex(5);
				ImGui::Text("%.E %.8f", function_values_AB[row], function_values_AB[row]);
			}
			ImGui::EndTable();
		}

		ImGui::TreePop();
	}


	ImGui::Separator();


	if (ImGui::TreeNode("Составная КФ Гаусса с весом"))
	{
		// function = "sin(x)"
		static std::string weight_function = "exp(-x)";
		static std::string function_with_weight_antiderivative = "-(sin(x)+cos(x))/(2*exp(x))";

		static queue* weight_function_rpn;
		static queue* function_with_weight_antiderivative_rpn;

		static double integral_real_weight_value;
		static double integral_simpsons_weight_value;
		static double integral_value_by_compound_Gauss;
		static double integral_error_by_compound_Gauss_relate_to_real, integral_error_by_compound_Gauss_relate_to_simpson;

		static int divisions_count_input = 4;
		static int divisions_count = divisions_count_input;
		static int compound_arguments_count = divisions_count * arguments_count;

		static std::vector<double> compound_arguments(compound_arguments_count);
		static std::vector<double> main_values;

		ImGui::InputText("Весовая функция", &weight_function);
		ImGui::InputText("Первообразная весовой функции умноженной на основную (q(x)*f(x))", &function_with_weight_antiderivative);

		ImGui::Separator();

		ImGui::DragInt("Количество подразделений для СКФ Гаусса", &divisions_count_input, 1, 2, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Separator();


		if (ImGui::Button("Получить обратные польские нотации"))
		{
			weight_function_rpn = get_Reverse_Polish_Notation(weight_function.data());
			function_with_weight_antiderivative_rpn = get_Reverse_Polish_Notation(function_with_weight_antiderivative.data());
		}



		ImGui::Separator();


		if (ImGui::Button("Вычислить узлы, значения функции и интеграла"))
		{
			divisions_count = divisions_count_input;
			compound_arguments_count = divisions_count * arguments_count;

			main_values.resize(compound_arguments_count);
			compound_arguments.resize(compound_arguments_count);

			std::string function_with_weight = weight_function + "*" + main_function_name;
			queue* function_with_weight_rpn = get_Reverse_Polish_Notation(function_with_weight.data());

			printf_s("\nFunction with weight: %s\n", function_with_weight.c_str());

			double h = (B - A) / divisions_count;

			printf_s("\nCalculating arguments and function with weight values [division argument]:\n");
			int index;
			for (int j = 0; j < divisions_count; ++j)
				for (int k = 0; k < arguments_count; ++k)
				{
					index = j * arguments_count + k;
					//arguments_divisions[j] = h / 2 * arguments[k] + A + (j + 1) * h;
					compound_arguments[index] = h / 2 * arguments[k] + (A + j * h + A + (j + 1) * h) / 2;

					variables[0]->value = compound_arguments[index];
					main_values[index] = get_RPN_result(function_with_weight_rpn, variables, 1);

					printf_s("\t%d %d] %.32lf\t%.32lf\n", j, k, compound_arguments[index], main_values[index]);
				}


			variables[0]->value = A;
			double FA = get_RPN_result(function_with_weight_antiderivative_rpn, variables, 1);
			variables[0]->value = B;
			double FB = get_RPN_result(function_with_weight_antiderivative_rpn, variables, 1);

			integral_real_weight_value = FB - FA;


			integral_simpsons_weight_value = get_integration_compound_simpsons(function_with_weight_rpn, variables, 0, 1, A, B, 2000);


			// По СКФ Гаусса
			integral_value_by_compound_Gauss = 0;
			for (int j = 0; j < divisions_count; ++j)
			{
				for (int k = 0; k < arguments_count; ++k)
				{
					index = j * arguments_count + k;
					variables[0]->value = compound_arguments[index];
					integral_value_by_compound_Gauss += Ak[k] * main_values[index];
				}
			}
			integral_value_by_compound_Gauss *= h / 2;


			integral_error_by_compound_Gauss_relate_to_real = abs(integral_value_by_compound_Gauss - integral_real_weight_value);
			integral_error_by_compound_Gauss_relate_to_simpson = abs(integral_value_by_compound_Gauss - integral_simpsons_weight_value);

			queue_free(function_with_weight_rpn);

		}

		ImGui::Text("Реальное значение интеграла с весом на [A,B]\t-\t%.24f\nЗначение интеграла с весом на [A, B] по СКФ Симпсона с 1000 делений\t-\t%.24f\n"
			"Значение по СКФ Гаусса на [A,B]\t-\t %.32f\nАбсолютная погрешность по СКФ Гаусса на [A,B] относительно СКФ Симпсона\t-\t%.E %.32f\n"
			"Абсолютная погрешность по СКФ Гаусса на [A,B] отгосительно реального значения\t-\t%.E %.32f\n\n",
			integral_real_weight_value, integral_simpsons_weight_value,
			integral_value_by_compound_Gauss, integral_error_by_compound_Gauss_relate_to_simpson, integral_error_by_compound_Gauss_relate_to_simpson,
			integral_error_by_compound_Gauss_relate_to_real, integral_error_by_compound_Gauss_relate_to_real);


		ImGui::TreePop();
	}
}



void quadrature_weigth(double A, double B, variable** variables, queue* main_function)
{
	// function = "sin(x)"
	static std::string weight_function = "exp(-x)";
	static std::string function_with_weight_antiderivative = "-(sin(x)+cos(x))/(2*exp(x))";

	static int arguments_count_input = 8;
	static int divisions_count = 1000;
	static int secant_divisions_count = arguments_count_input * 200;
	static double secant_error = 1.0E-12;

	static queue* weight_function_rpn;
	static queue* function_with_weight_antiderivative_rpn;

	static std::vector<double> arguments;
	static std::vector<double> function_values;
	static std::vector<double> moments; // содержит 2 * fixed_arguments_count элементов
	static std::vector<double> ak; // для многочлена W fixed_arguments_count (x)
	static std::vector<double> Ak;
	static int arguments_count = 0;


	static double QFA, QFB;
	static double integral_real_value, integral_value_by_IQF;
	static double integral_error_by_IQF;



	ImGui::InputText("Весовая функция", &weight_function);
	ImGui::InputText("Первообразная весовой функции умноженной на основную (q(x)*f(x))", &function_with_weight_antiderivative);
	ImGui::InputInt("Введите количество пар аргумент-значение", &arguments_count_input);
	ImGui::InputInt("Количество делений промежутка для СКФ", &divisions_count);
	ImGui::InputInt("Количество делений промежутка для метода секущих", &secant_divisions_count);
	ImGui::InputDouble("Погрешность нахождения решения методом секущих", &secant_error, 1.0E-11, 1.0E-6, "%.E");

	ImGui::Separator();

	if (ImGui::Button("Получить обратные польские нотации"))
	{
		weight_function_rpn = get_Reverse_Polish_Notation(weight_function.data());
		function_with_weight_antiderivative_rpn = get_Reverse_Polish_Notation(function_with_weight_antiderivative.data());
	}


	if (ImGui::Button("Обновить размеры массивов"))
	{
		arguments_count = arguments_count_input;
		arguments.resize(arguments_count);
		function_values.resize(arguments_count);
		moments.resize(arguments_count * 2);
		Ak.resize(arguments_count);
	}

	if (ImGui::Button("Вычислить моменты весовой функции с заданным числом делений\n(зависит от весовой функции, промежутка и числа делений)"))
	{
		//moments[i] = get_integration_compound_middle_rectange(f, variables, 0, 1, fixed_A, fixed_B, divisions_count);
		// Вычисление моментов по Симпсону, оптимизированное
		double h = (B - A) / (double)divisions_count;

		// Mm = h/6 * (fA + 2*w + 4*q + fB)
		// fA = wA * pow(A, m)
		// fB = wB * pow(B, m)
		// r = A+h*i, d = A+h*(i+0.5)
		// q = sumi[w(d)] * pow(d, m), i = 0,args-1
		// w = sumi[w(r)] * pow(r, m), i = 1,args-1
		// Mm = h/6 * (wA*pow(A,m) + wB*pow(B,m) + 2*pow(d,m)*si[w(d)] + 4*pow(r,m)*si[w(r)])

		double wA, wB, q, w, r, d;

		variables[0]->value = A;
		wA = get_RPN_result(weight_function_rpn, variables, 1);
		variables[0]->value = B;
		wB = get_RPN_result(weight_function_rpn, variables, 1);

		d = variables[0]->value = A + h * 0.5;
		q = 4 * get_RPN_result(weight_function_rpn, variables, 1);

		for (int m = 0; m < arguments_count * 2; ++m)
			moments[m] = wA * pow(A, m) + wB * pow(B, m) + q * pow(d, m);


		for (int i = 1; i < divisions_count; ++i)
		{
			r = variables[0]->value = A + h * (double)i;
			w = 2 * get_RPN_result(weight_function_rpn, variables, 1);
			d = variables[0]->value = A + h * ((double)i + 0.5);
			q = 4 * get_RPN_result(weight_function_rpn, variables, 1);

			for (int m = 0; m < arguments_count * 2; ++m)
				moments[m] += q * pow(d, m) + w * pow(r, m);
		}

		h /= 6;
		for (int m = 0; m < arguments_count * 2; ++m)
			moments[m] *= h;
	}

	ImGui::Separator();

	if (ImGui::Button("Задать узлы автоматически"))
	{
		for (int i = 0; i < arguments_count; ++i)
		{
			arguments[i] = A + (B - A) / (arguments_count - 1) * i;
		}
	}


	if (ImGui::Button("Вычислить узлы по КФ НАСТ\n(зависит от моментов)"))
	{
		printf_s("\n\n-----| Arguments calculating |-----\n");
		ak.resize(arguments_count);

		printf_s("Moments matrix");
		alglib::real_2d_array X;
		X.setlength(arguments_count, arguments_count);
		for (int rows = 0; rows < arguments_count; ++rows)
		{
			printf_s("\n\t");
			for (int columns = 0; columns < arguments_count; ++columns)
			{
				X[rows][columns] = moments.at(columns + rows);
				printf_s("%2.2f ", X[rows][columns]);
			}
		}
		printf_s("\n");

		printf_s("Right matrix\n");
		alglib::real_1d_array M;
		M.setlength(arguments_count);
		for (int rows = 0; rows < arguments_count; ++rows)
		{
			M[rows] = -moments.at(rows + arguments_count);
			printf_s("\t%2.2f\n", M[rows]);
		}
		printf_s("\n");

		alglib::rmatrixsolvefast(X, arguments_count, M); // В M записывается решение

		for (int rows = 0; rows < arguments_count; ++rows)
			ak[rows] = M[rows];

		// Строим многочлен
		std::string wnx = std::format("pow(x,{})", arguments_count);
		std::string wnx_view = std::format("x^{}", arguments_count);
		for (int i = arguments_count - 1; i >= 0; --i)
		{
			wnx += std::format("{0:+.32f}*pow(x,{1})", ak[i], i);
			wnx_view += std::format("{0:+.12f}x^{1}", ak[i], i);
		}
		printf_s("Orthogonal polinom\n%s\n\n", wnx_view.c_str());
		printf_s("Orthogonal polinom for calculations\n%s\n\n", wnx.c_str());

		// Получаем решение по методу секущих
		queue* wnx_rpn = get_Reverse_Polish_Notation(wnx.data());
		int solutions_count;
		double* xs = non_linear_equation(&solutions_count, A, B, secant_divisions_count, secant_error, bisection, variables, 0, 1, wnx_rpn);

		printf_s("\nFounded %d arguments by bisection method\n", solutions_count);
		if (solutions_count > arguments_count) printf_s("[ERROR]");
		if (solutions_count <= arguments_count)
		{
			for (int i = 0; i < solutions_count; ++i)
			{
				arguments[i] = xs[i];
				printf_s("\t%+.64lf\n", arguments.at(i));
			}
			for (int i = solutions_count; i < arguments_count; ++i)
				arguments[i] = 0;
		}
		else
		{
			for (int i = 0; i < arguments_count; ++i)
			{
				arguments[i] = xs[i];
				printf_s("\t%+.64lf\n", arguments.at(i));
			}
		}

		queue_free(wnx_rpn);
		free(xs);
	}

	ImGui::Separator();

	if (ImGui::Button("Вычислить значения функции в заданных узлах \n(зависит от узлов)"))
	{
		for (int i = 0; i < arguments_count; ++i)
		{
			variables[0]->value = arguments[i];
			function_values[i] = get_RPN_result(main_function, variables, 1);
		}
	}


	if (ImGui::Button("Вычислить коэффициенты ИКФ\n(зависит от узлов и моментов)"))
	{
		printf_s("\n\n-----| IQF coeffs calculating |-----\n");
		printf_s("Arguments in degrees matrix");
		alglib::real_2d_array X;
		X.setlength(arguments_count, arguments_count);
		for (int rows = 0; rows < arguments_count; ++rows)
		{
			printf_s("\n\t");
			for (int columns = 0; columns < arguments_count; ++columns)
			{
				X[rows][columns] = pow(arguments[columns], rows);
				printf_s("%2.2f ", X[rows][columns]);
			}
		}
		printf_s("\n");

		printf_s("Right matrix\n");
		alglib::real_1d_array M;
		M.setlength(arguments_count);
		for (int rows = 0; rows < arguments_count; ++rows)
		{
			M[rows] = moments[rows];
			printf_s("\t%.32lf\n", M[rows]);
		}
		printf_s("\n");

		alglib::rmatrixsolvefast(X, arguments_count, M); // В M записывается решение

		for (int rows = 0; rows < arguments_count; ++rows)
			Ak[rows] = M[rows];
	}

	if (ImGui::Button("Вычислить значение интеграла\n(требуются значения функции в узлах)"))
	{
		double Ak_sum = 0;
		for (int i = 0; i < arguments_count; ++i)
		{
			Ak_sum += Ak[i];
		}
		printf_s("\nCheck coeffs\nAk_sum\t\t%.16lf\nmoment_0\t%.16lf\n\n", Ak_sum, moments.at(0));

		variables[0]->value = A;
		QFA = get_RPN_result(function_with_weight_antiderivative_rpn, variables, 1);
		variables[0]->value = B;
		QFB = get_RPN_result(function_with_weight_antiderivative_rpn, variables, 1);
		integral_real_value = QFB - QFA;

		integral_value_by_IQF = 0;
		for (int i = 0; i < arguments_count; ++i)
			integral_value_by_IQF += function_values[i] * Ak[i];
		integral_error_by_IQF = abs(integral_real_value - integral_value_by_IQF);
	}

	ImGui::Separator();

	ImGui::Text("Реальное значение интеграла\t-\t%.24f\nЗначение по ИКФ\t-\t %.32f\nАбсолютная погрешность по ИКФ\t-\t%.E %.32f",
		integral_real_value, integral_value_by_IQF, integral_error_by_IQF, integral_error_by_IQF);

	ImGui::Separator();

	if (ImGui::TreeNode("Сводные данные"))
	{
		static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
		if (ImGui::BeginTable("table4", 4, table_flags))
		{
			ImGui::TableSetupColumn("Моменты весовой функции", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Узлы", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Значения функции", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Коэффициенты ИКФ", ImGuiTableColumnFlags_None);
			ImGui::TableHeadersRow();
			for (int row = 0; row < arguments_count; row++)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%.E %.8f", moments[row], moments[row]);
				ImGui::TableSetColumnIndex(1);
				char buf[32];
				sprintf_s(buf, "%d", row);
				ImGui::InputDouble(buf, &(arguments[row]), 0.1, 2.0, "%.8f");
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%.E %.12f", function_values[row], function_values[row]);
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%.E %.12f", Ak[row], Ak[row]);
			}
			ImGui::EndTable();
		}

		ImGui::TreePop();
	}


}



void runge_principle(double B, double A, variable** variables, queue* main_function, queue* antiderivative_function, float fd_max[3],
	int divisions_count, double compound_value_table[6][3])
{
	static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

	static int divisions_multiplier = 2;
	static int divisions_multiplier_input = divisions_multiplier;
	ImGui::DragInt("Во сколько раз увеличить число разбиений", &divisions_multiplier_input, 1, 2, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

	static int pairs_count = divisions_count * 2 + 1, multiplied_divisions_count = 4;

	static double* arguments = (double*)malloc(sizeof(double) * pairs_count);
	static double* main_values;

	static bool isCalculated = false;

	if (ImGui::Button("Пересчитать значения функции"))
	{
		divisions_multiplier = divisions_multiplier_input;
		multiplied_divisions_count = divisions_count * divisions_multiplier;
		pairs_count = multiplied_divisions_count * 2 + 1;

		main_values = (double*)realloc(main_values, sizeof(double) * pairs_count);
		arguments = (double*)realloc(arguments, sizeof(double) * pairs_count);

		for (int i = 0; i < pairs_count; ++i)
			arguments[i] = A + i * (B - A) / (double)(pairs_count - 1);

		for (int j = 0; j < pairs_count; ++j)
		{
			variables[0]->value = arguments[j];
			main_values[j] = get_RPN_result(main_function, variables, 1);
		}

		isCalculated = true;
	}

	static double runge_value_table[6][4];
	static const char* names_table[6] = {
		{"По Ньютону-Лейбницу"},
		{"СКФ Левые прямоугольники"},
		{"СКФ Правые прямоугольники"},
		{"СКФ Средние прямоугольники"},
		{"СКФ Трапеции"},
		{"СКФ Симпсон"}
	};

	runge_value_table[0][0] = compound_value_table[0][0];
	static double FA, FB, fA, fB;
	static double W, Q, h;
	if (isCalculated && ImGui::Button("Посчитать таблицу по Рунге"))
	{
		fA = main_values[0];
		fB = main_values[pairs_count - 1];

		Q = W = h = 0;
		h = (B - A) / (double)multiplied_divisions_count;
		for (int m = 1; m < (pairs_count - 1) / 2; ++m)
			W += main_values[m * 2];
		for (int m = 0; m < (pairs_count - 1) / 2; ++m)
			Q += main_values[m * 2 + 1];

		// Значение m*l
		runge_value_table[1][0] = integration_compound_left_rectangle(h, W, fA);
		runge_value_table[2][0] = integration_compound_right_rectangle(h, W, fB);
		runge_value_table[3][0] = integration_compound_middle_rectangle(h, Q);
		runge_value_table[4][0] = integration_compound_trapezoid(h, W, fA, fB);
		runge_value_table[5][0] = integration_compound_simpsons(h, W, Q, fA, fB);

		// Фактическая погрешность m*l
		for (int m = 1; m < 6; ++m)
			runge_value_table[m][1] = abs(runge_value_table[0][0] - runge_value_table[m][0]);

		// Значение по Рунге
		// F = F2h + (F2h - Fh)/((1/fixed_multiplied_divisions_count) ^ p - 1)
		double pow_r_2 = pow(divisions_multiplier, 1);
		double pow_r_3 = pow(divisions_multiplier, 2);
		double pow_r_5 = pow(divisions_multiplier, 4);
		runge_value_table[1][2] = (pow_r_2 * runge_value_table[1][0] - compound_value_table[1][0]) / (pow_r_2 - 1);
		runge_value_table[2][2] = (pow_r_2 * runge_value_table[2][0] - compound_value_table[2][0]) / (pow_r_2 - 1);
		runge_value_table[3][2] = (pow_r_3 * runge_value_table[3][0] - compound_value_table[3][0]) / (pow_r_3 - 1);
		runge_value_table[4][2] = (pow_r_3 * runge_value_table[4][0] - compound_value_table[4][0]) / (pow_r_3 - 1);
		runge_value_table[5][2] = (pow_r_5 * runge_value_table[5][0] - compound_value_table[5][0]) / (pow_r_5 - 1);

		// Фактическая погрешность для Рунге
		for (int m = 1; m < 6; ++m)
			runge_value_table[m][3] = abs(runge_value_table[0][0] - runge_value_table[m][2]);

	}


	if (isCalculated && ImGui::BeginTable("table3", 5, table_flags))
	{
		ImGui::TableSetupColumn("Метод", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Значение m*l", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Фактическая погрешность m*l", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Значение по Рунге", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Фактическая погрешность для Рунге", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();
		for (int row = 0; row < 6; row++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", names_table[row]);
			for (int column = 0; column < 4; column++)
			{
				ImGui::TableSetColumnIndex(column + 1);
				ImGui::Text("%.E %.10f", runge_value_table[row][column], runge_value_table[row][column]);
			}
		}
		ImGui::EndTable();
	}
}



void compound_quadrature_formulas(double end, double begin,
	variable** variables, queue* main_function, queue* antiderivative_function)
{
	static float fd_max[3]{ 1.0f, 1.0f, 1.0f };
	ImGui::DragFloat3("Максимум значений 1, 2 и 4 производных", fd_max, 0.1f, 0, 1000, "%.4f", ImGuiSliderFlags_AlwaysClamp);

	static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

	static double fixed_A = begin, fixed_B = end;

	static int divisions_count = 4;
	ImGui::DragInt("Количество делений отрезка для составных КФ", &divisions_count, 1, 2, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

	static int fixed_pairs_count = 9, fixed_divisions_count = 4;

	static double* arguments = (double*)malloc(sizeof(double) * fixed_pairs_count);
	static double* main_values;

	static bool isCalculated = false;

	if (ImGui::Button("Пересчитать значения функции и первообразной при заданном числе делений"))
	{
		fixed_divisions_count = divisions_count;
		fixed_pairs_count = fixed_divisions_count * 2 + 1;
		fixed_A = begin;
		fixed_B = end;

		main_values = (double*)realloc(main_values, sizeof(double) * fixed_pairs_count);
		arguments = (double*)realloc(arguments, sizeof(double) * fixed_pairs_count);

		for (int m = 0; m < fixed_pairs_count; ++m)
			arguments[m] = fixed_A + m * (fixed_B - fixed_A) / (double)(fixed_pairs_count - 1);

		for (int j = 0; j < fixed_pairs_count; ++j)
		{
			variables[0]->value = arguments[j];
			main_values[j] = get_RPN_result(main_function, variables, 1);
		}

		isCalculated = true;
	}


	if (isCalculated && ImPlot::BeginPlot("Значения в точках"))
	{
		ImPlot::SetupAxisLimits(ImAxis_X1, 0, 1.0);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1.6);
		ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond);
		ImPlot::PlotStems("Функция", arguments, main_values, fixed_pairs_count);
		ImPlot::EndPlot();
	}


	static double value_table[6][3]; // real error real_error
	static const char* names_table[6] = {
		{"По Ньютону-Лейбницу"},
		{"СКФ Левые прямоугольники"},
		{"СКФ Правые прямоугольники"},
		{"СКФ Средние прямоугольники"},
		{"СКФ Трапеции"},
		{"СКФ Симпсон"}
	};

	static double FA, FB, fA, fB;
	static double W, Q, h;
	if (isCalculated && ImGui::Button("Посчитать таблицу интегралов по СКФ"))
	{
		variables[0]->value = fixed_A;
		FA = get_RPN_result(antiderivative_function, variables, 1);
		variables[0]->value = fixed_B;
		FB = get_RPN_result(antiderivative_function, variables, 1);

		value_table[0][0] = FB - FA;

		fA = main_values[0];
		fB = main_values[fixed_pairs_count - 1];

		Q = W = h = 0;
		h = (fixed_B - fixed_A) / (double)fixed_divisions_count;
		for (int m = 1; m < (fixed_pairs_count - 1) / 2; ++m)
			W += main_values[m * 2];
		for (int m = 0; m < (fixed_pairs_count - 1) / 2; ++m)
			Q += main_values[m * 2 + 1];

		value_table[1][0] = integration_compound_left_rectangle(h, W, fA);
		value_table[2][0] = integration_compound_right_rectangle(h, W, fB);
		value_table[3][0] = integration_compound_middle_rectangle(h, Q);
		value_table[4][0] = integration_compound_trapezoid(h, W, fA, fB);
		value_table[5][0] = integration_compound_simpsons(h, W, Q, fA, fB);
		//value_table[6][0] = integration_compound_38(h, W fA);

		double l = fixed_B - fixed_A;
		double l2 = pow(l, 2);
		double l3 = pow(l, 3);
		double l5 = pow(l, 5);

		double m = (double)fixed_divisions_count;
		double m2 = pow(m, 2);
		double m4 = pow(m, 4);

		value_table[1][1] = abs(integration_compound_left_theoretical_error(l2, m, fd_max[0]));
		value_table[2][1] = abs(integration_compound_right_theoretical_error(l2, m, fd_max[0]));
		value_table[3][1] = abs(integration_compound_middle_theoretical_error(l3, m2, fd_max[1]));
		value_table[4][1] = abs(integration_compound_trapezoid_theoretical_error(l3, m2, fd_max[1]));
		value_table[5][1] = abs(integration_compound_simpsons_theoretical_error(l5, m4, fd_max[2]));

		for (int m = 1; m < 6; ++m)
			value_table[m][2] = abs(value_table[0][0] - value_table[m][0]);
	}

	if (isCalculated && ImGui::BeginTable("table2", 4, table_flags))
	{
		ImGui::TableSetupColumn("Метод", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Значение", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Теоретическая погрешность", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Фактическая погрешность", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();
		for (int row = 0; row < 6; row++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", names_table[row]);
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column + 1);
				ImGui::Text("%.E %.10f", value_table[row][column], value_table[row][column]);
			}
		}
		ImGui::EndTable();
	}


	if (isCalculated && ImGui::TreeNode("Уточнение по принципу Рунге"))
	{
		runge_principle(fixed_B, fixed_A, variables, main_function, antiderivative_function, fd_max, fixed_divisions_count, value_table);

		ImGui::TreePop();
	}
}



void quadrature_formulas(double end, double begin,
	variable** variables, queue* main_function, queue* antiderivative_function)
{
	static float fd_max[3]{ 1.0f, 1.0f, 1.0f };
	ImGui::DragFloat3("Максимум значений 1, 2 и 4 производных", fd_max, 0.1f, 0, 1000, "%.4f", ImGuiSliderFlags_AlwaysClamp);

	static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

	static double value_table[7][3]; // real error real_error
	static const char* names_table[7] = {
		{"По Ньютону-Лейбницу"},
		{"КФ Левые прямоугольники"},
		{"КФ Правые прямоугольники"},
		{"КФ Средние прямоугольники"},
		{"КФ Трапеции"},
		{"КФ Симпсон"},
		{"КФ Три восьмых"}
	};

	static double FA, FB, fA, fB;

	if (ImGui::Button("Посчитать интегралы по КФ"))
	{
		variables[0]->value = begin;
		fA = get_RPN_result(main_function, variables, 1);
		FA = get_RPN_result(antiderivative_function, variables, 1);
		variables[0]->value = end;
		fB = get_RPN_result(main_function, variables, 1);
		FB = get_RPN_result(antiderivative_function, variables, 1);

		value_table[0][0] = FB - FA;

		double fAB2, fA1B, fA2B;
		double l = end - begin;
		double l2 = pow(l, 2);
		double l3 = pow(l, 3);
		double l5 = pow(l, 5);


		variables[0]->value = begin + l * 0.5;
		fAB2 = get_RPN_result(main_function, variables, 1);
		variables[0]->value = begin + l / 3.0;
		fA1B = get_RPN_result(main_function, variables, 1);
		variables[0]->value = begin + 2.0 * l / 3.0;
		fA2B = get_RPN_result(main_function, variables, 1);

		value_table[1][0] = integration_left_rectangle(l, fA);
		value_table[2][0] = integration_right_rectangle(l, fB);
		value_table[3][0] = integration_middle_rectangle(l, fAB2);
		value_table[4][0] = integration_trapezoid(l, fA, fB);
		value_table[5][0] = integration_simpsons(l, fA, fAB2, fB);
		value_table[6][0] = integration_38(l, fA, fA1B, fA2B, fB);

		value_table[1][1] = abs(integration_left_theoretical_error(l2, fd_max[0]));
		value_table[2][1] = abs(integration_right_theoretical_error(l2, fd_max[0]));
		value_table[3][1] = abs(integration_middle_theoretical_error(l3, fd_max[1]));
		value_table[4][1] = abs(integration_trapezoid_theoretical_error(l3, fd_max[1]));
		value_table[5][1] = abs(integration_simpsons_theoretical_error(l5, fd_max[2]));
		value_table[6][1] = abs(integration_38_theoretical_error(l5, fd_max[2]));

		for (int m = 1; m < 7; ++m)
			value_table[m][2] = abs(value_table[0][0] - value_table[m][0]);
	}

	if (ImGui::BeginTable("table1", 4, table_flags))
	{
		ImGui::TableSetupColumn("Метод", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Значение", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Теоретическая погрешность", ImGuiTableColumnFlags_None);
		ImGui::TableSetupColumn("Фактическая погрешность", ImGuiTableColumnFlags_None);
		ImGui::TableHeadersRow();
		for (int row = 0; row < 7; row++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%s", names_table[row]);
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column + 1);
				ImGui::Text("%.E %.10f", value_table[row][column], value_table[row][column]);
			}
		}
		ImGui::EndTable();
	}

}


void input_functions(std::vector<std::string>::iterator start, int count, int start_number);

void show_functions(double begin, double end, std::vector<std::string> function_names, std::vector<queue*> function_rpns, variable** variables);

void Integration()
{

	// Можно заполнить
	static int antiderivatives_count_input = 1;
	static int deriatives_count_input = 3;
	static float A = 1.0f, B = 2.0f;

	static int antiderivatives_count = antiderivatives_count_input;
	static int deriatives_count = deriatives_count_input;
	static int* main_function_index = &antiderivatives_count;
	static int function_count = deriatives_count + antiderivatives_count + 1;

	static std::vector<std::string> function_names{
		"pow(x,4)",
			"sin(x)",
			"12*pow(x,2)",
			"24*x",
			"24"
	};
	static std::vector<std::string>::iterator main_function_name = function_names.begin() + 1;

	static bool isNewFunctionsRPN = true;
	static std::vector<queue*> function_rpns;
	static std::vector<std::string> function_rpns_strs;
	static variable** variables = formula_rpn::get_standard_variable();

	input_functions(main_function_name, 1, 1);

	ImGui::DragFloatRange2("Промежуток", &A, &B, 0.1f, -FLT_MAX, FLT_MAX, "A: %.4f", "B: %.4f", ImGuiSliderFlags_AlwaysClamp);

	if (ImGui::TreeNode("Производные и первообразные"))
	{
		ImGui::DragInt("Количество первообразных функции", &antiderivatives_count_input, 0.5f, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragInt("Количество производных функции", &deriatives_count_input, 0.5f, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

		if (ImGui::Button("Обновить количество производных и первообразных функции"))
		{
			int l1 = antiderivatives_count_input - antiderivatives_count;

			if (l1 > 0)
				for (int i = 0; i < l1; ++i)
					function_names.insert(function_names.begin(), "");
			else
				for (int i = 0; i < -l1; ++i)
					function_names.erase(function_names.begin());

			deriatives_count = deriatives_count_input;
			antiderivatives_count = antiderivatives_count_input;
			function_count = deriatives_count + antiderivatives_count + 1;

			function_names.resize(function_count, "");
			main_function_name = function_names.begin() + antiderivatives_count;

			isNewFunctionsRPN = true;
		}


		if (ImGui::TreeNode("Ввод первообразных"))
		{
			if (antiderivatives_count > 0)
				input_functions(function_names.begin(), antiderivatives_count, 0);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Ввод производных"))
		{
			if (deriatives_count > 0)
				input_functions(main_function_name + 1, deriatives_count, antiderivatives_count + 1);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}


	if (ImGui::Button("Получить обратную польскую нотацию для всех функций"))
	{
		function_rpns.resize(function_names.size());
		function_rpns_strs.resize(function_names.size());
		for (int i = 0; i < function_names.size(); ++i)
		{

			function_rpns[i] = get_Reverse_Polish_Notation(function_names.at(i).data());
			function_rpns_strs[i] = formula_rpn::queue_to_string(function_rpns.at(i));
		}

		isNewFunctionsRPN = false;
	}


	if (ImGui::TreeNode("Обратные польские нотации"))
	{
		for (int i = 0; i < function_rpns_strs.size(); ++i)
			ImGui::Text(function_rpns_strs[i].c_str());
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("График функции"))
	{
		show_functions(A, B, function_names, function_rpns, variables);
		ImGui::TreePop();
	}


	if (ImGui::TreeNode("Интегрирование"))
	{
		if (!isNewFunctionsRPN)
			IntegrationMenu(B, A, variables, function_rpns, function_names, main_function_index);

		ImGui::TreePop();
	}

}

void show_functions(double begin, double end, std::vector<std::string> function_names, std::vector<queue*> function_rpns, variable** variables)
{
	static int pairs_for_view_count = 18;
	ImGui::DragInt("Количество пар аргумент-значение функции для просмотра", &pairs_for_view_count, 1, 1, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

	static double* arguments_array;
	static std::vector<double*> values_array;

	static int fixed_pairs_count = 1, fixed_function_count = 0;
	static double fixed_begin = begin, fixed_end = end;

	if ((fixed_function_count != function_rpns.size() || fixed_pairs_count != pairs_for_view_count || fixed_begin != begin || fixed_end != end)
		&& ImGui::Button("Вычислить пары аргумент-значение и показать график"))
	{
		values_array.resize(function_rpns.size());

		for (int i = 0; i < fixed_function_count; ++i)
			values_array[i] = (double*)realloc(values_array[i], sizeof(double) * pairs_for_view_count);
		for (int i = fixed_function_count; i < function_rpns.size(); ++i)
			values_array[i] = (double*)malloc(sizeof(double) * pairs_for_view_count);

		arguments_array = (double*)realloc(arguments_array, sizeof(double) * pairs_for_view_count);

		fixed_function_count = function_rpns.size();
		fixed_begin = begin;
		fixed_end = end;
		fixed_pairs_count = pairs_for_view_count;

		for (int i = 0; i < pairs_for_view_count; ++i)
			arguments_array[i] = begin + i * (end - begin) / (double)pairs_for_view_count;

		for (int i = 0; i < fixed_function_count; ++i)
			if (function_rpns.at(i))
				for (int j = 0; j < pairs_for_view_count; ++j)
				{
					variables[0]->value = arguments_array[j];
					values_array.at(i)[j] = get_RPN_result(function_rpns.at(i), variables, 1);
				}
	}


	if (!arguments_array) return;

	if (ImGui::Button("Написать пары аргумент-значение в консоль"))
		for (int i = 0; i < fixed_function_count; ++i)
		{
			if (i < function_names.size()) printf_s("argument-value pairs for %s\n", function_names[i]);
			for (int j = 0; j < fixed_pairs_count; ++j)
				printf_s("\t[%lf, %lf]\n", arguments_array[j], values_array.at(i)[j]);
			printf_s("\n");
		}


	static ImVec2 size = { 1350, 600 };

	static bool show_lines = true;
	static bool show_fills = true;
	static float fill_ref = 0;
	static int shade_mode = 2;
	static ImPlotShadedFlags flags = 0;


	if (ImPlot::BeginPlot("График функций", size))
	{
		ImPlot::SetupAxes("X", "Y");
		ImPlot::SetupAxesLimits(begin, end, -10, 10);

		for (int i = 0; i < fixed_function_count; ++i)
		{
			if (i < function_names.size())
			{
				if (show_fills)
				{
					ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
					ImPlot::PlotShaded(function_names[i].data(), arguments_array, values_array.at(i), fixed_pairs_count,
						shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref, flags);
					ImPlot::PopStyleVar();
				}
				if (show_lines)
				{
					ImPlot::PlotLine(function_names[i].data(), arguments_array, values_array.at(i), fixed_pairs_count);
				}
			}
		}

		ImPlot::EndPlot();
	}
}

void input_functions(std::vector<std::string>::iterator start, int count, int start_number)
{
	for (int i = 0; i < count; ++i)
	{
		char buf[32];
		sprintf_s(buf, "Функция %d", start_number + i);

		ImGui::InputText(buf, &(start[i]));
	}
}