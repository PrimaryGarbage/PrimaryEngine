#include "ui.hpp"
#include "renderer.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "globals.hpp"
#include "node.hpp"

namespace prim
{
    void UI::drawRightPanel()
    {
        const static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
        const ImVec2 panelPos(renderer->getWindowWidth(), 0.0f);
        static ImVec2 panelSize(rightPanelWidth, renderer->getWindowHeight());
        ImGui::SetNextWindowSize(panelSize);
        ImGui::SetNextWindowPos(panelPos, 0, ImVec2(1.0f, 0.0f));

        ImGui::Begin("Node Info", nullptr, flags);
        {
            panelSize = ImVec2(ImGui::GetWindowSize().x, renderer->getWindowHeight());

            if(selectedNode) selectedNode->visualizeOnUi();

            ImGui::Separator();

            for (const std::string& str : printLines)
            {
                ImGui::Text(str.c_str());
            }

            ImGui::Separator();

            for (const auto& df : dragFloats)
            {
                ImGui::DragFloat(df.label, df.f, df.speed);
            }

        }
        ImGui::End();
    }

    void UI::drawLeftPanel()
    {
        const static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
        static ImVec2 panelSize(rightPanelWidth, renderer->getWindowHeight());
        ImGui::SetNextWindowSize(panelSize);
        ImGui::SetNextWindowPos(ImVec2());

        ImGui::Begin("Scene", nullptr, flags);
        {
            panelSize = ImVec2(ImGui::GetWindowSize().x, renderer->getWindowHeight());

            static PrimaryApp* app = Globals::app;
            Node* currentScene = app->getCurrentScene();
            if (currentScene)
            {
                if (ImGui::TreeNode(currentScene->getName().c_str()))
                {
                    for (Node* child : currentScene->getChildren())
                    {
                        drawNodeInTree(child);
                    }
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }

    void UI::drawNodeInTree(Node* node)
    {
        static const ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
        ImGuiTreeNodeFlags flags = baseFlags;
        if (node == selectedNode) flags |= ImGuiTreeNodeFlags_Selected;
        if (node->getChildren().empty()) flags |= ImGuiTreeNodeFlags_Leaf;
        bool nodeIsOpen = ImGui::TreeNodeEx(node->getName().c_str(), flags);
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) selectedNode = node;
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(dragNodePayloadType, static_cast<void*>(&node), sizeof(size_t), ImGuiCond_Once);
            ImGui::Text(node->getName().c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dragNodePayloadType);
            if (payload)
            {
                Node* payloadNode = *static_cast<Node**>(payload->Data);
                payloadNode->orphanize();
                node->addChild(payloadNode);
            }
            ImGui::EndDragDropTarget();
        }
        if (nodeIsOpen)
        {
            for (Node* child : node->getChildren()) drawNodeInTree(child);
            ImGui::TreePop();
        }
    }
    

    UI::~UI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void UI::init(Renderer* renderer)
    {
        this->renderer = renderer;
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(renderer->getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    void UI::draw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /// Drawing happends here ///
        drawRightPanel();
        drawLeftPanel();

        //ImGui::ShowDemoWindow();

        /////////////////////////////

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        printLines.clear();
        dragFloats.clear();
    }

    void UI::print(std::string str)
    {
        printLines.push_back(std::move(str));
    }

    void UI::addDragFloat(const char* label, float* f, float speed)
    {
        dragFloats.push_back(DragFloat{ label, f, speed });
    }
}