#include "graphics/renderer.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include "globals.hpp"
#include "input.hpp"
#include "ImGuiFileDialog.h"
#include "nodes/drawable.hpp"
#include "resource_manager.hpp"
#include "graphics/primitives.hpp"
#include "GLM/gtc/matrix_transform.hpp"

namespace prim
{
    Node* nodeToAddNewNodeTo = nullptr;
    Node* nodeToAddLoadedSceneTo = nullptr;

    SceneEditor::SceneEditor(Renderer* renderer):
        renderer(renderer), positionPointMesh(Primitives::createSquareMesh(positionPointSize))
    {
        positionPointMesh.compositions.front().shader = Shader::getDefaultShader(DefaultShader::plainColor);
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(renderer->getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        Logger::inst().logInfo("Scene editor initialized");
    }

    SceneEditor::~SceneEditor()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void SceneEditor::drawRightPanel()
    {
        const static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
        const ImVec2 panelPos(renderer->getWindowWidth(), 0.0f);
        static ImVec2 panelSize(rightPanelWidth, renderer->getWindowHeight());
        ImGui::SetNextWindowSize(panelSize);
        ImGui::SetNextWindowPos(panelPos, 0, ImVec2(1.0f, 0.0f));

        ImGui::Begin("Node Info", nullptr, flags);
        {
            panelSize = ImVec2(ImGui::GetWindowSize().x, renderer->getWindowHeight());

            if (selectedNode) selectedNode->renderFields(this);

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

    void SceneEditor::drawLeftPanel()
    {
        const static ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
        static ImVec2 panelSize(rightPanelWidth, renderer->getWindowHeight());
        ImGui::SetNextWindowSize(panelSize);
        ImGui::SetNextWindowPos(ImVec2());

        ImGui::Begin("Scene", nullptr, flags);
        {
            panelSize = ImVec2(ImGui::GetWindowSize().x, renderer->getWindowHeight());

            Node* currentScene = Globals::app->getCurrentScene();

            drawNodeInTree(currentScene);

            ImGui::Separator();
            drawLoadSceneButton();
            ImGui::SameLine();
            drawSaveSceneButton();

            drawCreateNodeMenu();
            drawLoadNodeMenu();
        }
        ImGui::End();
    }

    void SceneEditor::drawNodeInTree(Node* node)
    {
        static const ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
        ImGuiTreeNodeFlags flags = baseFlags;
        if (node == selectedNode) flags |= ImGuiTreeNodeFlags_Selected;
        if (node->getChildren().empty()) flags |= ImGuiTreeNodeFlags_Leaf;
        bool nodeIsOpen = ImGui::TreeNodeEx(node->getName().c_str(), flags);
        bool hovered = ImGui::IsItemHovered();

        if (hovered && Input::isJustReleased(MouseButton::left) && !ImGui::IsItemToggledOpen())
        {
            selectedNode = node;
        }

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
            for(int i = 0; i < node->children.size(); ++i)
                drawNodeInTree(node->children[i]);
            ImGui::TreePop();
        }

        // this must reside at the bottom, because of potential
        // deletion of the current tree node from context menu
        drawNodeTreeContextMenu(node, hovered);
    }

    void SceneEditor::drawSelectedNodeFraming()
    {
        if (!selectedNode) return;
        Drawable* node = dynamic_cast<Drawable*>(selectedNode);
        if (node) renderer->drawNodeFrame(node, Utils::Color::Green, 1.4f);
    }

    void SceneEditor::drawSelectedNodePositionPoint(glm::vec2 position)
    {
        const float elapsedTime = Globals::app->getElapsedTime();
        const float sinTime = std::pow(std::sin(elapsedTime * 4.0f), 2);
        const glm::vec4 color(sinTime, sinTime, sinTime, 1.0f);
        static Shader* shader = positionPointMesh.compositions.front().shader;
        static const glm::vec2 halfSizeVec = glm::vec2(positionPointSize) / 2.0f;
        static constexpr float rotationSpeed = 0.2f;
        static float rotation = 0.0f;

        if (!selectedNode) return;
        shader->setUniform4f("u_color", color);
        shader->setUniform1f("u_time", Globals::app->getElapsedTime());
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, Utils::toVec3(position, 1.0f));
        rotation += rotationSpeed;
        if (rotation > Utils::twoPi) rotation -= Utils::twoPi;
        modelMat = glm::rotate(modelMat, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::translate(modelMat, Utils::toVec3(-halfSizeVec));
        renderer->setModelMat(modelMat);
        renderer->drawMesh(positionPointMesh);
    }

    void SceneEditor::drawLoadSceneButton()
    {
        if (ImGui::Button("Load Scene"))
            ImGuiFileDialog::Instance()->OpenDialog("LoadSceneKey", "Open Scene", ".psc", ResourceManager::getResourceDirPathAbsolute(), 1, nullptr, ImGuiFileDialogFlags_Modal);

        if (ImGuiFileDialog::Instance()->Display("LoadSceneKey", 32, Utils::castVec2<ImVec2>(fileExplorerMinSize)))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string resPath = Utils::splitString(ImGuiFileDialog::Instance()->GetFilePathName(), ResourceManager::resDirName + ResourceManager::separator()).back();
                Globals::app->loadCurrentScene(ResourceManager::createResourcePath(resPath));
            }
            ImGuiFileDialog::Instance()->Close();
        }
    }

