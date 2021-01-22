//
//  Blueprint.hpp
//  Editor
//
//  Created by JasonCheng on 2021/1/18.
//

#ifndef Blueprint_hpp
#define Blueprint_hpp
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <imgui-node-editor/imgui_node_editor.h>
#include "BlueprintNode.h"
#include "BlueprintFactory.h"

namespace LutraEditor {

    namespace ed = ax::NodeEditor;

    class BlueprintVM;
    class Blueprint
    {
    public:
        
        Blueprint(const std::shared_ptr<BlueprintVM>& vm);
        
        bool OnGui(uint32_t width = 0, uint32_t height = 0);
        
        void AddDefaultBlueprint(const std::shared_ptr<BlueprintFactory>& factory);
        void AddTemplate(const std::shared_ptr<BlueprintFactory>& factory);
        const std::vector<BlueprintNode*>& GetSelectedNodes() const;
        
        bool IsPinLinked(ed::PinId id);
        BlueprintPin* FindPin(ed::PinId id) const;
        BlueprintNode* FindNode(ed::NodeId id) const;
        BlueprintLink* FindLink(ed::LinkId id) const;
        BlueprintLink* FindLink(ed::PinId id) const;
        
        void Compile();
        
    private:
        
        void fetchNodes(BlueprintNode* node, std::vector<BlueprintNode*>& nodes);
        
        std::shared_ptr<BlueprintNode> buildNode(const std::shared_ptr<BlueprintFactory>& factory);
        
    private:
        
        ed::EditorContext*  m_editor = nullptr;
        std::shared_ptr<BlueprintVM> m_vm;
        std::vector<std::shared_ptr<BlueprintNode>> m_nodes;
        std::vector<std::shared_ptr<BlueprintLink>> m_links;
        std::vector<BlueprintNode*> m_selectedNodes;
        std::shared_ptr<BlueprintNode> m_defaultNode;
        std::unordered_map<std::string, std::vector<std::shared_ptr<BlueprintFactory>>> m_templates;
        
        uint32_t m_backgroundTexID;
        uint32_t m_backgroundWidth, m_backgroundHeight;
    };
}

#endif /* Blueprint_hpp */
