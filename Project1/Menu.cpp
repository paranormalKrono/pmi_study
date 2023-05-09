#include "Application.h"

namespace MyApp
{
	const int start_menu_sections_count = 1;
	const char* start_menu_texts[] 
	{
		"Numerical mathematics"
	};
	void (*menus[])()
	{
		ComputationalMathematics
	};

	void ShowMenu()
	{
		static int selected = 0;
		if (ImGui::TreeNode("Menu selection"))
		{
			for (int i = 0; i < start_menu_sections_count; i++)
			{
				if (ImGui::Selectable(start_menu_texts[i], selected == i))
					selected = i;
			}


			ImGui::TreePop();
		}

		if (selected > -1 && selected < start_menu_sections_count)
			menus[selected]();

	}
}