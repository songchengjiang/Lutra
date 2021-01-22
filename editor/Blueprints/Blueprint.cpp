//
//  Blueprint.cpp
//  Editor
//
//  Created by JasonCheng on 2021/1/18.
//

#include "Blueprint.h"
#include <stb_image.h>
#include "builders.h"
#include "widgets.h"
#include "TextureManager.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include "BlueprintVM.h"

namespace LutraEditor {

    namespace util = ax::NodeEditor::Utilities;

    static inline ImRect ImGui_GetItemRect()
    {
        return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    }

    static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
    {
        auto result = rect;
        result.Min.x -= x;
        result.Min.y -= y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    static bool CanCreateLink(const BlueprintPin* a, const BlueprintPin* b)
    {
        if (!a || !b || a == b || a->GetKind() == b->GetKind() || a->Type != b->Type || a->GetNode() == b->GetNode())
            return false;

        return true;
    }

    static glm::vec4 GetIconColor(PinType type)
    {
        switch (type)
        {
            default:
            case PinType::Flow:     return glm::vec4(255 / 255.0, 255 / 255.0, 255 / 255.0, 1.0);
            case PinType::Bool:     return glm::vec4(220 / 255.0,  48 / 255.0,  48 / 255.0, 1.0);
            case PinType::Int:      return glm::vec4( 68 / 255.0, 201 / 255.0, 156 / 255.0, 1.0);
            case PinType::Float:    return glm::vec4(147 / 255.0, 226 / 255.0,  74 / 255.0, 1.0);
            case PinType::Float2:   return glm::vec4(147 / 255.0, 126 / 255.0,  74 / 255.0, 1.0);
            case PinType::Float3:   return glm::vec4(147 / 255.0, 126 / 255.0,  174 / 255.0, 1.0);
            case PinType::Float4:   return glm::vec4(255 / 255.0,  255 / 255.0,  255 / 255.0, 1.0);
            case PinType::String:   return glm::vec4(124 / 255.0,  21 / 255.0, 153 / 255.0, 1.0);
            case PinType::Object:   return glm::vec4( 51 / 255.0, 150 / 255.0, 215 / 255.0, 1.0);
            case PinType::Function: return glm::vec4(218 / 255.0,   0 / 255.0, 183 / 255.0, 1.0);
            case PinType::Delegate: return glm::vec4(255 / 255.0,  48 / 255.0,  48 / 255.0, 1.0);
        }
    };


    static void DrawPinIcon(const BlueprintPin& pin, bool connected, int alpha)
    {
        ax::Drawing::IconType iconType;
        glm::vec4 color = GetIconColor(pin.Type);
        color.w = alpha / 255.0f;
        switch (pin.Type)
        {
            case PinType::Flow:     iconType = ax::Drawing::IconType::Flow;   break;
            case PinType::Bool:     iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Int:      iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Float:    iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Float2:   iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Float3:   iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Float4:   iconType = ax::Drawing::IconType::Circle; break;
            case PinType::String:   iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Object:   iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Function: iconType = ax::Drawing::IconType::Circle; break;
            case PinType::Delegate: iconType = ax::Drawing::IconType::Square; break;
            default:
                return;
        }

        ax::Widgets::Icon(ImVec2(24, 24), iconType, connected, {color.r, color.g, color.b, color.a}, ImColor(32, 32, 32, alpha));
    };

    Blueprint::Blueprint(const std::shared_ptr<BlueprintVM>& vm)
    : m_vm(vm)
    {
        int width, height, channels;
        stbi_uc* data = nullptr;
        {
            data = stbi_load("Resources/BlueprintBackground.png", &width, &height, &channels, 0);
        }
        
        m_backgroundTexID = TextureManager::Instance().CreateTextue(width, height, channels == 3? Lutra::TextureFormat::RGB8: Lutra::TextureFormat::RGBA8, data);
        m_backgroundWidth = width;
        m_backgroundHeight = height;
        
        ed::Config config;

        config.SettingsFile = "Blueprints.json";

//        config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t
//        {
//            auto node = FindNode(nodeId);
//            if (!node)
//                return 0;
//
//            if (data != nullptr)
//                memcpy(data, node->State.data(), node->State.size());
//            return node->State.size();
//        };
//
//        config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason, void* userPointer) -> bool
//        {
//            auto node = FindNode(nodeId);
//            if (!node)
//                return false;
//
//            node->State.assign(data, size);
//
//            TouchNode(nodeId);
//
//            return true;
//        };

        m_editor = ed::CreateEditor(&config);
        ed::SetCurrentEditor(m_editor);
    }

    bool Blueprint::OnGui(uint32_t width, uint32_t height)
    {
        bool isChanged = false;
        static bool createNewNode  = false;
        static BlueprintPin* newLinkPin = nullptr;
        static BlueprintPin* newNodeLinkPin = nullptr;
        
        ed::Begin("Editor", {(float)width, (float)height});
        
        util::BlueprintNodeBuilder builder(reinterpret_cast<ImTextureID>(m_backgroundTexID), m_backgroundWidth, m_backgroundHeight);
        
        for (auto& node : m_nodes)
        {
            if (node->Type != NodeType::Blueprint && node->Type != NodeType::Simple)
                continue;

            const auto isSimple = node->Type == NodeType::Simple;

            bool hasOutputDelegates = false;
            for (auto& output : node->GetOutputs())
                if (output.Type == PinType::Delegate)
                    hasOutputDelegates = true;

            builder.Begin(node->GetID());
                if (!isSimple)
                {
                    builder.Header({node->Color.r, node->Color.g, node->Color.b, node->Color.a});
                        ImGui::Spring(0);
                        ImGui::TextUnformatted(node->Title.c_str());
                        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", node->Name.c_str());
                        ImGui::Spring(1);
                        ImGui::Dummy(ImVec2(0, 28));
                        if (hasOutputDelegates)
                        {
                            ImGui::BeginVertical("delegates", ImVec2(0, 28));
                            ImGui::Spring(1, 0);
                            for (auto& output : node->GetOutputs())
                            {
                                if (output.Type != PinType::Delegate)
                                    continue;

                                auto alpha = ImGui::GetStyle().Alpha;
                                if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                                    alpha = alpha * (48.0f / 255.0f);

                                ed::BeginPin(output.GetID(), ed::PinKind::Output);
                                ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
                                ed::PinPivotSize(ImVec2(0, 0));
                                ImGui::BeginHorizontal(output.GetID().AsPointer());
                                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                                if (!output.Name.empty())
                                {
                                    ImGui::TextUnformatted(output.Name.c_str());
                                    ImGui::Spring(0);
                                }
                                DrawPinIcon(output, IsPinLinked(output.GetID()), (int)(alpha * 255));
                                ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
                                ImGui::EndHorizontal();
                                ImGui::PopStyleVar();
                                ed::EndPin();

                                //DrawItemRect(ImColor(255, 0, 0));
                            }
                            ImGui::Spring(1, 0);
                            ImGui::EndVertical();
                            ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
                        }
                        else
                            ImGui::Spring(0);
                    builder.EndHeader();
                }

                for (auto& input : node->GetInputs())
                {
                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    builder.Input(input.GetID());
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                    DrawPinIcon(input, IsPinLinked(input.GetID()), (int)(alpha * 255));
                    ImGui::Spring(0);
                    if (!input.Name.empty())
                    {
                        ImGui::TextUnformatted(input.Name.c_str());
                        ImGui::Spring(0);
                    }
                    ImGui::PopStyleVar();
                    builder.EndInput();
                }

                if (isSimple)
                {
                    builder.Middle();

                    ImGui::Spring(1, 0);
                    ImGui::TextUnformatted(node->Name.c_str());
                    ImGui::Spring(1, 0);
                }

                for (auto& output : node->GetOutputs())
                {
                    if (!isSimple && output.Type == PinType::Delegate)
                        continue;

                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                    builder.Output(output.GetID());
                    if (output.Type == PinType::String)
                    {
                        static char buffer[128] = "Edit Me\nMultiline!";
                        static bool wasActive = false;

                        ImGui::PushItemWidth(100.0f);
                        ImGui::InputText("##edit", buffer, 127);
                        ImGui::PopItemWidth();
                        if (ImGui::IsItemActive() && !wasActive)
                        {
                            ed::EnableShortcuts(false);
                            wasActive = true;
                        }
                        else if (!ImGui::IsItemActive() && wasActive)
                        {
                            ed::EnableShortcuts(true);
                            wasActive = false;
                        }
                        ImGui::Spring(0);
                    }
                    if (!output.Name.empty())
                    {
                        ImGui::Spring(0);
                        ImGui::TextUnformatted(output.Name.c_str());
                    }
                    ImGui::Spring(0);
                    DrawPinIcon(output, IsPinLinked(output.GetID()), (int)(alpha * 255));
                    ImGui::PopStyleVar();
                    builder.EndOutput();
                }
            builder.End();
        }
        
        for (auto& node : m_nodes)
        {
            if (node->Type != NodeType::Comment)
                continue;

            const float commentAlpha = 0.75f;

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
            ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
            ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
            ed::BeginNode(node->GetID());
            ImGui::PushID(node->GetID().AsPointer());
            ImGui::BeginVertical("content");
            ImGui::BeginHorizontal("horizontal");
            ImGui::Spring(1);
            ImGui::TextUnformatted(node->Name.c_str());
            ImGui::Spring(1);
            ImGui::EndHorizontal();
            //ed::Group(node.Size);
            ImGui::EndVertical();
            ImGui::PopID();
            ed::EndNode();
            ed::PopStyleColor(2);
            ImGui::PopStyleVar();

            if (ed::BeginGroupHint(node->GetID()))
            {
                //auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
                auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

                //ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

                auto min = ed::GetGroupMin();
                //auto max = ed::GetGroupMax();

                ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
                ImGui::BeginGroup();
                ImGui::TextUnformatted(node->Name.c_str());
                ImGui::EndGroup();

                auto drawList = ed::GetHintBackgroundDrawList();

                auto hintBounds      = ImGui_GetItemRect();
                auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

                drawList->AddRectFilled(
                    hintFrameBounds.GetTL(),
                    hintFrameBounds.GetBR(),
                    IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f);

                drawList->AddRect(
                    hintFrameBounds.GetTL(),
                    hintFrameBounds.GetBR(),
                    IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f);

                //ImGui::PopStyleVar();
            }
            ed::EndGroupHint();
        }

        for (auto& link : m_links)
            ed::Link(link->GetID(), link->StartPin.GetID(), link->EndPin.GetID(), {link->Color.r, link->Color.g, link->Color.b, link->Color.a}, 2.0f);

        if (!createNewNode)
        {
            if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
            {
                auto showLabel = [](const char* label, ImColor color)
                {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
                    auto size = ImGui::CalcTextSize(label);

                    auto padding = ImGui::GetStyle().FramePadding;
                    auto spacing = ImGui::GetStyle().ItemSpacing;

                    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

                    auto rectMin = ImGui::GetCursorScreenPos() - padding;
                    auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

                    auto drawList = ImGui::GetWindowDrawList();
                    drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
                    ImGui::TextUnformatted(label);
                };

                ed::PinId startPinId = 0, endPinId = 0;
                if (ed::QueryNewLink(&startPinId, &endPinId))
                {
                    auto startPin = FindPin(startPinId);
                    auto endPin   = FindPin(endPinId);

                    newLinkPin = startPin ? startPin : endPin;

                    if (startPin->GetKind() == PinKind::Input)
                    {
                        std::swap(startPin, endPin);
                        std::swap(startPinId, endPinId);
                    }

                    if (startPin && endPin)
                    {
                        if (endPin == startPin)
                        {
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                        }
                        else if (endPin->GetKind() == startPin->GetKind())
                        {
                            showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                        }
                        //else if (endPin->Node == startPin->Node)
                        //{
                        //    showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                        //    ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                        //}
                        else if (endPin->Type != startPin->Type)
                        {
                            showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                            ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                        }
                        else
                        {
                            showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                            if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                            {
                                m_links.emplace_back(new BlueprintLink(*startPin, *endPin, GetIconColor(startPin->Type)));
                                isChanged = true;
                            }
                        }
                    }
                }

                ed::PinId pinId = 0;
                if (ed::QueryNewNode(&pinId))
                {
                    newLinkPin = FindPin(pinId);
                    if (newLinkPin)
                        showLabel("+ Create Node", ImColor(32, 45, 32, 180));

                    if (ed::AcceptNewItem())
                    {
                        createNewNode  = true;
                        newNodeLinkPin = FindPin(pinId);
                        newLinkPin = nullptr;
                        ed::Suspend();
                        ImGui::OpenPopup("Create New Node");
                        ed::Resume();
                    }
                }
            }
            else
                newLinkPin = nullptr;

            ed::EndCreate();

            if (ed::BeginDelete())
            {
                ed::LinkId linkId = 0;
                while (ed::QueryDeletedLink(&linkId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        auto id = std::find_if(m_links.begin(), m_links.end(), [linkId](auto& link) { return link->GetID() == linkId; });
                        if (id != m_links.end())
                            m_links.erase(id);
                        isChanged = true;
                    }
                }

                ed::NodeId nodeId = 0;
                while (ed::QueryDeletedNode(&nodeId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        auto id = std::find_if(m_nodes.begin(), m_nodes.end(), [nodeId](auto& node) { return node->GetID() == nodeId; });
                        if (id != m_nodes.end())
                            m_nodes.erase(id);
                    }
                }
            }
            ed::EndDelete();
        }
        
        static ed::NodeId contextNodeId      = 0;
        static ed::LinkId contextLinkId      = 0;
        static ed::PinId  contextPinId       = 0;
        
        ImVec2 openPopupPosition = ImGui::GetMousePos();
        ed::Suspend();
        if (ed::ShowNodeContextMenu(&contextNodeId))
            ImGui::OpenPopup("Node Context Menu");
        else if (ed::ShowPinContextMenu(&contextPinId))
            ImGui::OpenPopup("Pin Context Menu");
        else if (ed::ShowLinkContextMenu(&contextLinkId))
            ImGui::OpenPopup("Link Context Menu");
        else if (ed::ShowBackgroundContextMenu())
        {
            ImGui::OpenPopup("Create New Node");
            newNodeLinkPin = nullptr;
        }
        ed::Resume();

        ed::Suspend();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
        if (ImGui::BeginPopup("Node Context Menu"))
        {
            auto node = FindNode(contextNodeId);

            ImGui::TextUnformatted("Node Context Menu");
            ImGui::Separator();
            if (node)
            {
                ImGui::Text("ID: %p", node->GetID().AsPointer());
                ImGui::Text("Type: %s", node->Type == NodeType::Blueprint ? "Blueprint" : (node->Type == NodeType::Tree ? "Tree" : "Comment"));
                ImGui::Text("Inputs: %d", (int)node->GetInputs().size());
                ImGui::Text("Outputs: %d", (int)node->GetOutputs().size());
            }
            else
                ImGui::Text("Unknown node: %p", contextNodeId.AsPointer());
            ImGui::Separator();
            if (node->CanRemove && ImGui::MenuItem("Delete"))
                ed::DeleteNode(contextNodeId);
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("Pin Context Menu"))
        {
            auto pin = FindPin(contextPinId);

            ImGui::TextUnformatted("Pin Context Menu");
            ImGui::Separator();
            if (pin)
            {
                ImGui::Text("ID: %p", pin->GetID().AsPointer());
                if (pin->GetNode())
                    ImGui::Text("Node: %p", pin->GetNode()->GetID().AsPointer());
                else
                    ImGui::Text("Node: %s", "<none>");
            }
            else
                ImGui::Text("Unknown pin: %p", contextPinId.AsPointer());

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("Link Context Menu"))
        {
            auto link = FindLink(contextLinkId);

            ImGui::TextUnformatted("Link Context Menu");
            ImGui::Separator();
            if (link)
            {
                ImGui::Text("ID: %p", link->GetID().AsPointer());
                ImGui::Text("From: %p", link->StartPin.GetID().AsPointer());
                ImGui::Text("To: %p", link->EndPin.GetID().AsPointer());
            }
            else
                ImGui::Text("Unknown link: %p", contextLinkId.AsPointer());
            ImGui::Separator();
            if (ImGui::MenuItem("Delete"))
                ed::DeleteLink(contextLinkId);
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("Create New Node"))
        {
            auto newNodePostion = openPopupPosition;
            
            std::shared_ptr<BlueprintNode> node;
            for (auto &temp : m_templates) {
                ImGui::Separator();
                ImGui::MenuItem(temp.first.c_str(), NULL, false, false);
                for (auto &factory : temp.second) {
                    if (ImGui::MenuItem(factory->GetType().c_str()))
                        node = buildNode(factory);
                }
            }

            if (node)
            {
                createNewNode = false;

                ed::SetNodePosition(node->GetID(), newNodePostion);

                if (auto startPin = newNodeLinkPin)
                {
                    auto& pins = startPin->GetKind() == PinKind::Input ? node->GetOutputs() : node->GetInputs();

                    for (auto& pin : pins)
                    {
                        if (CanCreateLink(startPin, &pin))
                        {
                            auto endPin = &pin;
                            if (startPin->GetKind() == PinKind::Input)
                                std::swap(startPin, endPin);

                            m_links.emplace_back(new BlueprintLink(*startPin, *endPin, GetIconColor(startPin->Type)));
                            isChanged = true;
                            break;
                        }
                    }
                }
            }

            ImGui::EndPopup();
        }
        else
            createNewNode = false;
        ImGui::PopStyleVar();
        ed::Resume();
        
        m_selectedNodes.clear();
        int maxCount = ed::GetSelectedObjectCount();
        if (0 < maxCount) {
            ed::NodeId *nodes = new ed::NodeId[maxCount];
            int nodeCount = ed::GetSelectedNodes(nodes, maxCount);
            if (0 < nodeCount) {
                auto bpNode = FindNode(nodes[0]);
                m_selectedNodes.push_back(bpNode);
            }
            delete [] nodes;
        }
        
        ed::End();
        return isChanged;
    }

    void Blueprint::AddDefaultBlueprint(const std::shared_ptr<BlueprintFactory>& factory)
    {
        m_defaultNode = buildNode(factory);
        m_defaultNode->CanRemove = false;
    }

    void Blueprint::AddTemplate(const std::shared_ptr<BlueprintFactory>& factory)
    {
        m_templates[factory->GetGroup()].emplace_back(factory);
    }

    const std::vector<BlueprintNode*>& Blueprint::GetSelectedNodes() const
    {
        return m_selectedNodes;
    }

    void Blueprint::fetchNodes(BlueprintNode* node, std::vector<BlueprintNode*>& nodes)
    {
        for (auto& input : node->GetInputs()) {
            auto link = FindLink(input.GetID());
            if (link) {
                fetchNodes(link->StartPin.GetNode(), nodes);
            }
        }
        if (auto iter = std::find(nodes.begin(), nodes.end(), node) == nodes.end())
            nodes.push_back(node);
    }

    void Blueprint::Compile()
    {
        std::vector<BlueprintNode*> nodes;
        fetchNodes(m_defaultNode.get(), nodes);
        for (auto& node : nodes) {
            node->Compile(m_vm.get());
        }
        m_vm->Compile();
    }

    bool Blueprint::IsPinLinked(ed::PinId id)
    {
        if (!id)
            return false;

        for (auto& link : m_links)
            if (link->StartPin.GetID() == id || link->EndPin.GetID() == id)
                return true;

        return false;
    }

    BlueprintPin* Blueprint::FindPin(ed::PinId id) const
    {
        if (!id)
            return nullptr;

        for (auto& node : m_nodes)
        {
            for (auto& pin : node->GetInputs())
                if (pin.GetID() == id)
                    return &pin;

            for (auto& pin : node->GetOutputs())
                if (pin.GetID() == id)
                    return &pin;
        }

        return nullptr;
    }

    BlueprintNode* Blueprint::FindNode(ed::NodeId id) const
    {
        for (auto& node : m_nodes)
            if (node->GetID() == id)
                return node.get();

        return nullptr;
    }

    BlueprintLink* Blueprint::FindLink(ed::LinkId id) const
    {
        for (auto& link : m_links)
            if (link->GetID() == id)
                return link.get();

        return nullptr;
    }

    BlueprintLink* Blueprint::FindLink(ed::PinId id) const
    {
        for (auto& link : m_links) {
            if (link->StartPin.GetID() == id)
                return link.get();
            else if (link->EndPin.GetID() == id)
                return link.get();
        }
        return nullptr;
    }

    std::shared_ptr<BlueprintNode> Blueprint::buildNode(const std::shared_ptr<BlueprintFactory>& factory)
    {
        m_nodes.emplace_back(factory->Create());
        m_nodes.back()->m_bluePrint = this;
        m_nodes.back()->OnInit(m_vm.get());
        return m_nodes.back();
    }

}
