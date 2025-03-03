#ifndef	__NODE_MENU_H__
#define	__NODE_MENU_H__
#pragma once

#include <stdio.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_stdlib.h>

#include <imgui_node_editor.h>
#include <imgui_internal.h>

#include <solvers.h>

#include "widgets.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

namespace ed = ax::NodeEditor;

enum class PinDataType
{
    Flow,
    Bool,
    Int,
    Double,
    String,
    Object,
    Function,
    Delegate,

    Real_1d_array,
    Real_2d_array
};

enum class PinIOKind
{
    Output,
    Input
};

enum class NodeType
{
    Blueprint,
    Simple,
    Comment
};

struct Node;
struct Link;

struct Pin
{
    Node* Node = nullptr;
    std::vector<Link*> links;

    ed::PinId   ID;
    std::string Name;
    PinDataType   DataType;
    PinIOKind     IOKind;

    bool isOutsideData;
    void* Data;

    Pin(int id, const char* name, PinDataType type, PinIOKind kind, bool outside = false, void* data = NULL) :
        ID(id), Name(name), DataType(type), IOKind(kind), isOutsideData(outside), Data(data)
    {
    }
};

struct Link
{
    ed::LinkId ID;

    Pin* StartPin;
    Pin* EndPin;

    ImColor Color;

    Link(ed::LinkId id, Pin* startPin, Pin* endPin) :
        ID(id), StartPin(startPin), EndPin(endPin), Color(255, 255, 255)
    {
        EndPin->links.push_back(this);
        StartPin->links.push_back(this);
        EndPin->Data = StartPin->Data;
    }
};

struct Node
{
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;

    std::string State;
    std::string SavedState;

    bool HasInputs()
    {
        for (auto input : Inputs)
            if (input.links.size() != 0) return true;
        return false;
    }

    using ProcessOperation = void (*)(const std::vector<Pin>& Inputs, std::vector<Pin>& Outputs);
    ProcessOperation Process = NULL;

    void ProcessData()
    {
        if (!Outputs.size()) return;

        // Получаем данные
        for (auto input : Inputs)
            if (input.links.size() != 0)
                input.Data = input.links[0]->StartPin->Data;
        
        // Обрабатываем
        if (Process)
            Process(Inputs, Outputs);

        // Отправляем данные
        for (auto output : Outputs)
            for (auto link : output.links)
                link->EndPin->Data = output.Data;
    }


    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255), ProcessOperation process = NULL) :
        ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0), Process(process)
    {
    }
};


struct NodeIdLess
{
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

static ed::EditorContext* m_Editor = nullptr;

struct NodeMenu
{
    NodeMenu()
    {
        OnStart();
    }
    ~NodeMenu()
    {
        OnStop();
    }

    void OnStart();
    void OnFrame(float deltaTime);
    void OnStop();

    Node* FindNode(ed::NodeId id);
    Link* FindLink(ed::LinkId id);
    Pin* FindPin(ed::PinId id);

    bool IsPinLinked(ed::PinId id);


private:
    std::vector<Node>    m_Nodes;
    std::vector<Link>    m_Links;

    int GetNextId();
    ed::LinkId GetNextLinkId();

    void TouchNode(ed::NodeId id);
    float GetTouchProgress(ed::NodeId id);
    void UpdateTouch();

    bool CanCreateLink(Pin* a, Pin* b);

    void BuildNode(Node* node);
    void BuildNodes();

    void ShowStyleEditor(bool* show = nullptr);
    void ShowLeftPane(float paneWidth);

    int                  m_NextId = 1;
    const int            m_PinIconSize = 24;
    ImTextureID          m_HeaderBackground = nullptr;
    ImTextureID          m_SaveIcon = nullptr;
    ImTextureID          m_RestoreIcon = nullptr;
    const float          m_TouchTime = 1.0f;
    std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool                 m_ShowOrdinals = false;

