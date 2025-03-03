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
}

#pragma execution_character_set("utf-8")

void DifferentialMenu()
{
	
	if (ImGui::TreeNode("Краевая задача для ОДУ 2-го порядка"))
	{
		BoundaryMenu();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Коши"))
	{
		CauchyMenu();
		ImGui::TreePop();
	}

}