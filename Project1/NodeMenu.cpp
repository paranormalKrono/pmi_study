#include "Node_menu.h"

#include <stdio.h>

namespace ed = ax::NodeEditor;

int NodeMenu::GetNextId()
{
    return m_NextId++;
}

ed::LinkId NodeMenu::GetNextLinkId()
{
    return ed::LinkId(GetNextId());
}

void NodeMenu::TouchNode(ed::NodeId id)
{
    m_NodeTouchTime[id] = m_TouchTime;
}

float NodeMenu::GetTouchProgress(ed::NodeId id)
{
    auto it = m_NodeTouchTime.find(id);
    if (it != m_NodeTouchTime.end() && it->second > 0.0f)
        return (m_TouchTime - it->second) / m_TouchTime;
    else
        return 0.0f;
}

void NodeMenu::UpdateTouch()
{
    const auto deltaTime = ImGui::GetIO().DeltaTime;
    for (auto& entry : m_NodeTouchTime)
    {
        if (entry.second > 0.0f)
            entry.second -= deltaTime;
    }
}

Node* NodeMenu::FindNode(ed::NodeId id)
{
    for (auto& node : m_Nodes)
        if (node.ID == id)
            return &node;

    return nullptr;
}

Link* NodeMenu::FindLink(ed::LinkId id)
{
    for (auto& link : m_Links)
        if (link.ID == id)
            return &link;

    return nullptr;
}

Pin* NodeMenu::FindPin(ed::PinId id)
{
    if (!id)
        return nullptr;

    for (auto& node : m_Nodes)
    {
        for (auto& pin : node.Inputs)
            if (pin.ID == id)
                return &pin;

        for (auto& pin : node.Outputs)
            if (pin.ID == id)
                return &pin;
    }

    return nullptr;
}

bool NodeMenu::IsPinLinked(ed::PinId id)
{
    if (!id)
        return false;

    for (auto& link : m_Links)
        if (link.StartPin->ID == id || link.EndPin->ID == id)
            return true;

    return false;
}

bool NodeMenu::CanCreateLink(Pin* a, Pin* b)
{
    return a && b && a != b && 
        a->IOKind != b->IOKind && 
        a->DataType == b->DataType &&
        a->Node != b->Node &&
        (a->IOKind != PinIOKind::Output || b->links.size() != 1) && 
        (a->IOKind != PinIOKind::Input || a->links.size() != 1);
}

void NodeMenu::BuildNode(Node* node)
{
    for (auto& input : node->Inputs)
    {
        input.Node = node;
        input.IOKind = PinIOKind::Input;
    }

    for (auto& output : node->Outputs)
    {
        output.Node = node;
        output.IOKind = PinIOKind::Output;
    }
}

void NodeMenu::BuildNodes()
{
    for (auto& node : m_Nodes)
        BuildNode(&node);
}