    void AddToTopsorted(std::vector<Node*>& topsort_nodes) // Расширение входящих нодов до всех отсортированных по топологии
    {
        Node* linked_node;
        ed::NodeId nodeId = 0;
        for (int i = 0; i < topsort_nodes.size(); ++i)
        {
            for (auto output : topsort_nodes.at(i)->Outputs) // Не лучший код
            {
                for (Link* link : output.links)
                {
                    linked_node = link->EndPin->Node;
                    nodeId = linked_node->ID;

                    auto id = std::find_if(topsort_nodes.begin(), topsort_nodes.end(), [nodeId](Node* node) { return node->ID == nodeId; });
                    if (id != topsort_nodes.end()) // Нод повторился, значит по топологии он находится дальше
                    {
                        Node* cur_node = topsort_nodes.at(i);
                        topsort_nodes.erase(id);
                        if (i < topsort_nodes.size() && cur_node != topsort_nodes.at(i)) 
                            --i;
                    }
                    topsort_nodes.push_back(linked_node);
                }
            }
        }
    }

    void ReprocessAllNodesLinked(std::vector<Node*> topsort_nodes) // Когда требуется обработать часть системы
    {
        AddToTopsorted(topsort_nodes);

        for (Node* node : topsort_nodes)
            node->ProcessData();
    }

    void ReprocessAllNodesLinked(Node* input_node) // Когда требуется обработать только часть системы, которая идёт от этого нода
    {
        if (!input_node)
            return;
        std::vector<Node*> topsort_nodes;
        topsort_nodes.push_back(input_node);
        ReprocessAllNodesLinked(topsort_nodes);
    }

    void ReprocessAllNodesLinked() // Когда требуется найти входящие ноды
    {
        std::vector<Node*> input_nodes;
        for (Node& node : m_Nodes)
            if (!node.HasInputs())
                input_nodes.push_back(&node);

        ReprocessAllNodesLinked(input_nodes);
    }

    void FindAndErase(std::vector<Link*>& Links,const ax::NodeEditor::LinkId& ID)
    {
        if (Links.size() == 0)
            return;
        auto id = std::find_if(Links.begin(), Links.end(), [ID](Link* link) { return link->ID == ID; });
        Links.erase(id);
    }

    void DeleteLink(std::vector<Link>::iterator id)
    {
        Link& link = *id;
        link.EndPin->Data = NULL;

        auto& ID = link.ID;
        FindAndErase(link.StartPin->links, ID);
        FindAndErase(link.EndPin->links, ID);

        m_Links.erase(id); // Меняет id!
        ReprocessAllNodesLinked(link.EndPin->Node);
    }

    void AddLink(Pin* StartPin, Pin* EndPin)
    {
        Link* link = new Link(GetNextLinkId(), StartPin, EndPin);
        m_Links.push_back(*link);

        m_Links.back().Color = GetIconColor(StartPin->DataType);
        ReprocessAllNodesLinked(EndPin->Node);
    }

    // -----SELF NODES----------------

    void InputData(Pin& output)
    {
        static void* cur_data;
        cur_data = output.Data;
        switch (output.DataType)
        {
        case PinDataType::Int: InputInt((int*)cur_data); break;
        case PinDataType::Double: InputDouble((double*)cur_data); break;
        case PinDataType::Real_1d_array: InputVector((alglib::real_1d_array*)cur_data); break;
        case PinDataType::Real_2d_array: InputMatrix((alglib::real_2d_array*)cur_data); break;
        default:
            break;
        }
    }

    void ShowData(Pin& input)
    {
        static void* cur_data;
        cur_data = input.Data;
        switch (input.DataType)
        {
        case PinDataType::Int: ShowInt((int*)cur_data); break;
        case PinDataType::Double: ShowDouble((double*)cur_data); break;
        case PinDataType::Real_1d_array: ShowVector((alglib::real_1d_array*)cur_data); break;
        case PinDataType::Real_2d_array: ShowMatrix((alglib::real_2d_array*)cur_data); break;
        default:
            break;
        }
    }

    ImColor GetIconColor(PinDataType type)
    {
        switch (type)
        {
        default:
        case PinDataType::Flow:     return ImColor(255, 255, 255);
        case PinDataType::Bool:     return ImColor(220, 48, 48);
        case PinDataType::Int:      return ImColor(68, 201, 156);
        case PinDataType::Double:    return ImColor(147, 226, 74);
        case PinDataType::String:   return ImColor(124, 21, 153);
        case PinDataType::Object:   return ImColor(51, 150, 215);
        case PinDataType::Function: return ImColor(218, 0, 183);
        case PinDataType::Delegate: return ImColor(255, 48, 48);
        case PinDataType::Real_1d_array: return ImColor(127, 255, 212);
        case PinDataType::Real_2d_array: return ImColor(120, 219, 226);
        }
    };

