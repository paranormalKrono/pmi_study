#include "Application.h"
#include <stdio.h>
#include <math.h>

extern "C"
{
#include "Tokens.h"
#include "Lessons/sq/queue_list.h"
#include "Shunting_yard.h"
#include "numerical_integration.h"

	//#include <Python.h>
	//#include "python_gui.h"
}

#pragma execution_character_set("utf-8")

namespace MyApp
{

	static void PushStyleCompact();
	static void PopStyleCompact();

	void IntegrationMenu(int fixed_antiderivatives_count, double end, double begin, variable** variables, queue** function_rpns, int fixed_function_count, int* main_function_index)
	{
		// Посчитать по формулам для A B делённого на отрезки
		static double integration_real_value = 0;
		static double left_rectangle_value = 0;
		static double right_rectangle_value = 0;
		static double middle_rectangle_value = 0;
		static double trapezoid_rectangle_value = 0;
		static double simpsons_value;
		static double _38_value = 0;

		static double integration_real_error = 0;
		static double right_rectangle_error = 0;
		static double left_rectangle_error = 0;
		static double middle_rectangle_error = 0;
		static double trapezoid_rectangle_error = 0;
		static double simpsons_error = 0;
		static double _38_error = 0;

		static double integration_error = 0;
		static double right_rectangle_real_error = 0;
		static double left_rectangle_real_error = 0;
		static double middle_rectangle_real_error = 0;
		static double trapezoid_rectangle_real_error = 0;
		static double simpsons_real_error = 0;
		static double _38_real_error = 0;

		const double* value_table[7][3] = {
			{&integration_real_value, &integration_real_error, &integration_error},
			{&left_rectangle_value, &left_rectangle_error, &right_rectangle_real_error},
			{&right_rectangle_value, &right_rectangle_error, &left_rectangle_real_error},
			{&middle_rectangle_value, &middle_rectangle_error, &middle_rectangle_real_error},
			{&trapezoid_rectangle_value, &trapezoid_rectangle_error, &trapezoid_rectangle_real_error},
			{&simpsons_value, &simpsons_error, &simpsons_real_error},
			{&_38_value, &_38_error, &_38_real_error}
		};

		static double fd_max = 0.5f;
		if (fixed_antiderivatives_count > 0 && ImGui::TreeNode("Интегрирование по формулам КФ"))
		{
			const char* names_table[7] = {
				{"Настоящее значение"},
				{"Левые прямоугольники"},
				{"Правые прямоугольники"},
				{"Средние прямоугольники"},
				{"Трапеции"},
				{"Симпсон"},
				{"Три восьмых"}
			};

			static double FA, FB, fA, fB, fAB2, fA1B, fA2B;

			if (ImGui::Button("Посчитать интегралы по формулам КФ"))
			{
				double l = end - begin;
				double l2 = pow(l, 2);
				double l3 = pow(l, 3);
				double l5 = pow(l, 5);

				variables[0]->value = begin;
				fA = get_RPN_result(function_rpns[*main_function_index], variables, 1);
				FA = get_RPN_result(function_rpns[*main_function_index - 1], variables, 1);
				variables[0]->value = end;
				fB = get_RPN_result(function_rpns[*main_function_index], variables, 1);
				FB = get_RPN_result(function_rpns[*main_function_index - 1], variables, 1);

				variables[0]->value = begin + l * 0.5;
				fAB2 = get_RPN_result(function_rpns[*main_function_index], variables, 1);
				variables[0]->value = begin + l / 3.0;
				fA1B = get_RPN_result(function_rpns[*main_function_index], variables, 1);
				variables[0]->value = begin + 2.0 * l / 3.0;
				fA2B = get_RPN_result(function_rpns[*main_function_index], variables, 1);

				integration_real_value = FB - FA;
				left_rectangle_value = integration_left_rectangle(l, fA);
				right_rectangle_value = integration_right_rectangle(l, fB);
				middle_rectangle_value = integration_middle_rectangle(l, fAB2);
				trapezoid_rectangle_value = integration_trapezoid(l, fA, fB);
				simpsons_value = integration_simpsons(l, fA, fAB2, fB);
				_38_value = integration_38(l, fA, fA1B, fA2B, fB);

				left_rectangle_error = abs(integration_left_absolute_error(l2, fd_max));
				right_rectangle_error = abs(integration_right_absolute_error(l2, fd_max));
				middle_rectangle_error = abs(integration_middle_absolute_error(l3, fd_max));
				trapezoid_rectangle_error = abs(integration_trapezoid_absolute_error(l3, fd_max));
				simpsons_error = abs(integration_simpsons_absolute_error(l5, fd_max));
				_38_error = abs(integration_38_absolute_error(l5, fd_max));

				right_rectangle_real_error = abs(integration_real_value - left_rectangle_value);
				left_rectangle_real_error = abs(integration_real_value - right_rectangle_value);
				middle_rectangle_real_error = abs(integration_real_value - middle_rectangle_value);
				trapezoid_rectangle_real_error = abs(integration_real_value - trapezoid_rectangle_value);
				simpsons_real_error = abs(integration_real_value - simpsons_value);
				_38_real_error = abs(integration_real_value - _38_value);
			}

			static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;

			PushStyleCompact();
			ImGui::CheckboxFlags("Resizable", &flags, ImGuiTableFlags_Resizable);
			ImGui::CheckboxFlags("NoBordersInBodyUntilResize", &flags, ImGuiTableFlags_NoBordersInBodyUntilResize);
			PopStyleCompact();

			if (ImGui::BeginTable("table1", 4, flags))
			{
				ImGui::TableSetupColumn("Название КФ", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("По КФ", ImGuiTableColumnFlags_None);
				ImGui::TableSetupColumn("Абсолютная погрешность", ImGuiTableColumnFlags_None);
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
						ImGui::Text("%lf", *(value_table[row][column]));
					}
				}
				ImGui::EndTable();
			}

			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Интегрирование по составным формулам КФ"))
		{
			static int divisions_count = 20;
			ImGui::DragInt("Количество делений отрезка для составных КФ", &divisions_count, 1, 1, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

			static double W, Q, h;
			static double* divised_arguments_array = (double*)malloc(sizeof(double) * divisions_count);
			static double** divised_values_array = (double**)malloc(sizeof(double*) * fixed_function_count);


			ImGui::TreePop();
		}
	}

	// Make the UI compact because there are so many fields
	static void PushStyleCompact()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.60f)));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.60f)));
	}

	static void PopStyleCompact()
	{
		ImGui::PopStyleVar(2);
	}

}