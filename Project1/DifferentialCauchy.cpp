#include "DifferentialSystems.h"

#pragma execution_character_set("utf-8")

void CauchyMenu()
{
	ImGui::Text("y'(x) = f(x,y) \t y(x0) = y0");
	static std::string function_def { "-y + 2*pow(y,2) + 1" };
	static std::string solution_def { "(pow(7,0.5) * tg((pow(7,0.5)*x + C) * 0.5) + 1) * 0.25" };
	static float x0{ 0 }, y0{ 0.25 }, C{ 0 };
	static float h{ 0.1 };
	static int N{ 10 };

	ImGui::InputText("Функция f(x,y)", &(function_def));
	ImGui::InputText("Функция y(x)", &(solution_def));
	ImGui::InputFloat("Введите x0", &x0, -FLT_MAX, FLT_MAX, "%.8f");
	ImGui::InputFloat("Введите y0", &y0, -FLT_MAX, FLT_MAX, "%.8f");
	ImGui::InputFloat("Введите C", &C, -FLT_MAX, FLT_MAX, "%.8f");
	ImGui::InputFloat("Введите шаг h", &h, -FLT_MAX, FLT_MAX, "%.8f");
	ImGui::InputInt("Введите N", &N);

	const int N_shift{ 3 };
	static int arguments_count{ N + N_shift };
	static bool isRPNCalculated{ false }, isArraysUpdated{ false };
	static queue* function_rpn, * solution_rpn;
	static variable** variables { formula_rpn::get_standard_variables(3, "x", "y") }; // x, y, C

	static formula_rpn* function_formula { new formula_rpn(function_def, 3, variables) };

	const int methods_count{ 6 };
	static std::vector<double> arguments, solution_values, table_values[methods_count], table_errors[methods_count];

	if (ImGui::Button("Вычислить обратную польскую нотацию"))
	{
		function_formula->recalculate_rpn();
		solution_rpn = get_Reverse_Polish_Notation(solution_def.data());

		isRPNCalculated = true;
	}

	if (ImGui::Button("Обновить массивы"))
	{
		isArraysUpdated = true;
		arguments_count = N + N_shift;
		arguments.resize(arguments_count);
		solution_values.resize(arguments_count);

		for (int column = 0; column < methods_count; ++column)
		{
			table_values[column].resize(arguments_count);
			table_errors[column].resize(arguments_count);
		}
	}

	if (isRPNCalculated && arguments_count == N + N_shift && ImGui::Button("Вычислить"))
	{
		for (int i = 0; i < arguments_count; ++i)
		{
			arguments[i] = x0 + h * (i - 2); // Решение y зависит только от x

			variables[0]->value = arguments[i];
			solution_values[i] = get_RPN_result(solution_rpn, variables, 3); // Реальные значения 
		}

		// Метод Тейлора
		const int tailor_count = 5;
		double ykt[tailor_count];

		// Находим коэффициенты разложения решения в ряд последовательным дифференцированием обеих частей уравнения и подстановкой 
		ykt[0] = y0;

		function_formula->calculate(ykt[1], { arguments[0],  y0 });

		ykt[2] = -ykt[1] + 4 * ykt[0] * ykt[1];
		ykt[3] = -ykt[2] + 4 * (pow(ykt[1], 2) + ykt[0] * ykt[2]);
		ykt[4] = -ykt[3] + 4 * (3 * ykt[1] * ykt[2] + ykt[0] + ykt[3]);

		double fact[tailor_count];
		fact[0] = 1;
		for (int i = 1; i < tailor_count; ++i) fact[i] = fact[i - 1] * i;
		for (int row = 0; row < arguments_count; ++row)
		{
			table_values[0][row] = 0;
			for (int k = 0; k < tailor_count; ++k)
				table_values[0][row] += ykt[k] / fact[k] * pow(arguments[row] - x0, k);
		}


		double yk, xk, fkk, fkk2;

		for (int i = 0; i < N_shift - 1; ++i)
			table_values[1][i] = table_values[2][i] = table_values[3][i] = table_values[4][i] = 0;
		table_values[1][N_shift - 1] = table_values[2][N_shift - 1] = table_values[3][N_shift - 1] = table_values[4][N_shift - 1] = y0;

		// Метод Эйлера
		for (int row = N_shift; row < arguments_count; ++row)
		{
			xk = arguments[row - 1];
			yk = table_values[1][row - 1];

			function_formula->calculate(fkk, { xk, yk });

			table_values[1][row] = yk + h * fkk; // Формула левых прямоугольников
		}


		// Метод Эйлера 1
		for (int row = N_shift; row < arguments_count; ++row)
		{
			xk = arguments[row - 1];
			yk = table_values[2][row - 1];

			function_formula->calculate(fkk, { xk, yk });
			function_formula->calculate(fkk, { xk + h / 2, fkk * h / 2 + yk });

			table_values[2][row] = yk + h * fkk; // Формула средних прямоугольников
		}


		// Метод Эйлера 2
		for (int row = N_shift; row < arguments_count; ++row)
		{
			xk = arguments[row - 1];
			yk = table_values[3][row - 1];

			function_formula->calculate(fkk, { xk, yk });
			function_formula->calculate(fkk2, { xk + h, yk + h * fkk });

			table_values[3][row] = yk + h / 2 * (fkk + fkk2);
		}


		// Рунге-Кутта 4-го порядка
		double k[4];
		for (int row = N_shift; row < arguments_count; ++row)
		{
			xk = arguments[row - 1];
			yk = table_values[4][row - 1];


			function_formula->calculate(fkk, { xk, yk });
			k[0] = h * fkk;

			function_formula->calculate(fkk, { xk + h / 2, yk + k[0] / 2 });
			k[1] = h * fkk;

			function_formula->set_argument(1, yk + k[1] / 2);
			function_formula->calculate(fkk);
			k[2] = h * fkk;

			function_formula->calculate(fkk, { xk + h, yk + k[2] });
			k[3] = h * fkk;

			table_values[4][row] = yk + 1.0 / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);
		}



		// Экстраполяционный метод Адамса 4-го порядка по безразностной формуле
		// qk = h*f(xk,yk)
		// ym+1 = ym + 1.0 / 704 * (1901 * qm - 2774 * qm-1 + 2616 * qm-2 - 1274 * qm-3 + 251 * qm-4)
		// ym+1 = ym + 1.0 / 24 * (55 * qm - 59 * qm-1 + 37 * qm-2 - 9 * qm)

		double qk[4];

		// Копируем результаты, полученные по формуле Рунге-Кутта // Тейлора
		for (int row = 2; row < 5; ++row)
			table_values[5][row] = table_values[4][row];
		for (int row = 0; row < 2; ++row)
			table_values[5][row] = table_values[0][row];


		for (int row = 1; row < 4; ++row)
		{
			function_formula->calculate(fkk, { arguments[row], table_values[5][row] });
			qk[row] = fkk; // вычисляем 1, 2 и 3, после сдвига получим 0 1 2 3
		}

		for (int row = 4; row < arguments_count; ++row)
		{
			qk[0] = qk[1];
			qk[1] = qk[2];
			qk[2] = qk[3];
			function_formula->calculate(fkk, { arguments[row], table_values[5][row] });
			qk[3] = fkk;

			table_values[5][row + 1] = table_values[5][row] + h * 1. / 24. * (55. * qk[3] - 59. * qk[2] + 37. * qk[1] - 9. * qk[0]);
		}



		// Погрешности
		for (int column = 0; column < methods_count; ++column)
			for (int row = 0; row < arguments_count; ++row)
				table_errors[column][row] = fabs(solution_values[row] - table_values[column][row]);

		for (int column = 1; column < 5; ++column)
			for (int row = 0; row < N_shift - 1; ++row)
				table_errors[column][row] = 0;
	}



	if (isRPNCalculated && isArraysUpdated && arguments_count == N + N_shift && ImGui::TreeNode("Сводные данные"))
	{
		static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

		ImGui::Text("Узлы и приближённые значения решения");

		if (ImGui::BeginTable("table1", 8, table_flags))
		{
			ImGui::TableSetupColumn("Узлы", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Реальное значение", ImGuiTableColumnFlags_None);
			ImGui::TableSetupColumn("Тейлор", ImGuiTableColumnFlags_None); // k = -2
			ImGui::TableSetupColumn("Эйлер", ImGuiTableColumnFlags_None); // k = 1
			ImGui::TableSetupColumn("Эйлер 1", ImGuiTableColumnFlags_None); // k = 1
			ImGui::TableSetupColumn("Эйлер 2", ImGuiTableColumnFlags_None); // k = 1
			ImGui::TableSetupColumn("Рунге-Кутта", ImGuiTableColumnFlags_None); // k = 1
			ImGui::TableSetupColumn("Адамс", ImGuiTableColumnFlags_None); // k = 3
			ImGui::TableHeadersRow();

			for (int row = 0; row < arguments_count; row++)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%.8f", arguments[row]);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%.8f", solution_values[row]);
				for (int column = 2; column < methods_count + 2; ++column)
				{
					ImGui::TableSetColumnIndex(column);
					ImGui::Text("%.8f", table_values[column - 2][row]);
				}
			}
			ImGui::EndTable();
		}

		ImGui::Text("Абсолютные погрешности каждого метода");

		if (ImGui::BeginTable("table2", methods_count, table_flags))
		{
			ImGui::TableSetupColumn("Тейлор", ImGuiTableColumnFlags_None); // O(h)
			ImGui::TableSetupColumn("Эйлер", ImGuiTableColumnFlags_None); // Если C1, то O(h2)
			ImGui::TableSetupColumn("Эйлер 1", ImGuiTableColumnFlags_None); // O(h3) на одном шаге, O(h2) - на остальных
			ImGui::TableSetupColumn("Эйлер 2", ImGuiTableColumnFlags_None); // O(h3) на одном шаге, O(h2) - на остальных
			ImGui::TableSetupColumn("Рунге-Кутта", ImGuiTableColumnFlags_None); // O(h5) на одном шаге
			ImGui::TableSetupColumn("Адамс", ImGuiTableColumnFlags_None); // O(h6) на одном шаге
			ImGui::TableHeadersRow();

			for (int row = 0; row < arguments_count; row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < methods_count; ++column)
				{
					ImGui::TableSetColumnIndex(column);
					ImGui::Text("%.12f", table_errors[column][row]);
				}
			}
			ImGui::EndTable();
		}

		ImGui::TreePop();
	}

}