    void DrawPinIcon(const Pin& pin, bool connected, int alpha)
    {
        using namespace ax;
        using ax::Widgets::IconType;

        IconType iconType;
        ImColor  color = GetIconColor(pin.DataType);
        color.Value.w = alpha / 255.0f;
        switch (pin.DataType)
        {
        case PinDataType::Flow:     iconType = IconType::Flow;   break;
        case PinDataType::Bool:     iconType = IconType::Circle; break;
        case PinDataType::Int:      iconType = IconType::Circle; break;
        case PinDataType::Double:    iconType = IconType::RoundSquare; break;
        case PinDataType::String:   iconType = IconType::Circle; break;
        case PinDataType::Object:   iconType = IconType::Diamond; break;
        case PinDataType::Function: iconType = IconType::Grid; break;
        case PinDataType::Delegate: iconType = IconType::Square; break;
        case PinDataType::Real_1d_array: iconType = IconType::Circle; break;
        case PinDataType::Real_2d_array: iconType = IconType::Square; break;
        default:
            return;
        }

        ax::Widgets::Icon(ImVec2(static_cast<float>(m_PinIconSize), static_cast<float>(m_PinIconSize)), iconType, connected, color, ImColor(32, 32, 32, alpha));
    };

    void node(const char* Name, int C1, int C2, int C3, const NodeType& Type, Node::ProcessOperation Process = NULL)
    {
        m_Nodes.emplace_back(GetNextId(), Name, ImColor(C1, C2, C3), Process);
        m_Nodes.back().Type = Type;
    }

    #define output(Name, Type) m_Nodes.back().Outputs.emplace_back(GetNextId(), Name, Type, PinIOKind::Output);
    #define outputd(Name, Type, Data) m_Nodes.back().Outputs.emplace_back(GetNextId(), Name, Type, PinIOKind::Output, true, Data);

    #define input(Name, Type) m_Nodes.back().Inputs.emplace_back(GetNextId(), Name, Type, PinIOKind::Input);
    #define inputd(Name, Type) m_Nodes.back().Inputs.emplace_back(GetNextId(), Name, Type, PinIOKind::Input, true);

    Node* build()
    {
        BuildNode(&m_Nodes.back());
        return &m_Nodes.back();
    }


    Node* SpawnInputNode();
    Node* SpawnOutputNode();

    Node* SpawnMatrixMultiplicationNode();
    
    Node* SpawnGaussNode();
    Node* SpawnQRNode(); 
    Node* SpawnSimpleIterationNode();

    Node* CreationMenu()
    {
        if (ImGui::MenuItem("Input"))
            return SpawnInputNode();
        if (ImGui::MenuItem("Output"))
            return SpawnOutputNode();

        ImGui::Separator();

        if (ImGui::MenuItem("Matrix Multiplication"))
            return SpawnMatrixMultiplicationNode();

        ImGui::Separator();

        if (ImGui::MenuItem("Gauss Method"))
            return SpawnGaussNode();
        if (ImGui::MenuItem("QR Method"))
            return SpawnQRNode();
        if (ImGui::MenuItem("Simple Iterstion Method"))
            return SpawnSimpleIterationNode();

    }

    void UpdateMatrix(alglib::real_2d_array& M, const int& n, const int& m)
    {
        M.setlength(n, m);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                M[i][j] = 0;
    }
    void InputMatrix(alglib::real_2d_array* M)
    {
        static int n[2];
        n[0] = M->rows();
        n[1] = M->cols();

        ImGui::BeginVertical("##scale");

        ImGui::BeginHorizontal("##scales");
        ImGui::PushItemWidth(100);
        ImGui::DragInt("##edit1", &(n[0]), 0.25f, 1, 20);
        ImGui::Spring(0);

        ImGui::DragInt("##edit2", &(n[1]), 0.25f, 1, 20);
        ImGui::Spring(0);
        ImGui::PopItemWidth();
        ImGui::EndHorizontal();

        if (M->rows() != n[0] || M->cols() != n[1])
            UpdateMatrix(*M, n[0], n[1]);

        ImGui::PushItemWidth(150);

        for (int row = 0; row < n[0]; ++row)
        {
            for (int column = 0; column < n[1]; ++column)
            {
                char buf[64];
                sprintf_s(buf, "##obj_%d_%d", row, column);

                static float* flt = new float();
                *flt = (float)(*M)[row][column];
                ImGui::DragFloat(buf, flt, 0.001, -FLT_MAX, FLT_MAX, "%.5f", ImGuiSliderFlags_AlwaysClamp);
                (*M)[row][column] = *flt;

                if (column != n[1] - 1)
                    ImGui::SameLine();
            }
        }
        ImGui::PopItemWidth();

        ImGui::EndVertical();
    }


