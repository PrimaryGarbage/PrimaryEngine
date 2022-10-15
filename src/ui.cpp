#include "ui.hpp"
#include "renderer.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "globals.hpp"
#include "input.hpp"
#include "ImGuiFileDialog.h"
#include "drawable.hpp"

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

            if (selectedNode) selectedNode->renderFields();

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
                    if (ImGui::BeginDragDropTarget())
                    {
                        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dragNodePayloadType);
                        if (payload)
                        {
                            Node* payloadNode = *static_cast<Node**>(payload->Data);
                            payloadNode->orphanize();
                            currentScene->addChild(payloadNode);
                        }
                        ImGui::EndDragDropTarget();
                    }

                    for (Node* child : currentScene->getChildren())
                    {
                        drawNodeInTree(child);
                    }

                    ImGui::TreePop();
                }

                ImGui::Separator();

                if (ImGui::Button("Load Scene"))
                    ImGuiFileDialog::Instance()->OpenDialog("LoadSceneKey", "Open Scene", ".psc", "./res/scenes/", 1, nullptr, ImGuiFileDialogFlags_Modal);

                if (ImGuiFileDialog::Instance()->Display("LoadSceneKey", 32, fileExplorerMinSize))
                {
                    if (ImGuiFileDialog::Instance()->IsOk())
                    {
                        std::string name = ImGuiFileDialog::Instance()->GetCurrentFileName();
                        name = Utils::removeSceneFileExtension(name);
                        app->loadCurrentScene(name);
                    }
                    ImGuiFileDialog::Instance()->Close();
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
        if (ImGui::IsItemHovered() && Input::isJustReleased(MouseButton::left) && !ImGui::IsItemToggledOpen()) selectedNode = node;
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
    
    void UI::drawSelectedNodeFraming()
    {
        if(!selectedNode) return;
        Drawable* node = dynamic_cast<Drawable*>(selectedNode);
        if(node) renderer->drawSelectedNodeFraming(node);
    }

    UI::~UI()
    {
        if(!initialized) return;
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
        initialized = true;
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

        drawSelectedNodeFraming();

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