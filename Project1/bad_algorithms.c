///*
/* 
* 

		for (auto& node : m_Nodes)
		{
			if (node.Type != NodeType::Tree)
				continue;

			const float rounding = 5.0f;
			const float padding = 12.0f;

			const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

			ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
			ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
			ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
			ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

			ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
			ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
			ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
			ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
			ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
			ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
			ed::PushStyleVar(ed::StyleVar_PinRadius, 5.0f);
			ed::BeginNode(node.ID);

			ImGui::BeginVertical(node.ID.AsPointer());
			ImGui::BeginHorizontal("inputs");
			ImGui::Spring(0, padding * 2);

			ImRect inputsRect;
			int inputAlpha = 200;
			if (!node.Inputs.empty())
			{
				auto& pin = node.Inputs[0];
				ImGui::Dummy(ImVec2(0, padding));
				ImGui::Spring(1, 0);
				inputsRect = ImGui_GetItemRect();

				ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
				ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);

				ed::PushStyleVar(ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersBottom);

				ed::BeginPin(pin.ID, ed::PinKind::Input);
				ed::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
				ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
				ed::EndPin();
				ed::PopStyleVar(3);

				if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
					inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
			}
			else
				ImGui::Dummy(ImVec2(0, padding));

			ImGui::Spring(0, padding * 2);
			ImGui::EndHorizontal();

			ImGui::BeginHorizontal("content_frame");
			ImGui::Spring(1, padding);

			ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
			ImGui::Dummy(ImVec2(160, 0));
			ImGui::Spring(1);
			ImGui::TextUnformatted(node.Name.c_str());
			ImGui::Spring(1);
			ImGui::EndVertical();
			auto contentRect = ImGui_GetItemRect();

			ImGui::Spring(1, padding);
			ImGui::EndHorizontal();

			ImGui::BeginHorizontal("outputs");
			ImGui::Spring(0, padding * 2);

			ImRect outputsRect;
			int outputAlpha = 200;
			if (!node.Outputs.empty())
			{
				auto& pin = node.Outputs[0];
				ImGui::Dummy(ImVec2(0, padding));
				ImGui::Spring(1, 0);
				outputsRect = ImGui_GetItemRect();

				ed::PushStyleVar(ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersTop);

				ed::BeginPin(pin.ID, ed::PinKind::Output);
				ed::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
				ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
				ed::EndPin();
				ed::PopStyleVar();

				if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
					outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
			}
			else
				ImGui::Dummy(ImVec2(0, padding));

			ImGui::Spring(0, padding * 2);
			ImGui::EndHorizontal();

			ImGui::EndVertical();

			ed::EndNode();
			ed::PopStyleVar(7);
			ed::PopStyleColor(4);

			auto drawList = ed::GetNodeBackgroundDrawList(node.ID);

			//const auto fringeScale = ImGui::GetStyle().AntiAliasFringeScale;
			//const auto unitSize    = 1.0f / fringeScale;

			//const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
			//{
			//    if ((col >> 24) == 0)
			//        return;
			//    drawList->PathRect(a, b, rounding, rounding_corners);
			//    drawList->PathStroke(col, true, thickness);
			//};

			const auto    topRoundCornersFlags = ImDrawFlags_RoundCornersTop;
			const auto bottomRoundCornersFlags = ImDrawFlags_RoundCornersBottom;

			drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
			//ImGui::PopStyleVar();
			drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
				IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
			//ImGui::PopStyleVar();
			drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			drawList->AddRect(
				contentRect.GetTL(),
				contentRect.GetBR(),
				IM_COL32(48, 128, 255, 100), 0.0f);
			//ImGui::PopStyleVar();
		}


		for (auto& node : m_Nodes)
		{
			if (node.Type != NodeType::Houdini)
				continue;

			const float rounding = 10.0f;
			const float padding = 12.0f;


			ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(229, 229, 229, 200));
			ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
			ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(229, 229, 229, 60));
			ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));

			const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

			ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
			ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
			ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
			ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
			ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
			ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
			ed::PushStyleVar(ed::StyleVar_PinRadius, 6.0f);
			ed::BeginNode(node.ID);

			ImGui::BeginVertical(node.ID.AsPointer());
			if (!node.Inputs.empty())
			{
				ImGui::BeginHorizontal("inputs");
				ImGui::Spring(1, 0);

				ImRect inputsRect;
				int inputAlpha = 200;
				for (auto& pin : node.Inputs)
				{
					ImGui::Dummy(ImVec2(padding, padding));
					inputsRect = ImGui_GetItemRect();
					ImGui::Spring(1, 0);
					inputsRect.Min.y -= padding;
					inputsRect.Max.y -= padding;

					const auto allRoundCornersFlags = ImDrawFlags_RoundCornersAll;

					//ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
					//ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);
					ed::PushStyleVar(ed::StyleVar_PinCorners, allRoundCornersFlags);

					ed::BeginPin(pin.ID, ed::PinKind::Input);
					ed::PinPivotRect(inputsRect.GetCenter(), inputsRect.GetCenter());
					ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
					ed::EndPin();
					//ed::PopStyleVar(3);
					ed::PopStyleVar(1);

					auto drawList = ImGui::GetWindowDrawList();
					drawList->AddRectFilled(inputsRect.GetTL(), inputsRect.GetBR(),
						IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, allRoundCornersFlags);
					drawList->AddRect(inputsRect.GetTL(), inputsRect.GetBR(),
						IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, allRoundCornersFlags);

					if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
						inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
				}

				ImGui::Spring(1, 0);
				ImGui::EndHorizontal();
			}

			ImGui::BeginHorizontal("content_frame");
			ImGui::Spring(1, padding);

			ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
			ImGui::Dummy(ImVec2(160, 0));
			ImGui::Spring(1);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::TextUnformatted(node.Name.c_str());
			ImGui::PopStyleColor();
			ImGui::Spring(1);
			ImGui::EndVertical();
			auto contentRect = ImGui_GetItemRect();

			ImGui::Spring(1, padding);
			ImGui::EndHorizontal();

			if (!node.Outputs.empty())
			{
				ImGui::BeginHorizontal("outputs");
				ImGui::Spring(1, 0);

				ImRect outputsRect;
				int outputAlpha = 200;
				for (auto& pin : node.Outputs)
				{
					ImGui::Dummy(ImVec2(padding, padding));
					outputsRect = ImGui_GetItemRect();
					ImGui::Spring(1, 0);
					outputsRect.Min.y += padding;
					outputsRect.Max.y += padding;

					const auto allRoundCornersFlags = ImDrawFlags_RoundCornersAll;
					const auto topRoundCornersFlags = ImDrawFlags_RoundCornersTop;

					ed::PushStyleVar(ed::StyleVar_PinCorners, topRoundCornersFlags);
					ed::BeginPin(pin.ID, ed::PinKind::Output);
					ed::PinPivotRect(outputsRect.GetCenter(), outputsRect.GetCenter());
					ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
					ed::EndPin();
					ed::PopStyleVar();


					auto drawList = ImGui::GetWindowDrawList();
					drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR(),
						IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, allRoundCornersFlags);
					drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR(),
						IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, allRoundCornersFlags);


					if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
						outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
				}

				ImGui::EndHorizontal();
			}

			ImGui::EndVertical();

			ed::EndNode();
			ed::PopStyleVar(7);
			ed::PopStyleColor(4);

			// auto drawList = ed::GetNodeBackgroundDrawList(node.ID);

			//const auto fringeScale = ImGui::GetStyle().AntiAliasFringeScale;
			//const auto unitSize    = 1.0f / fringeScale;

			//const auto ImDrawList_AddRect = [](ImDrawList* drawList, const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners, float thickness)
			//{
			//    if ((col >> 24) == 0)
			//        return;
			//    drawList->PathRect(a, b, rounding, rounding_corners);
			//    drawList->PathStroke(col, true, thickness);
			//};

			//drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
			//    IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			//drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
			//    IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
			//ImGui::PopStyleVar();
			//drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
			//    IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
			////ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			//drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
			//    IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
			////ImGui::PopStyleVar();
			//drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
			//drawList->AddRect(
			//    contentRect.GetTL(),
			//    contentRect.GetBR(),
			//    IM_COL32(48, 128, 255, 100), 0.0f);
			//ImGui::PopStyleVar();
		}

//void NodeMenu()
//{
//    static bool firstframe = true; // Used to position the nodes on startup
//
//    if (firstframe)
//    {
//        ed::Config config;
//        config.SettingsFile = "Simple.json";
//        m_Editor = ed::CreateEditor(&config);
//    }
//
//    auto& io = ImGui::GetIO();
//
//    ed::SetCurrentEditor(m_Editor);
//
//
//    // Node Editor Widget
//    ed::Begin("Node window", ImVec2(0.0, 0.0f));
//    int uniqueId = 1;
//
//    ed::NodeId nodeMatrix_Id = uniqueId++;
//    ed::PinId  nodeB_InputPinId1 = uniqueId++;
//    ed::PinId  nodeB_InputPinId2 = uniqueId++;
//    ed::PinId  nodeB_OutputPinId = uniqueId++;
//
//    if (firstframe)
//        ed::SetNodePosition(nodeMatrix_Id, ImVec2(210, 60));
//
//    ed::BeginNode(nodeMatrix_Id);
//    ImGui::Text("Matrix");
//    ImGuiEx_BeginColumn();
//    ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
//    ImGui::Text("-> In1");
//    ed::EndPin();
//    ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
//    ImGui::Text("-> In2");
//    ed::EndPin();
//    ImGuiEx_NextColumn();
//    ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
//    ImGui::Text("Out ->");
//    ed::EndPin();
//    ImGuiEx_EndColumn();
//
//    // Use AlignTextToFramePadding() to align text baseline to the baseline of framed elements (otherwise a Text+SameLine+Button sequence will have the text a little too high by default)
//    ImGui::AlignTextToFramePadding();
//    ImGui::Text("Hold to repeat:");
//    ImGui::SameLine();
//
//    // The input widgets also require you to manually disable the editor shortcuts so the view doesn't fly around.
//    // (note that this is a per-frame setting, so it disables it for all text boxes.  I left it here so you could find it!)
//    ed::EnableShortcuts(!io.WantTextInput);
//
//    // The input widgets require some guidance on their widths, or else they're very large. (note matching pop at the end).
//    ImGui::PushItemWidth(400);
//    static char str1[128] = "";
//    ImGui::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_ARRAYSIZE(str1));
//
//    static float f0 = 0.001f;
//    ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");
//
//    static float f1 = 1.00f, f2 = 0.0067f;
//    ImGui::DragFloat("drag float", &f1, 0.005f);
//    ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
//    ImGui::PopItemWidth();
//
//    ed::EndNode();
//
//
//
//    auto popup_id = uniqueId++;
//    ed::BeginNode(popup_id);
//
//
//    ImGui::Text("Tool Tip & Pop-up Demo");
//    ed::BeginPin(uniqueId++, ed::PinKind::Input);
//    ImGui::Text("-> In");
//    ed::EndPin();
//    ImGui::SameLine();
//    ImGui::Dummy(ImVec2(120, 0)); // Hacky magic number to space out the output pin.
//    ImGui::SameLine();
//    ed::BeginPin(uniqueId++, ed::PinKind::Output);
//    ImGui::Text("Out ->");
//    ed::EndPin();
//
//    // Tooltip example
//    ImGui::Text("Hover over me");
//    static bool do_tooltip = false;
//    do_tooltip = ImGui::IsItemHovered() ? true : false;
//    ImGui::SameLine();
//    ImGui::Text("- or me");
//    static bool do_adv_tooltip = false;
//    do_adv_tooltip = ImGui::IsItemHovered() ? true : false;
//
//    ed::EndNode();
//
//    if (firstframe) {
//        ed::SetNodePosition(popup_id, ImVec2(610, 20));
//    }
//
//    // --------------------------------------------------------------------------------------------------
//    // Deferred Pop-up Section
//
//    // This entire section needs to be bounded by Suspend/Resume!  These calls pop us out of "node canvas coordinates"
//    // and draw the popups in a reasonable screen location.
//    ed::Suspend();
//
//    // Handle the simple tooltip
//    if (do_tooltip)
//        ImGui::SetTooltip("I am a tooltip");
//
//    // Handle the advanced tooltip
//    if (do_adv_tooltip) {
//        ImGui::BeginTooltip();
//        ImGui::Text("I am a fancy tooltip");
//        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
//        ImGui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
//        ImGui::EndTooltip();
//    }
//
//    ed::Resume();
//    // End of "Deferred Pop-up section"
//
//
//
//    // ==================================================================================================
//    // Link Drawing Section
//
//    for (auto& linkInfo : m_Links)
//        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
//
//    // ==================================================================================================
//    // Interaction Handling Section
//    // This was coppied from BasicInteration.cpp. See that file for commented code.
//
//    // Handle creation action ---------------------------------------------------------------------------
//    if (ed::BeginCreate())
//    {
//        ed::PinId inputPinId, outputPinId;
//        if (ed::QueryNewLink(&inputPinId, &outputPinId))
//        {
//            if (inputPinId && outputPinId) // valid
//            {
//                if (ed::AcceptNewItem())
//                {
//                    m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });
//                    ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
//                }
//            }
//
//            // You may choose to reject connection between these nodes
//            // by calling ed::RejectNewItem(). This will allow editor to give
//            // visual feedback by changing link thickness and color.
//        }
//    }
//    ed::EndCreate();
//
//    // Handle deletion action ---------------------------------------------------------------------------
//    if (ed::BeginDelete())
//    {
//        ed::LinkId deletedLinkId;
//        while (ed::QueryDeletedLink(&deletedLinkId))
//        {
//            if (ed::AcceptDeletedItem())
//            {
//                for (auto& link : m_Links)
//                {
//                    if (link.Id == deletedLinkId)
//                    {
//                        m_Links.erase(&link);
//                        break;
//                    }
//                }
//            }
//
//            // You may reject link deletion by calling:
//            // ed::RejectDeletedItem();
//        }
//    }
//    ed::EndDelete();
//
//    ed::End();
//    ed::SetCurrentEditor(nullptr);
//    firstframe = false;
//}



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


			L[0][0] = sqrt(A[0][0]);
			for (int i = 1; i < N; ++i)
				L[i][0] = A[i][0] / L[0][0];

			for (int i = 1; i < N; ++i)
			{
				sqr_sum = A[i][i];
				for (int p = 0; p < i; ++p) {
					sqr_sum -= pow(L[i][p], 2);
				}
				L[i][i] = sqrt(sqr_sum);
			}

			for (int i = 1; i < N - 1; ++i)
			{
				for (int j = i + 1; j < N; ++j)
				{
					sqr_sum = A[j][i];
					for (int p = 0; p < i; ++p) {
						sqr_sum -= L[i][p] * L[j][p];
					}

					L[j][i] = 1. / L[i][i] * sqr_sum;
				}
			}

			const variable* cur_v = vars.begin();
			for (int i{ 0 }; i < vars_count; ++i, cur_v++)
			{
				vars[i]->name = cur_v->name;
				vars[i]->value = cur_v->value;
			}




			// Экстраполяционный метод Адамса 4-го порядка
			double* qk = (double*)malloc(sizeof(double) * (arguments_count));
			for (int i = 0; i < arguments_count; ++i) qk[i] = (double*)malloc(sizeof(double) * (arguments_count - i));

			for (int row = 0; row < N_shift * 2; ++row)
			{
				qk[0][row] = table_values[5][row] = table_values[1][row];

				for (int column = 1; column < row + 1; ++column)
					qk[column][row - column] = qk[column - 1][row - column] - qk[column - 1][row - column + 1];
			}

			for (int row = N_shift * 2; row < arguments_count; ++row)
			{
				yk = table_values[5][row - 1];

				for (int column = 1; column < row + 1; ++column)
					qk[column][row - column] = qk[column - 1][row - column] - qk[column - 1][row - column + 1];

				// qm ∆qm-1 ∆2qm-2 ∆3qm-3 ∆4qm-4

				table_values[5][row] = yk;
				for (int col = 0; col < N_shift; ++col)
				{

				}

			}

			for (int i = 0; i < arguments_count; ++i) free(qk[i]);
			free(qk);


#ifndef	_PYTHON_GUI_H_
#define	_PYTHON_GUI_H_

void create_window();

#endif // _PYTHON_GUI_H_



#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

#include "python_gui.h"

void create_window() 
{

	//PyObject* pInt;

	//Py_Initialize();

	//PyRun_SimpleString("print('Hello World from Embedded Python!!!')");

	//Py_Finalize();

	//printf("\nPress any key to exit...\n");
	//if (!_getch()) _getch();
	//return 0;


	PyObject* pModule, * pFunc, * pArgs, * pValue; // pointers to Python objects
	double x, y;
	x = 2.0; y = 3.0; // values to be added
	Py_Initialize(); // initialize Python

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	PyRun_SimpleString("sys.path.append(os.getcwd())");

	// import module
	pModule = PyImport_Import(PyUnicode_FromString("Py_graphlib"));
	PyErr_Print();
	if (!pModule) printf_s("Failed to load <Py_graphlib.py>");
	// get reference
	pFunc = PyObject_GetAttrString(pModule, "create_graph");
	pArgs = PyTuple_New(2); // define tuple of 2 arguments
	PyTuple_SetItem(pArgs, 0, PyFloat_FromDouble(x)); // set 1st argument
	PyTuple_SetItem(pArgs, 1, PyFloat_FromDouble(y)); // set 2nd argument
	pValue = PyObject_CallObject(pFunc, pArgs); // call Python function
	printf_s("x + y = %lf", PyFloat_AsDouble(pValue));

	Py_DECREF(pArgs); // clean up
	Py_DECREF(pFunc);
	Py_DECREF(pModule);
	Py_Finalize(); // finish Python
}


import numpy as np
import matplotlib.pyplot as plt


def create_graph(x, y):
	x = np.random.randint(low=1, high=11, size=50)
	y = x + np.random.randint(1, 5, size=x.size)
	data = np.column_stack((x, y))

	fig, (ax1, ax2) = plt.subplots(
		nrows=1, ncols=2,
		figsize=(8, 4)
	)

	ax1.scatter(x=x, y=y, marker='o', c='r', edgecolor='b')
	ax1.set_title('Scatter: $x$ versus $y$')
	ax1.set_xlabel('$x$')
	ax1.set_ylabel('$y$')

	ax2.hist(
		data, bins=np.arange(data.min(), data.max()),
		label=('x', 'y')
	)

	ax2.legend(loc=(0.65, 0.8))
	ax2.set_title('Frequencies of $x$ and $y$')
	ax2.yaxis.tick_right()

	plt.show()




			printf_s("\n\nFounded %d arguments by secant method\n", solutions_count);
			double* xss = non_linear_equation(&solutions_count, A, B, secant_divisions_count, secant_error, secant, variables, 0, 1, wnx_rpn);
			if (solutions_count > arguments_count) printf_s("[ERROR]");
			if (solutions_count <= arguments_count)
			{
				for (int i = 0; i < solutions_count; ++i)
					printf_s("\t%+.64lf\n", xss[i]);
			}
			else
			{
				for (int i = 0; i < arguments_count; ++i)
					printf_s("\t%+.64lf\n", xss[i]);
			}

	double get_legender_orthogonal_polinom_value(double x, int n)
	{
		if (n >= 2) return (2 * n - 1) / n * get_legender_orthogonal_polinom_value(x, n - 1) - (n - 1) / n * get_legender_orthogonal_polinom_value(x, n - 2);
		else if (n == 1) return x;
		else return 1;
	}

		if (ImGui::Button("Вычислить моменты весовой функции с заданным числом делений\n(зависит от весовой функции, промежутка и числа делений)"))
		{
			//moments[i] = get_integration_compound_middle_rectange(f, variables, 0, 1, fixed_A, fixed_B, divisions_count);
			// Вычисление моментов по СКФ средних прямоугольников, оптимизированное
			double h = (B - A) / (double)divisions_count;

			for (int i = 0; i < arguments_count * 2; ++i)
				moments[i] = 0;

			double weight;
			for (int i = 0; i < divisions_count; ++i)
			{
				variables[0]->value = A + h * ((double)i + 0.5);
				weight = get_RPN_result(weight_function_rpn, variables, 1);

				for (int cur_moment = 0; cur_moment < arguments_count * 2; ++cur_moment)
					moments[cur_moment] += weight * pow(variables[0]->value, cur_moment);
			}

			printf_s("\n\n-----| Moments calculated |-----\n");
			for (int i = 0; i < arguments_count * 2; ++i)
			{
				moments[i] *= h;
				printf_s("\t%d]\t%.64lf\n", i, moments[i]);
			}
			printf_s("2n-1 = %d\n", arguments_count * 2 - 1);


		}



			//double* xss = non_linear_equation(&solutions_count, A, B, secant_divisions_count, secant_error, secant, variables, 0, 1, wnx_rpn);
			//printf_s("\nFounded %d arguments by secant method\n", solutions_count);
			//if (solutions_count > arguments_count) printf_s("[ERROR]");
			//if (solutions_count <= arguments_count)
			//{
			//	for (int i = 0; i < solutions_count; ++i)
			//		printf_s("\t%+.64lf\n", xss[i]);
			//}
			//else
			//{
			//	for (int i = 0; i < arguments_count; ++i)
			//		printf_s("\t%+.64lf\n", xss[i]);
			//}


//"ln(cos(x)+x*(2+pow(x,-x)))",  
			//"(1-x+2*pow(x,x)-x*ln(x)-pow(x,x)*sin(x))/(x+2*pow(x,1+x)+pow(x,x)*cos(x))"


char* function2_view();
double calculate_function2_result(double x);
double calculate_f2_deriative_result(double x);
double calculate_f2_deriative2_result(double x);


void nonlinear_equation_dbg();



#include "computational_mathematics.h"
#include <stdio.h>
#include <math.h>
#include "Time_Debug.h"
#include "dbg_computational_mathematics.h"

char* function2_view() { return "(sin(x) + cos^2(x)) / (x^2 + x)"; }

double calculate_function2_result(double x)
{
	return (sin(x) + pow(cos(x), 2)) / (pow(x, 2) + x);
}

double calculate_f2_deriative_result(double x)
{
	// ((cos x-2sin(x)cos x)(x^2+x)-(2x+1)(sin x+cos^2x))/(x^2+x)^2
	double s = sin(x), c = cos(x), te = pow(x, 2) + x;
	return ((c - 2 * s * c) * te - (2 * x + 1) * (s + pow(c, 2))) / pow(te, 2);
}

double calculate_f2_deriative2_result(double x)
{
	// Сделано при помощи калькулятора и текстовых замен
	//
	//((2*x^4+4*x^3+2*x^2)*sin(x)^2+((8*x^3+12*x^2+4*x)*cos(x)-x^4-2*x^3+5*x^2+6*x+2)*sin(x)+((-2*x^4)-4*x^3+4*x^2+6*x+2)*cos(x)^2+((-4*x^3)-6*x^2-2*x)*cos(x))/(x^6+3*x^5+3*x^4+x^3)
	return ((2 * pow(x, 4) + 4 * pow(x, 3) + 2 * pow(x, 2)) * pow(sin(x), 2) + ((8 * pow(x, 3) + 12 * pow(x, 2) + 4 * x) * cos(x) - pow(x, 4) - 2 * pow(x, 3) + 5 * pow(x, 2) + 6 * x + 2) * sin(x) +
		((-2 * pow(x, 4)) - 4 * pow(x, 3) + 4 * pow(x, 2) + 6 * x + 2) * pow(cos(x), 2) + ((-4 * pow(x, 3)) - 6 * pow(x, 2) - 2 * x) * cos(x)) / (pow(x, 6) + 3 * pow(x, 5) + 3 * pow(x, 4) + pow(x, 3));
}

void nonlinear_equation_dbg()
{
	const int text_count = 4;
	const char* Text[] = {
		"Метод секущих",
		"Метод бисекций",
		"Метод Ньютона",
		"Модифицированный метод Ньютона"
	};

	printf_s("Функция f(x) = ");
	printf_s(function2_view());
	printf_s("\n");

	double A, B, eps;
	int segments_count;

	printf_s("Вводите числа в виде 1 или 0,1\n");
	do
	{
		printf_s("Введите левую и правую границу отрезка поиска решения: ");
		scanf_s("%lf%lf", &A, &B);
	} while (B - A <= 0);

	do
	{
		printf_s("Введите количество разделений отрезка: ");
		scanf_s("%d", &segments_count);
	} while (segments_count <= 0);

	eps = input_epsilon();

	printf_s("Выберите метод поиска решения: ");
	int choice = choice_menu_h(Text, text_count);
	finding_roots_method frm = (finding_roots_method)choice;

	int solutions_count = 0;
	startTimer();
	double* solutions = non_linear_equation(&solutions_count, A, B, segments_count, eps, frm, calculate_function2_result, calculate_f2_deriative_result, calculate_f2_deriative2_result);
	stopTimer();

	printf_s("\nВремя поиска решений - %lf\n", getTimerClock());
	printf_s("\nОтвет:\nКоличество решений - %d\n", solutions_count);
	print_solutions(solutions, solutions_count);
}





			values_array = (double**)realloc(values_array, sizeof(double*) * function_count);

			for (int i = 0; i < fixed_function_count; ++i)
				values_array[i] = (double*)realloc(values_array[i], sizeof(double) * new_pairs_count);
			for (int i = fixed_function_count; i < function_count; ++i)
				values_array[i] = (double*)malloc(sizeof(double) * new_pairs_count);

			arguments_array = (double*)realloc(arguments_array, sizeof(double) * new_pairs_count);



		if (ImGui::Button("Получить информацию из файла calculation_functions.txt"))
		{
			printf_s("Выберите тип записи графа в файле для чтения: ");

			printf_s("Выберите файл с графом: ");
			char* path = file_choice_menu_stdir();

			FILE* f;
			fopen_s(&f, path, "r");
			if (f)
			{
				fscanf_s(f, "%d", &fixed);
				fscanf_s(f, "%d", &vertex_count);

				char cur_char = ' ';
				fgetc(f);
				for (int i = 0; i < vertex_count; ++i)
				{
					fscanf_s(f, "%d -", &vertex_arc);
					cur_char = fgetc(f);
					while (cur_char != '\n' && cur_char != '\0' && cur_char != -1)
					{
						fscanf_s(f, "%d", &vertex_arc);
						add_arc(g, i, vertex_arc);
						cur_char = fgetc(f);
					}
				}
				fclose(f);
			}
			else
			{
				perror("Error");
			}
		}



		//// Переменные
		//static int variables_count = 1; ImGui::InputInt("input int", &variables_count);

		//static char variable_name[10] = "";
		//static float value = 0.0f;
		//if (ImGui::CollapsingHeader("Variables", ImGuiTreeNodeFlags_None))
		//{
		//	ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
		//	for (int i = 0; i < variables_count; i++)
		//	{
		//		// Ввод переменных
		//		ImGui::InputText("default", variable_name, 10); ImGui::SameLine(0, 20);
		//		ImGui::DragFloat("Value", &value);
		//	}
		//}


	//const int c = 100;
	//for (int n = 1; n < c; ++n)
	//{
	//	printf_s("\n\n---||---\n");
	//	printf_s("\nn = %d\n", n);
	//	//res = calculate_all_combinations(n);
	//	res = calculate_all_combinations_binary(n);
	//	printf_s("  ");
	//	//calculate_all_combinations_by_formula(n);
	//	printf_s("\n");
	//	printf_s("Результат: %d\n\n", res);

	//	calculate_by_burnside(n);
	//}

//int calculate_all_combinations_by_formula(int n)
//{
//	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
//	double inner_result;
//	inner_result = (pow(2, n) + 2 * pow(2, n)) / n;
//
//	printf_s("[");
//	set_console_color_(h, LightMagenta, Black);
//	printf_s("%.1lf", inner_result);
//	set_console_color_(h, White, Black);
//	printf_s("]");
//	return inner_result;
//}


// f(xi, ..., xi+k)
// 1 столбец f(x_0) ... f(x_degree)
		//printf_s("[0, %d] | f(x%d) = %lf\n", i, i, fk[0][i]);

			//printf_s("[%d, %d] | f(x%d,...,x%d) | [%d, %d] - [%d, %d] | x%d - x%d\n", k, i, i, i + k, k - 1, i+1, k - 1, i, i, i + k);

	PyGraph(0, NULL);

	//GraphInit(500, 500);
	MainLoop();

	PyGraph_Finish();

PyObject *pModule;

void Python(int argc, wchar_t **argv) // constructor: initialize Python
{
   Py_Initialize();
   PySys_SetArgv(argc, argv);
}

void Python_Finish() { Py_Finalize(); }

void PyGraph(int argc, wchar_t **argv) // constructor: import graphlib.py
{
   Python(argc, argv);
   PyRun_SimpleString("import sys");
   PyRun_SimpleString("import os");
   PyRun_SimpleString("sys.path.append(os.getcwd())");
   pModule = PyImport_Import(PyUnicode_FromString("graphlib"));
   if (!pModule)
	  printf_s("Failed to load <graphlib.py>");
}

void PyGraph_Finish() // destructor: release graphlib.py
{
   Py_DECREF(pModule);
   Python_Finish();
}



	//show_bracket_sequence();
	//show_max_value_segment();
	//show_repeated_int();
	//show_complex();
	//show_squares_intersection_area();

//// Если достаточное условие сходимости метода Ньютона не выполнено
//if (fdr == 0 || fr * function_deriative2(x) <= 0)
//{
//	x = ls[i]->A + l / 101 * (rand() % 101); // один из концов 101 деления
//	fdr = function_deriative(x);
//	fr = function(x);
//	printf_s("\nНе выполнено условие сходимости, x изменён\n");
//}


Отладка фибоначчиевых деревьев

		printf_s("\n--{CONSOLIDATING}--\n");
		print_fibonacci_heap(fh);
		printf_s("\n");

		printf_s("trees by degrees\n");
		for (int i = 0; i < max_degree; ++i)
			if (trees_by_degrees[i])
			{
				printf_s("[%d %.2f - %d]\n", i, trees_by_degrees[i]->key, *((int*)trees_by_degrees[i]->data));
				if (trees_by_degrees[i]->child)
					print_fibonacci_heap_root(trees_by_degrees[i]->child, 1);
			}
		printf_s("---\n");

		print_fibonacci_heap(fh);
		printf_s("\n");
		printf_s("\n--{CONSOLIDATING_END}--\n\n");

printf_s("\n");
	print_fibonacci_heap_root(nf1, 0);
	printf_s("\n");

	printf_s("\n\n");
	printf_s("Дерево:\n");
	print_fibonacci_heap(fh);
	printf_s("\n");

	printf_s("\n");
	printf_s("Дерево после уменьшения ключа:\n");
	print_fibonacci_heap(fh);
	printf_s("\n");

	printf_s("\n");
	printf_s("Дерево после вытаскивания:\n");
	print_fibonacci_heap(fh);
	printf_s("\n");

	graph_m_print(gm);
	printf_s("\n");
	graph_m_print(gm_t);
	printf_s("\n");

	printf_s("in_sum\n");
	print_floats(in_sums, count);
	printf_s("\n");
	printf_s("out_sum\n");
	print_floats(out_sums, count);
	printf_s("\n");


	printf_s("\nnodes:\n");
	for (int i = 0; i < count; ++i)
	{
		printf_s("[%d - node - %.3f]\n", i, nodes[i]->key);
	}
	printf_s("\n");
	print_fibonacci_heap(fh);
	printf_s("\n\n");


	print_fibonacci_heap(fh);
	printf_s("\n");

	printf_s("degree key data\n");

			printf_s("merging\n");
			printf_s("[%d %.2f - %d]\n", cur_n->degree, cur_n->key, *((int*)cur_n->data));
			print_fibonacci_heap_root(cur_n->child, 1);
			printf_s("\n");


	printf_s("Итоговое дерево после изъятия минимума:\n");
	print_fibonacci_heap(fh);


*/


