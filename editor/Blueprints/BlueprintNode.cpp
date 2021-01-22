//
//  BlueprintNode.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#include "BlueprintNode.h"
#include "Blueprint.h"

namespace LutraEditor {

    static int GetId()
    {
        static int id = 1;
        return id++;
    }

    BlueprintPin::BlueprintPin(const std::string& name, PinType type, const std::string& scriptName)
    : Name(name)
    , Type(type)
    , ScriptName(scriptName)
    {
        m_ID = GetId();
    }

    BlueprintLink::BlueprintLink(const BlueprintPin& sPin, const BlueprintPin& ePin, const glm::vec4& color)
    : StartPin(sPin)
    , EndPin(ePin)
    , Color(color)
    {
        m_ID = GetId();
    }

    BlueprintNode::BlueprintNode()
    {
        Type = NodeType::Blueprint;
        m_ID = GetId();
    }

    void BlueprintNode::addInput(const BlueprintPin& pin)
    {
        m_inputs.push_back(pin);
        m_inputs.back().m_node = this;
        m_inputs.back().m_kind = PinKind::Input;
    }

    void BlueprintNode::addOutput(const BlueprintPin& pin)
    {
        m_outputs.push_back(pin);
        m_outputs.back().m_node = this;
        m_outputs.back().m_kind = PinKind::Output;
    }

    std::string BlueprintNode::findLinkName(const BlueprintPin& pin)
    {
        auto link = m_bluePrint->FindLink(pin.GetID());
        if (link)
            return link->StartPin.ScriptName;
        return pin.ScriptName;
    }
}
