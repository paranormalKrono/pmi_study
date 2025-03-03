#include "MainApp.h"
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

void ComputationalMenu()
{

	if (ImGui::TreeNode("СЛАУ"))
	{
		LinearSystemsMenu();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Дифференцирование"))
	{
		DifferentialMenu();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Интегрирование"))
	{
		IntegrationMenu();
		ImGui::TreePop();
	}

}