//CONSOLE_SCREEN_BUFFER_INFO bi;
//GetConsoleScreenBufferInfo(h, &bi);
//COORD point = bi.dwCursorPosition;
// 
//modPow_recursion_example();
	//euclid_recursion_example();
	//extended_euclid_example();
	//mul_inverse_example();
	//linear_diophantine_example();
	//chinese_reminder_example(iterative);
	/*legendre_symbol_example(legendre_symbol);
	legendre_symbol_example(legendre_symbol_2);*/

	//modPow_recursion_input();
	//euclid_recursion_input();
	//extended_euclid_input();
	//linear_diophantine_input();
	//chinese_reminder_input(iterative);
	//legendre_symbol_input(legendre_symbol);
	//legendre_symbol_input(legendre_symbol_2);

	//int iterations = 10e7;
	//double t1 = legendre_time(iterations, legendre_symbol);
	////legendre_time(iterations, legendre_symbol_2);
	//double t2 = legendre_time(iterations, legendre_symbol_3);
	//printf("Разница во времени выполнения: %f\n\n", (float)(t1 - t2));

//graph* new_graph = graph_init(g->count);
//
//node* cur_node;
//for (int i = 0; i < g->count; ++i)
//{
//	cur_node = g->adj_list[i].head;
//	while (cur_node)
//	{
//		add_arc(new_graph, i, cur_node->vertex);
//
//		cur_node = cur_node->next;
//	}
//}
//return new_graph;
//#include "MathPMI_Debug.h"
//#include <math.h>
//#include <gtk/gtk.h>
//#include <time.h>
//
//#define WIDTH   1500
//#define HEIGHT  800
//
//#define ZOOM_X  100.0
//#define ZOOM_Y  100.0
//
//void print_data()
//{
//	g_print("\n");
//}
//
//cairo_surface_t* surface = NULL;
//
//void clear_surface(void)
//{
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//
//	cairo_set_source_rgb(cr, 1, 1, 1);
//	cairo_paint(cr);
//
//	cairo_destroy(cr);
//}
//
//void configure_event_cb(GtkWidget* widget)
//{
//	if (surface)
//		cairo_surface_destroy(surface);
//
//	surface = gdk_surface_create_similar_surface(surface,
//		CAIRO_CONTENT_COLOR,
//		gtk_widget_get_allocated_width(widget),
//		gtk_widget_get_allocated_height(widget));
//
//	clear_surface();
//}
//
//void draw_cb(cairo_t* cr)
//{
//	cairo_set_source_surface(cr, surface, 0, 0);
//	cairo_paint(cr);
//}
//
//void draw_graph()
//{
//	int x;
//	int y = HEIGHT / 2;
//	int i, j;
//
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//	cairo_set_line_width(cr, 1);
//
//	srand(time(NULL));
//
//	for (x = 0; x < 1500; x++)
//	{
//		i = 10 % 6 % 3;
//		j = 10 % 6 % 3;
//		cairo_move_to(cr, x, y);
//
//		y += i - j;
//
//		cairo_line_to(cr, x + 1, y);
//	}
//
//	cairo_stroke(cr);
//
//	cairo_destroy(cr);
//}
//
//void draw_lines()
//{
//	clear_surface();
//
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//
//	/* Рисуем верхнюю линию */
//	cairo_set_source_rgba(cr, 0, 1, 0, 1);
//	cairo_move_to(cr, 0, (HEIGHT / 2) / 100 * 20);
//	cairo_line_to(cr, WIDTH, (HEIGHT / 2) / 100 * 20);
//	cairo_stroke(cr);
//
//	/* Рисуем нижнюю линию */
//	cairo_set_source_rgba(cr, 1, 0, 0, 1);
//	cairo_move_to(cr, 0, HEIGHT / 100 * 90);
//	cairo_line_to(cr, WIDTH, HEIGHT / 100 * 90);
//	cairo_stroke(cr);
//}
//
//int x = 0, y = HEIGHT / 2;
//int i, j;
//
//gboolean draw_graph2(gpointer widget)
//{
//	cairo_t* cr;
//
//	cr = cairo_create(surface);
//	cairo_set_line_width(cr, 1);
//
//	srand(time(NULL));
//
//	if (x < 1500)
//	{
//		i = rand() % 6 % 3;
//		j = rand() % 6 % 3;
//
//		cairo_move_to(cr, x, y);
//
//		y += i - j;
//
//		cairo_line_to(cr, x + 1, y);
//
//		x++;
//	}
//
//	cairo_stroke(cr);
//
//	gtk_widget_queue_draw(widget);
//
//	return TRUE;
//}
//
//void activate(GtkApplication* app, gpointer user_data)
//{
//	GtkWidget* window;
//	GtkButton* button;
//	GtkWidget** area;
//
//	window = gtk_application_window_new(app);
//	gtk_window_set_title(GTK_WINDOW(window), "Complex numbers");
//	gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
//	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
//
//	GtkBox* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
//	gtk_window_set_child(GTK_WINDOW(window), box);
//
//	area = gtk_drawing_area_new();
//	gtk_widget_set_size_request(area, WIDTH, HEIGHT);
//	gtk_box_append(box, area);
//
//	button = gtk_button_new_with_label("START");
//	gtk_widget_set_size_request(button, 200, 200);
//	gtk_box_append(box, button);
//
//	g_signal_connect(area, "draw", G_CALLBACK(draw_cb), NULL);
//	g_signal_connect(area, "configure-event", G_CALLBACK(configure_event_cb), NULL);
//	g_signal_connect(area, "draw", G_CALLBACK(draw_lines), NULL);
//	g_signal_connect(button, "button-press-event", G_CALLBACK(draw_graph), NULL);
//	gtk_widget_show(window);
//	gtk_window_present(GTK_WINDOW(window));
//
//	g_timeout_add(100, (GSourceFunc)draw_graph2, area);
//
//
//}
//
//void complex_numbers_dbg()
//{
//	GtkApplication* app;
//	int status;
//
//	app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
//	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
//	status = g_application_run(G_APPLICATION(app), 0, NULL);
//	g_object_unref(app);
//}
//*/
//
///*
//char* poly_tostring(const poly* p)
//{
//	char* current, *result = "", str_decimal,* buffer = (char*)malloc(16 * sizeof (char));
//	for (int i = 0; i < 15; ++i, buffer[i] = " ")
//	buffer[15] = '\0';
//	poly* cur_poly = p;
//
//	while (cur_poly)
//	{
//		current = NULL;
//		if (cur_poly->coeff)
//			if (cur_poly->exp == 0)
//				current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
//			else if (cur_poly->exp == 1)
//				if (cur_poly->coeff == 1)
//					current = "x";
//				else
//				{
//					current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
//					strcat_s(current, (strlen(current) + 2) * sizeof(char), "x");
//				}
//			else
//			{
//				current = _ltoa_s(cur_poly->coeff, buffer, strlen(buffer), 10);
//				strcat_s(current, (strlen(current) + 2) * sizeof(char), "x^");
//				str_decimal = _ltoa_s(cur_poly->exp, buffer, strlen(buffer), 10);
//				strcat_s(current, (strlen(current) + strlen(str_decimal) + 2) * sizeof(char), str_decimal);
//			}
//		else
//			current = "0";
//
//		if (cur_poly->coeff)
//			if (cur_poly->coeff > 0)
//				strcat_s(result, (strlen(result) + 1) * sizeof(char), "+");
//			strcat_s(result, (strlen(result) + strlen(current) + 1) * sizeof(char), current);
//
//		cur_poly = cur_poly->next;
//	}
//
//	if (result == "")
//		result = "0";
//	return result;
//
//}
//*/
///*
//void bigint_subtract_bigint(big_int* b1, const big_int* b2) // big_int 1 и 2 (из b1 вычитается b2)
//{
//	int temp = 0; // временная
//	unsigned int rank = 0; // рабочий разряд
//	int i = 0, j = 0; // итераторы
//
//	bigint_realloc_to_newlength(b1, b2->length); // Расширяем big_int1 до вычитаемого big_int2
//	while (rank < b2->length) // Проходим все разряды big_int2
//	{
//		temp = (int)b1->number[rank] - (int)b2->number[rank]; // Записываем разницу между разрядом rank в big_int1 и big_int2 
//		if (temp >= 0) // разница >= 0 
//		{
//			b1->number[rank] = temp; // записываем разницу
//		}
//		else
//		{
//			for (i = rank + 1; i < b1->length; ++i) // Ищем верхний разряд в big_int1 не равный 0, начиная с rank + 1
//			{
//				if (b1->number[i] > 0)
//				{
//					b1->number[i]--; // Вычитаем из него 1
//					for (j = rank + 1; j < i; ++j)
//					{
//						b1->number[j] = 0xFF; // оборачиваем пройденные нулевые разряды
//					}
//					break; // Нашли разряд
//				}
//			}
//
//			if (i < b1->length) // Если нашёлся такой разряд
//			{
//				b1->number[rank] = 256 + temp; // Записываем в рабочий разряд 256 из большего разряда вычесть разницу в этом разряде
//			}
//			else
//			{
//				b1->isNegative = !b1->isNegative; // Делаем число отрицательным
//				b1->number[rank] = -temp; // Присваиваем разницу в рабочий разряд
//
//				// Далее нужно обернуть все предыдущие разряды, так как они хранят положительное число
//
//				temp = rank; // сохраняем номер разряда, из которого будет вычитаться единица,
//				// так как нужно отнять все оставшиеся положительные разряды
//				for (i = rank - 1; i >= 0; --i) // Ищем нижний разряд в big_int1 не равный 0, начиная с rank - 1
//				{
//					if (b1->number[i] > 0)
//					{
//						b1->number[temp] -= 1; // Забираем из того разряда единицу
//						for (j = rank - 1; j > i; --j)
//						{
//							b1->number[j] = 0xFF; // оборачиваем пройденные нулевые разряды
//						}
//						b1->number[i] = 256 - b1->number[i]; // оборачиваем этот разряд
//						temp = i;
//					}
//				}
//
//				// Все верхние разряды переносим из вычитаемого
//				for (i = rank + 1; i < b2->length; ++i)
//				{
//					b1->number = b2->number[i];
//				}
//
//				// Если последний разряд оказался равным нулю, уменьшаем big_int
//				if (b1->number[b1->length - 1] == 0)
//				{
//					b1->number = (unsigned char*)realloc(b1->number, sizeof(unsigned char) * (b1->length - 1));
//					--b1->length;
//				}
//
//				return; // Получилось отрицательное число
//			}
//		}
//		rank++;
//	}
//
//	// Убираем возможные лишние нули, которые могли получиться в результате вычитания
//	temp = b1->length - 1;
//	while (temp > 0)
//	{
//		if (b1->number[temp])
//		{
//			break;
//		}
//		temp--;
//	}
//	b1->number = (unsigned char*)realloc(b1->number, sizeof(unsigned char) * (++temp));
//	b1->length = temp;
//}*/
//
///*
//unsigned char* decimal_to_256chars(unsigned int decimal, unsigned int* chars_l)
//{
//	unsigned char* chars = (unsigned char*)malloc(sizeof(unsigned char*)); // результат
//	int rank = 0;
//	while (decimal != 0)
//	{
//		chars = (unsigned char*)realloc(chars, sizeof(unsigned char) * (rank + 1));
//		chars[rank] = decimal & 255;
//		decimal >>= 8;
//		rank += 1;
//	}
//	*chars_l = rank;
//	return chars;
//}*/
//// //Плохой алгоритм 2^N		20-2 дня,	40-35 лет
////void big_int_add_one_decimal(big_int* b, unsigned int c, unsigned int k, unsigned int r) // bigint, число, степень, куда его.
////{
////	if (c == 0) 
////	{
////		return;
////	}
////	while (k > 0)
////	{
////		c = (c + (c << 2)) << 1; // добавляем разряд к десятичному числу
////		k -= 1;
////		if (c > 255) 
////		{
////			big_int_add_one_decimal(b, c & 255, k, r);
////			big_int_add_one_decimal(b, c >> 8, k, r + 1);
////			return;
////		}
////	}
////	big_int_add_one_char(b, (unsigned char)c, r);
////}
////
////void bigint_add_chars(big_int *b, const unsigned char* chars, const unsigned int chars_l)
////{
////	unsigned int t = 0; // сумма разряда
////	unsigned int rank = 0; // текущий разряд
////	bigint_realloc(b, chars_l);
////	do
////	{
////		if (rank < chars_l) 
////		{
////			t += (unsigned int)chars[rank];
////		}
////		if (rank < b->length)
////		{
////			t += (unsigned int)b->number[rank];
////		}
////
////		b->number[rank] = t & 255;
////		t = (unsigned int)(t >> 8);
////
////		rank += 1; // переходим к следующему разряду
////	} while (rank < chars_l);
////
////	if (t != 0) // Если что-то осталось
////	{
////		bigint_realloc(b, chars_l + 1);
////		b->number[rank] = t;
////	}
////}
//
//
////void bigint_substract_one_char(big_int* big_t, unsigned char ch, unsigned int rank) // big_int, число ch, которое нужно вычесть из разряда rank
////{
////	// 10000001 - 10000110|2
////	// 
////	// 10000010 10000001 11101110
////	// 10000010 00000000 11101110
////	// 10000001 (n - c = t; 256+t) 11101110
////	// 
////	// 10000001 11101110
////	// 00000000 11101110
////	// (-t)		00010001
////	// 
////	// 86 - 102|2
////	// 86 102
////	// 
////	// -16 102
////	// -15 154
////	// 
////	// 23 - 30
////	// - 0 7
////	if (rank > big_t->length - 1) 
////	{
////		big_t->isNegative = !big_t->isNegative;
////		bigint_realloc(big_t, rank+1);
////		big_t->number[rank] = (int)ch;
////
////		// 00000000 00000000 10000000 10000000 -
////		// 11000000 00000000 00000000 00000000
////		// 1)
////		// 
////		//-10111111 11111111 01111111 00000000
////		//-10111111 11111111 01111110 01111111 Ответ
////		// 2)
////		// 
////
////		bigint_minus_one_in_rank(big_t, rank);
////	}
////	else
////	{
////
////		int t = (int)big_t->number[rank] - (int)ch; // разница между разрядом r в big_t и вычитаемым разрядом ch
////		if (t < 0)
////		{
////			if (rank == big_t->length - 1)  // Если длина разрядов big_int и rank одинаковы
////			{
////				big_t->isNegative = !big_t->isNegative;
////				big_t->number[rank] = -t; // Присваиваем разницу
////
////				bigint_minus_one_in_rank(big_t, rank); // Меняем следующие разряды
////			}
////			else
////			{
////				big_t->number[rank] = 256 + t; // 1 (256) из большего разряда - разница
////				for (int i = rank + 1; i < big_t->length; ++i) // Проходим до верхнего разряда не равного нулю
////				{
////					if (big_t->number[i] > 0)
////					{
////						big_t->number[i] -= 1; // Вычитаем из него 1
////						break;
////					}
////					else
////					{
////						big_t->number[i] = 255; // оборачиваем другие разряды
////					}
////				}
////			}
////		}
////		else
////		{
////			big_t->number[rank] = t; // записываем разницу
////		}
////	}
////	bigint_remove_zeros(big_t);
////}