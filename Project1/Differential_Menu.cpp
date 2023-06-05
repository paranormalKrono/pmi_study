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

	variable** get_variables_x();
	variable** get_variables_xy();

	//void input_functions(std::vector<std::string>::iterator start, int count, int start_number);

	void DifferentialMathematics()
	{
		ImGui::Text("y'(x) = f(x,y) \t y(x0) = y0");
		static std::string function_name = "-y + 2*pow(y,2) + 1";
		static std::string solution_name = "pow(x,4)";
		ImGui::InputText("Функция f(x,y)", &(function_name));
		ImGui::InputText("Функция y(x)", &(solution_name));

		static bool isNewFunctionRPN = true;
		static queue* function_rpn, *solution_rpn;
		static variable** variables_x = get_variables_x();
		static variable** variables_xy = get_variables_xy();

		static float x0 = 0;

		ImGui::SliderFloat("Введите x0", &x0, -FLT_MAX, FLT_MAX, "%.3f");


		if (ImGui::Button("Вычислить обратную польскую нотацию"))
		{
			function_rpn = get_Reverse_Polish_Notation(function_name.data());

			isNewFunctionRPN = false;
		}



		/*if (ImGui::TreeNode("Решение задачи Коши"))
		{
			if (!isNewFunctionRPN)
				CauchyMenu(B, A, variables, function_rpns, function_names, main_function_index);

			ImGui::TreePop();
		}*/

	}

	variable** get_variables_x()
	{
		variable** variables = (variable**)malloc(sizeof(variable*) * 1);
		if (!variables) return NULL;
		char* x = (char*)malloc(sizeof(char) * 2);
		x[0] = 'x';
		x[1] = '\0';
		variables[0] = variable_init(x, 0);

		return variables;
	}

	variable** get_variables_xy()
	{
		variable** variables = (variable**)malloc(sizeof(variable*) * 2);
		if (!variables) return NULL;
		char* x = (char*)malloc(sizeof(char) * 2);
		x[0] = 'x';
		x[1] = '\0';
		variables[0] = variable_init(x, 0);
		char* y = (char*)malloc(sizeof(char) * 2);
		y[0] = 'y';
		y[1] = '\0';
		variables[1] = variable_init(y, 0);

		return variables;
	}


	/*void input_functions(std::vector<std::string>::iterator start, int count, int start_number)
	{
		for (int i = 0; i < count; ++i)
		{
			char buf[32];
			sprintf_s(buf, "Функция %d", start_number + i);

			ImGui::InputText(buf, &(start[i]));
		}
	}*/

}