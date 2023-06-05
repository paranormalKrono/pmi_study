#include "Application.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <imgui_stdlib.h>

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

	std::string queue_to_string(queue* q);

	void input_functions(std::vector<std::string>::iterator start, int count, int start_number);

	void show_functions(double begin, double end, std::vector<std::string> function_names, std::vector<queue*> function_rpns, variable** variables);

	variable** get_standart_variables();

	void ComputationalMathematics()
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
		static variable** variables = get_standart_variables();

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
				/*if (function_rpns[i])
					queue_free(function_rpns[i]);*/

				function_rpns[i] = get_Reverse_Polish_Notation(function_names.at(i).data());
				function_rpns_strs[i] = queue_to_string(function_rpns.at(i));
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
		//ImGui::DragFloat2("Размер окон графиков", (float*)&size, 0.5f, 100.0f, 2000.0f, "%.0f");

		static bool show_lines = true;
		static bool show_fills = true;
		static float fill_ref = 0;
		static int shade_mode = 2;
		static ImPlotShadedFlags flags = 0;
		//ImGui::Checkbox("Линии", &show_lines); ImGui::SameLine();
		//ImGui::Checkbox("Наполнение", &show_fills);
		/*if (show_fills) {
			ImGui::SameLine();
			if (ImGui::RadioButton("To -INF", shade_mode == 0))
				shade_mode = 0;
			ImGui::SameLine();
			if (ImGui::RadioButton("To +INF", shade_mode == 1))
				shade_mode = 1;
			ImGui::SameLine();
			if (ImGui::RadioButton("To Ref", shade_mode == 2))
				shade_mode = 2;
			if (shade_mode == 2) {
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				ImGui::DragFloat("##Ref", &fill_ref, 1, -100, 500);
			}
		}*/


		if (ImPlot::BeginPlot("График функций", size))
		{
			ImPlot::SetupAxes("X", "Y");
			ImPlot::SetupAxesLimits(begin, end, -10, 10);

			for (int i = 0; i < fixed_function_count; ++i)
			{
				if (i < function_names.size())
				{
					if (show_fills) {
						ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
						ImPlot::PlotShaded(function_names[i].data(), arguments_array, values_array.at(i), fixed_pairs_count,
							shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref, flags);
						ImPlot::PopStyleVar();
					}
					if (show_lines) {
						ImPlot::PlotLine(function_names[i].data(), arguments_array, values_array.at(i), fixed_pairs_count);
					}
				}
			}

			ImPlot::EndPlot();
		}
	}

	variable** get_standart_variables()
	{
		variable** variables = (variable**)malloc(sizeof(variable*) * 1);
		if (!variables) return NULL;
		char* x = (char*)malloc(sizeof(char) * 2);
		x[0] = 'x';
		x[1] = '\0';
		variables[0] = variable_init(x, 0);

		return variables;
	}

	std::string queue_to_string(queue* q)
	{
		printf_s("--- Обратная польская нотация ---\n");
		std::string s = "";
		queue* c = queue_clone(q);
		element_t e;
		token* t = token_alloc();
		while (queue_pop(c, &e))
		{
			t = static_cast<token*>(e);
			s += " ";
			s += t->name;
			printf_s("%s ", t->name);
		}
		printf_s("\n\n");
		return s;
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

}