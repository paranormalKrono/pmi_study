#include "MainApp.h"

#include <imgui_stdlib.h>

#pragma execution_character_set("utf-8")

void IntegrationMenu()
{

	if (ImGui::TreeNode("������� ������������� ��������� ������� ����"))
	{
		IntegrationEquationsMenu();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("���������"))
	{
		Integration();
		ImGui::TreePop();
	}

}