    void SceneEditor::drawSaveSceneButton()
    {
        static char scenePathBuf[INPUT_STRING_MAX_LENGTH];
        static bool openOverwritePopup = false;

        if (ImGui::Button("Save Scene"))
        {
            ImGui::OpenPopup("Save Scene");
            scenePathBuf[0] = '\0';
        }

        if (ImGui::BeginPopupModal("Save Scene", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowSize(Utils::castVec2<ImVec2>(popupMinSize));

            ImGui::Text("Path to save:");
            ImGui::InputText("##SceneNameInput", scenePathBuf, INPUT_STRING_MAX_LENGTH, ImGuiInputTextFlags_AutoSelectAll);

            if (ImGui::Button("Ok"))
            {
                if (ResourceManager::resourceExists(scenePathBuf))
                {
                    openOverwritePopup = true;
                }
                else
                {
                    Globals::app->saveCurrentScene(scenePathBuf, false);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (openOverwritePopup)
        {
            ImGui::OpenPopup("Overwrite warning");
            openOverwritePopup = false;
        }

        if (ImGui::BeginPopupModal("Overwrite warning", nullptr, ImGuiWindowFlags_NoResize))
        {
            ImGui::SetWindowSize(Utils::castVec2<ImVec2>(popupMinSize));

            ImGui::TextWrapped(std::string("File with the path '" + std::string(scenePathBuf) + "' already exists. Overwrite?").c_str());

            if (ImGui::Button("Yes"))
            {
                Globals::app->saveCurrentScene(scenePathBuf, true);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void SceneEditor::drawNodeTreeContextMenu(Node* node, bool hovered, bool cloningAllowed)
    {

        if (hovered && Input::isJustReleased(MouseButton::right))
            ImGui::OpenPopup(("Context Menu: " + std::to_string(node->getId())).c_str());

        if (ImGui::BeginPopup(("Context Menu: " + std::to_string(node->getId())).c_str()))
        {
            if (ImGui::MenuItem("Add Node"))
            {
                nodeToAddNewNodeTo = node;
            }
            else if (ImGui::MenuItem("Load Node"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("LoadNodeKey", "Load Node", ".psc", ResourceManager::getResourceDirPathAbsolute(), 1, nullptr, ImGuiFileDialogFlags_Modal);
                nodeToAddLoadedSceneTo = node;
            }
            else if (cloningAllowed && ImGui::MenuItem("Clone Node"))
            {
                Node* newNode = node->clone();
                newNode->setName(node->getName() + "c");
                newNode->orphanize();
                node->insertAfter(newNode);
            }
            else if (ImGui::MenuItem("Delete Node"))
            {
                node->orphanize();
                delete node;
                selectedNode = nullptr;
            }

            ImGui::EndPopup();
        }
    }

    void SceneEditor::drawCreateNodeMenu()
    {
        if (!nodeToAddNewNodeTo) return;
        static char nodeNameBuf[INPUT_STRING_MAX_LENGTH] = "NewNode";
        static std::string selectedNodeType;
        std::vector<std::string> nodeTypes = Node::getAllNodeTypes();
        ImGui::Begin("Create Node");
        ImGui::InputText("Name", nodeNameBuf, INPUT_STRING_MAX_LENGTH, ImGuiInputTextFlags_AutoSelectAll);
        for (const auto& type : nodeTypes)
        {
            if (ImGui::Selectable(type.c_str(), selectedNodeType == type))
                selectedNodeType = type;
        }
        if (ImGui::Button("Create") && !selectedNodeType.empty())
        {
            Node* newNode = Node::createNode(selectedNodeType);
            newNode->setName(nodeNameBuf);
            nodeToAddNewNodeTo->addChild(newNode);
            nodeToAddNewNodeTo = nullptr;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);
        if(ImGui::Button("Cancel"))
        {
            nodeToAddNewNodeTo = nullptr;
        }
        ImGui::End();
    }
    
    void SceneEditor::drawLoadNodeMenu()
    {
        if(!nodeToAddLoadedSceneTo) return;

        if (ImGuiFileDialog::Instance()->Display("LoadNodeKey", 32, Utils::castVec2<ImVec2>(fileExplorerMinSize)))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string resPath = Utils::splitString(ImGuiFileDialog::Instance()->GetFilePathName(), ResourceManager::resDirName + ResourceManager::separator()).back();
                Node* loadedScene = Globals::sceneManager->loadScene(resPath);
                std::vector<Node*> children = loadedScene->children;
                for(Node* child : children)
                    child->orphanize();
                nodeToAddLoadedSceneTo->addChildren(children);
            }
            ImGuiFileDialog::Instance()->Close();
        }
    }


    void SceneEditor::draw()
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

    void SceneEditor::print(std::string str)
    {
        printLines.push_back(std::move(str));
    }

    void SceneEditor::addDragFloat(const char* label, float* f, float speed)
    {
        dragFloats.push_back(DragFloat{ label, f, speed });
    }
}