#include "MainApp.h"

#include <imgui_stdlib.h>

#pragma execution_character_set("utf-8")

void IntegrationMenu()
{

	if (ImGui::TreeNode("Решение интегрального уравнения второго рода"))
	{
		IntegrationEquationsMenu();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Остальное"))
	{
		Integration();
		ImGui::TreePop();
	}

}