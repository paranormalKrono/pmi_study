#include "Node_menu.h"

#include <stdio.h>

NodeMenu NM = NodeMenu();

void NodeMenu_() {

	auto& io = ImGui::GetIO();

	NM.OnFrame(io.DeltaTime);
}