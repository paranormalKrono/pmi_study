#include "IntegrationSystems.h"
#include <stack>

#pragma execution_character_set("utf-8")

void IntegrationEquationsMenu()
{

}

//void IntegrationEquationsMenu()
//{
//	static std::string 
//		function_def { "2" },
//		kernel_def { "sin(x-2t)" },
//		solution_def { "2-3sin(x)" };
//	static float 
//		A{ 0 }, B{ M_PI_2 };
//	static double
//		h{ 0 }, lambda { -3 };
//	static int
//		N{ 5 };
//
//	ImGui::InputText("Функция f(x)", &(function_def));	
//	ImGui::InputText("Функция K(x,t)", &(kernel_def));
//	ImGui::InputText("Функция phi(x)", &(solution_def));
//	ImGui::DragFloatRange2("Промежуток", &A, &B, 1., -FLT_MAX, FLT_MAX, "%.5f");
//	ImGui::InputInt("Введите N", &N);
//
//	static int 
//		arguments_count{ N }, variables_count = 3;
//	static bool 
//		isRPNCalculated{ false }, isArraysUpdated{ false };
//	static variable
//		** variables { formula_rpn::get_standard_variables(variables_count, "x", "t") };
//
//	static formula_rpn
//		* function_formula { new formula_rpn(function_def, variables_count, variables) }, 
//		* kernel_formula { new formula_rpn(kernel_def, variables_count, variables) }, 
//		* solution_formula { new formula_rpn(solution_def, variables_count, variables) };
//
//
//	static std::vector<double> 
//		arguments, solution_values, errors;
//	static alglib::real_1d_array 
//		calculated_values;
//
//	if (ImGui::Button("Вычислить обратную польскую нотацию"))
//	{
//		function_formula->recalculate_rpn();
//		kernel_formula->recalculate_rpn();
//		solution_formula->recalculate_rpn();
//		isRPNCalculated = true;
//	}
//
//	if (ImGui::Button("Обновить массивы"))
//	{
//		isArraysUpdated = true;
//		arguments_count = N + 1;
//		h = (B - A) / N;
//
//		arguments.resize(arguments_count);
//
//		calculated_values.setlength(arguments_count);
//		solution_values.resize(arguments_count);
//		errors.resize(arguments_count);
//	}
//
//	if (isRPNCalculated && arguments_count == N + 1 && ImGui::Button("Вычислить"))
//	{
//		alglib::real_1d_array fk;
//		fk.setlength(arguments_count);
//
//		function_formula->set_arguments({ 0, 0 });
//
//		for (int i = 0; i < arguments_count; ++i)
//		{
//			arguments[i] = h * i;
//
//			variables[0]->value = arguments[i];
//			solution_values[i] = get_RPN_result(solution_rpn, variables, variables_count);
//
//			function_formula->calculate(fk[i], { arguments[i] });
//		}
//
//		fk[0] = A;
//		fk[N] = B;
//
//		alglib::real_2d_array An;
//		matrix_identity(An, arguments_count);
//
//		h = 1. / (h * h);
//		for (int row = 1; row < N; ++row)
//		{
//			An[row][row + 1] = An[row][row - 1] = -h;
//			An[row][row] = 1. + 2. * h;
//		}
//
//		print_matrix_and_vector(An, fk, arguments_count);
//
//		PassingMethod(An, fk, arguments_count, calculated_values);
//
//		//alglib::rmatrixsolvefast(An, fk);
//		//print_vector(fk);
//
//		for (int i = 0; i < arguments_count; ++i)
//			errors[i] = fabs(solution_values[i] - calculated_values[i]);
//	}
//
//
//
//	if (isRPNCalculated && isArraysUpdated && arguments_count == N + 1 && ImGui::TreeNode("Сводные данные"))
//	{
//		static ImGuiTableFlags table_flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_SizingFixedFit;
//
//		ImGui::Text("Узлы и приближённые значения решения");
//
//		if (ImGui::BeginTable("table1", 4, table_flags, ImVec2(1400, 100)))
//		{
//			ImGui::TableSetupColumn("Узлы", ImGuiTableColumnFlags_None);
//			ImGui::TableSetupColumn("Приближённые значения решения", ImGuiTableColumnFlags_None);
//			ImGui::TableSetupColumn("Реальные значения решения", ImGuiTableColumnFlags_None);
//			ImGui::TableSetupColumn("Абсолютная погрешность", ImGuiTableColumnFlags_None);
//			ImGui::TableHeadersRow();
//
//			for (int row = 0; row < arguments_count; row++)
//			{
//				ImGui::TableNextRow();
//				ImGui::TableSetColumnIndex(0);
//				ImGui::Text("%.12f", arguments[row]);
//				ImGui::TableSetColumnIndex(1);
//				ImGui::Text("%.12f", calculated_values[row]);
//				ImGui::TableSetColumnIndex(2);
//				ImGui::Text("%.12f", solution_values[row]);
//				ImGui::TableSetColumnIndex(3);
//				ImGui::Text("%.12f", errors[row]);
//			}
//			ImGui::EndTable();
//		}
//
//		ImGui::TreePop();
//	}
//}