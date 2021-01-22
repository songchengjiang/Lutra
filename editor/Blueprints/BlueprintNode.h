//
//  BlueprintNode.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/19.
//

#ifndef BlueprintNode_hpp
#define BlueprintNode_hpp
#include <string>
#include <vector>
#include <Lutra.h>
#include <imgui-node-editor/imgui_node_editor.h>

namespace LutraEditor {

    namespace ed = ax::NodeEditor;

    enum class PinType
    {
        Flow,
        Bool,
        Int,
        Float,
        Float2,
        Float3,
        Float4,
        String,
        Object,
        Function,
        Delegate,
    };

    enum class PinKind
    {
        Output,
        Input
    };

    enum class NodeType
    {
        Blueprint,
        Simple,
        Tree,
        Comment
    };

    struct BlueprintNode;
    struct BlueprintLink;
    struct BlueprintPin
    {
        BlueprintPin(const std::string& name, PinType type, const std::string& scriptName);
        std::string Name;
        PinType   Type;
        
        std::string ScriptName;
        
        BlueprintNode* GetNode() const { return m_node; }
        PinKind GetKind() const { return m_kind; }
        ed::PinId GetID() const { return m_ID; }
        
    private:
        BlueprintNode* m_node;
        ed::PinId m_ID;
        PinKind   m_kind;
        friend class BlueprintNode;
    };

    struct BlueprintLink
    {
        BlueprintLink(const BlueprintPin& sPin, const BlueprintPin& ePin, const glm::vec4& color);
        
        BlueprintPin StartPin;
        BlueprintPin EndPin;
        glm::vec4    Color;
        
        ed::LinkId GetID() const { return m_ID; }
        
    private:
        
        ed::LinkId m_ID;
    };

    struct Node
    {
        virtual ~Node() = default;
        NodeType Type;
    };

    class Blueprint;
    class BlueprintVM;
    struct BlueprintNode : public Node
    {
        BlueprintNode();
        virtual ~BlueprintNode() = default;
        
        std::string Title;
        std::string Name;
        glm::vec4   Color;
        
        bool CanRemove = true;
        
        void addInput(const BlueprintPin& pin);
        void addOutput(const BlueprintPin& pin);
        std::vector<BlueprintPin>& GetInputs() { return m_inputs; }
        std::vector<BlueprintPin>& GetOutputs() { return m_outputs; }
        
        ed::NodeId GetID() const { return m_ID; }
        
        virtual bool OnGUI(){ return false; };
        virtual void OnInit(BlueprintVM* vm) = 0;
        virtual bool Compile(BlueprintVM* vm) = 0;
        
    protected:
        
        std::string findLinkName(const BlueprintPin& pin);
        
    protected:
        
        Blueprint* m_bluePrint{nullptr};
        ed::NodeId m_ID;
        std::vector<BlueprintPin> m_inputs;
        std::vector<BlueprintPin> m_outputs;
        friend class Blueprint;
    };
}

#endif /* BlueprintNode_hpp */