    void UpdateVector(alglib::real_1d_array& V, const int& n)
    {
        V.setlength(n);
        for (int i = 0; i < n; ++i)
            V[i] = 0;
    }
    void InputVector(alglib::real_1d_array* V)
    {
        static int n;
        n = V->length();

        ImGui::BeginVertical("##scale");

        ImGui::BeginHorizontal("##scales");
        ImGui::PushItemWidth(100);
        if (ImGui::DragInt("##edit", &n, 0.25f, 1, 20))
            UpdateVector(*V, n);
        ImGui::PopItemWidth();
        ImGui::EndHorizontal();

        ImGui::PushItemWidth(150);

        for (int i = 0; i < n; ++i)
        {
            char buf[64];
            sprintf_s(buf, "##obj_%d", i);

            static float* flt = new float();
            *flt = (float)(*V)[i];
            ImGui::DragFloat(buf, flt, 0.25, -FLT_MAX, FLT_MAX, "%.5f", ImGuiSliderFlags_AlwaysClamp);
            (*V)[i] = *flt;
        }
        ImGui::PopItemWidth();

        ImGui::EndVertical();
    }

    void InputInt(int* d)
    {
        ImGui::PushItemWidth(300);
        ImGui::DragInt("##int", d, 1, INT_MIN, INT_MAX, "%5d", ImGuiSliderFlags_AlwaysClamp);
    }
    void InputDouble(double* d)
    {
        ImGui::PushItemWidth(300);
        static float* flt = new float();
        *flt = (float)*d;
        ImGui::DragFloat("##flt", flt, 0.25, -FLT_MAX, FLT_MAX, "%.5f", ImGuiSliderFlags_AlwaysClamp);
        *d = *flt;
    }


    void ShowMatrix(alglib::real_2d_array* M)
    {
        if (!M)
            return;

        ImGui::BeginVertical("##scale");

        ImGui::BeginHorizontal("##scales");
        ImGui::PushItemWidth(100);
        ImGui::Text("%d rows %d columns", M->rows(), M->cols());
        ImGui::PopItemWidth();
        ImGui::EndHorizontal();

        ImGui::PushItemWidth(250);
        for (int row = 0; row < M->rows(); ++row)
        {
            for (int column = 0; column < M->cols() - 1; ++column)
            {
                ImGui::Text("%.3f", ((*M)[row][column]));
                ImGui::SameLine();
            }
            ImGui::Text("%.3f", ((*M)[row][M->cols() - 1]));
        }
        ImGui::PopItemWidth();

        ImGui::EndVertical();
    }

    void ShowVector(alglib::real_1d_array* V)
    {
        if (!V)
            return;

        ImGui::BeginVertical("##scale");

        ImGui::BeginHorizontal("##scales");
        ImGui::PushItemWidth(100);
        ImGui::Text("%d components", V->length());
        ImGui::PopItemWidth();
        ImGui::EndHorizontal();

        ImGui::PushItemWidth(300);
        for (int i = 0; i < V->length(); ++i)
            ImGui::Text("%.12f", ((*V)[i]));
        ImGui::PopItemWidth();

        ImGui::EndVertical();
    }

    void ShowDouble(double* d)
    {
        ImGui::PushItemWidth(400);
        if (!d)
        {
            ImGui::Text("0");
            return;
        }

        ImGui::Text("%.12f", *d);
    }

    void ShowInt(int* d)
    {
        ImGui::PushItemWidth(400);
        if (!d)
        {
            ImGui::Text("0");
            return;
        }

        ImGui::Text("%d", *d);
    }
};

#endif // __NODE_MENU_H__