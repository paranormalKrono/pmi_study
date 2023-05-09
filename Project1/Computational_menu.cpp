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

	const int size_of_str = 256;
	const float begin_min{ 0.2f }, end_max{ 10000.0f };


	std::string queue_to_string(queue* q);

	void input_functions(char(*function_names)[size_of_str], int function_count);
	queue** get_function_rpns(char(*strs)[size_of_str], int count);
	std::string* get_rpns_strs(queue * *rpns, int count);
	void print_rpns_strs(std::string * strs, int count);

	void show_functions(int fixed_function_count, double begin, double end, queue** function_rpns, variable** variables, char(*function_names)[size_of_str]);

	variable** get_standart_variables();

	void functions_array_move(char(*new_function_names)[size_of_str], char(*function_names)[size_of_str],
		int l1, int l2, int fixed_function_count);

	void ComputationalMathematics()
	{
		// "ln(cos(x)+x*(2+pow(x,-x)))"
		// "(1-x+2*pow(x,x)-x*ln(x)-pow(x,x)*sin(x))/(x+2*pow(x,1+x)+pow(x,x)*cos(x))";
		// "-(x^(2*x)*sin(x)^2+(2*x^(x+1)*log(x)-4*x^(2*x)+x^x*(2*x-2))*sin(x)+((-x^(x+1)*cos(x))-2*x^(x+2))*log(x)^2+((2-2*x)*x^x*cos(x)-4*x^(x+2))*log(x)+x^(2*x)*cos(x)^2+(2*x^(2*x+1)+3*x^x)*cos(x)+4*x^(2*x)+x^x*((-2*x^2)+2*x+4)+x+1)/(x^(2*x)*cos(x)^2+(4*x^(2*x+1)+2*x^(x+1))*cos(x)+4*x^(2*x+2)+4*x^(x+2)+x^2)"
		
		static char(*function_names)[size_of_str] = (char(*)[size_of_str])malloc(sizeof(char[size_of_str]));
		static char(*main_function_name)[size_of_str] = function_names;
		input_functions(main_function_name, 1); 

		static int deriatives_count = 0;
		static int antiderivatives_count = 0;

		static int fixed_deriatives_count = 0;
		static int fixed_antiderivatives_count = 0;
		static int fixed_function_count = 1;
		static int *main_function_index = &fixed_antiderivatives_count;


		if (ImGui::TreeNode("Производные и первообразные"))
		{
			ImGui::DragInt("Количество первообразных функции", &antiderivatives_count, 0.5f, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragInt("Количество производных функции", &deriatives_count, 0.5f, 0, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

			if ((fixed_deriatives_count != deriatives_count || fixed_antiderivatives_count != antiderivatives_count)
				&& ImGui::Button("Обновить количество производных и первообразных функции"))
			{
				char(*new_function_names)[size_of_str]  = (char(*)[size_of_str])malloc(sizeof(char[size_of_str]) * (deriatives_count + antiderivatives_count + 1));

				functions_array_move(new_function_names, function_names, 
					antiderivatives_count - fixed_antiderivatives_count, 
					deriatives_count - fixed_deriatives_count, 
					fixed_function_count);

				free(function_names);
				function_names = new_function_names;

				fixed_deriatives_count = deriatives_count;
				fixed_antiderivatives_count = antiderivatives_count;
				fixed_function_count = fixed_deriatives_count + fixed_antiderivatives_count + 1;
				main_function_name = function_names + fixed_antiderivatives_count;
			}


			if (ImGui::TreeNode("Ввод первообразных"))
			{
				if (fixed_antiderivatives_count > 0)
					input_functions(function_names, fixed_antiderivatives_count);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Ввод производных"))
			{
				if (fixed_deriatives_count > 0)
					input_functions(main_function_name + 1, fixed_deriatives_count);
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		static queue** function_rpns;
		static std::string* function_rpns_strs;

		if (ImGui::Button("Получить обратную польскую нотацию для всех функций"))
		{
			function_rpns = get_function_rpns(function_names, fixed_function_count);
			function_rpns_strs = get_rpns_strs(function_rpns, fixed_function_count);
		}


		if (function_rpns && ImGui::TreeNode("Обратные польские нотации"))
		{
			print_rpns_strs(function_rpns_strs, fixed_function_count);
			ImGui::TreePop();
		}


		static variable** variables = get_standart_variables();

		if (!function_rpns) return;

		static float begin = begin_min, end = 4.0f;
		ImGui::DragFloatRange2("Промежуток", &begin, &end, 0.1f, begin_min, end_max, "A: %.4f", "B: %.4f", ImGuiSliderFlags_AlwaysClamp);

		show_functions(fixed_function_count, begin, end, function_rpns, variables, function_names);

		IntegrationMenu(fixed_antiderivatives_count, end, begin, variables, function_rpns, fixed_function_count, main_function_index);


	}


	void show_functions(int function_count, double begin, double end, queue** function_rpns, variable** variables, char(*function_names)[size_of_str])
	{
		static int new_pairs_count = 18;
		ImGui::DragInt("Количество пар аргумент-значение функции для просмотра", &new_pairs_count, 1, 1, INT32_MAX, "%d", ImGuiSliderFlags_AlwaysClamp);

		static double** values_array;
		static double* arguments_array;

		static int fixed_pairs_count = 1, fixed_function_count = 0;
		static float fixed_begin = begin, fixed_end = end;

		if ((fixed_function_count != function_count && fixed_pairs_count != new_pairs_count || fixed_begin != begin || fixed_end != end)
			&& ImGui::Button("Посчитать пары аргумент-значение"))
		{
			values_array = (double**)realloc(values_array, sizeof(double*) * function_count);

			for (int i = 0; i < fixed_function_count; ++i)
				values_array[i] = (double*)realloc(values_array[i], sizeof(double) * new_pairs_count);
			for (int i = fixed_function_count; i < function_count; ++i)
				values_array[i] = (double*)malloc(sizeof(double) * new_pairs_count);

			arguments_array = (double*)realloc(arguments_array, sizeof(double) * new_pairs_count);

			fixed_function_count = function_count;
			fixed_begin = begin;
			fixed_end = end;
			fixed_pairs_count = new_pairs_count;

			for (int i = 0; i < new_pairs_count; ++i)
				arguments_array[i] = begin + i * (end - begin) / (double)new_pairs_count;

			for (int i = 0; i < fixed_function_count; ++i)
				if (function_rpns[i])
					for (int j = 0; j < new_pairs_count; ++j)
					{
						variables[0]->value = arguments_array[j];
						values_array[i][j] = get_RPN_result(function_rpns[i], variables, 1);
					}
		}


		if (!arguments_array) return;

		if (ImGui::Button("Написать пары аргумент-значение в консоль"))
			for (int i = 0; i < fixed_function_count; ++i)
			{
				printf_s("argument-value pairs for %s\n", function_names[i]);
				for (int j = 0; j < fixed_pairs_count; ++j)
					printf_s("\t[%lf, %lf]\n", arguments_array[j], values_array[i][j]);
				printf_s("\n");
			}


		static ImVec2 size = { 1350, 700 };
		ImGui::DragFloat2("Размер окон графиков", (float*)&size, 0.5f, 100.0f, 2000.0f, "%.0f");

		static bool show_lines = true;
		static bool show_fills = true;
		static float fill_ref = 0;
		static int shade_mode = 2;
		static ImPlotShadedFlags flags = 0;
		ImGui::Checkbox("Линии", &show_lines); ImGui::SameLine();
		ImGui::Checkbox("Наполнение", &show_fills);
		if (show_fills) {
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
		}


		if (ImPlot::BeginPlot("График функций", size))
		{
			ImPlot::SetupAxes("X", "Y");
			ImPlot::SetupAxesLimits(begin, end, -10, 10);

			for (int i = 0; i < fixed_function_count; ++i)
			{
				if (show_fills) {
					ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
					ImPlot::PlotShaded(function_names[i], arguments_array, values_array[i], fixed_pairs_count,
						shade_mode == 0 ? -INFINITY : shade_mode == 1 ? INFINITY : fill_ref, flags);
					ImPlot::PopStyleVar();
				}
				if (show_lines) {
					ImPlot::PlotLine(function_names[i], arguments_array, values_array[i], fixed_pairs_count);
				}
			}

			ImPlot::EndPlot();
		}
	}

	void functions_array_move(char(*new_function_names)[size_of_str], char(*function_names)[size_of_str],
		int l1, int l2, int fixed_function_count)
	{
		// Перенос данных
		int t1 = 0, t2 = 0, t3 = 0;

		t3 = l1 + l2;
		if (t3 > 0) t3 = 0;

		if (l1 <= 0) t2 -= l1;
		else t1 += l1;

		memmove_s(
			new_function_names + t1,
			sizeof(char[size_of_str]) * (fixed_function_count + t3), // (deriatives_count + antiderivatives_count + 1),
			function_names + t2,
			sizeof(char[size_of_str]) * (fixed_function_count + t3));


		for (int i = 0; i < t1; ++i) new_function_names[i][0] = '\0';
		for (int i = fixed_function_count + t1; i < fixed_function_count + l1 + l2; ++i) new_function_names[i][0] = '\0';
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

	void input_functions(char (*function_names)[size_of_str], int function_count)
	{
		for (int i = 0; i < function_count; ++i)
		{
			char buf[32];
			sprintf(buf, "function %d", i);
			ImGui::InputText(buf, function_names[i], size_of_str);
		}
	}

	queue** get_function_rpns(char(*strs)[size_of_str], int count)
	{
		queue** rpns = (queue**)malloc(sizeof(queue*) * count);
		for (int i = 0; i < count; ++i)
		{
			rpns[i] = get_Reverse_Polish_Notation(strs[i]);
		}
		return rpns;
	}

	std::string* get_rpns_strs(queue** rpns, int count)
	{
		std::string* strs = new std::string[count];

		for (int i = 0; i < count; ++i)
		{
			strs[i] = queue_to_string(rpns[i]);
		}

		return strs;
	}

	void print_rpns_strs(std::string* strs, int count)
	{
		for (int i = 0; i < count; ++i)
			ImGui::Text(strs[i].c_str());
	}

}