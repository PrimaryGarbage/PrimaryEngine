#include "panel.hpp"
#include "graphics/primitives.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "resource_manager.hpp"
#include "ImGuiFileDialog.h"

namespace prim
{
    Panel::Panel(): Panel("NewPanel")
    {}
    
    Panel::Panel(std::string name): Control(name), backgroundMesh(Primitives::createSquareMesh(1.0f))
    {
        backgroundMesh.compositions.front().shader = Shader::getDefaultShader(DefaultShader::controlBackground);
    }
    
    void Panel::draw(Renderer& renderer)
    {
        NODE_DRAW
        (
            glm::vec2 globalPosition = getGlobalPosition();
            glm::vec2 globalSize = getSize();
            const glm::mat4& rendererViewMat = renderer.getViewMat();
            renderer.setViewMat(glm::mat4(1.0f));

            // render background
            glm::mat4 modelMat(1.0f);
            modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x, globalPosition.y, transform.zIndex));
            modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
            modelMat = glm::scale(modelMat, glm::vec3(globalSize.x, globalSize.y, 1.0f));
            backgroundMesh.compositions.front().shader->setUniform4f("u_color", backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
            backgroundMesh.compositions.front().shader->setUniform1f("u_borderRadius", borderRadius);
            backgroundMesh.compositions.front().shader->setUniform1i("u_sampleTexture", static_cast<int>(useTexture));
            renderer.setModelMat(std::move(modelMat));
            renderer.drawMesh(backgroundMesh);

            renderer.setViewMat(rendererViewMat);
        )
    }
    
    std::string Panel::serialize(bool withChildren) const 
    {
        std::stringstream ss;

        ss << Control::serialize(false);

        ss << Utils::createKeyValuePair(StateValues::backgroundColor, Utils::serializeVec4(backgroundColor)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::borderRadius, std::to_string(borderRadius)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::imagePath, imagePath);

        if(withChildren) ss << serializeChildren();

        return ss.str();
    }
    
    void Panel::restore(NodeValues& nodeValues)
    {
        Control::restore(nodeValues);

        backgroundColor = Utils::deserializeVec4(nodeValues[StateValues::backgroundColor]);
        borderRadius = std::stof(nodeValues[StateValues::borderRadius]);
        if(!nodeValues[StateValues::imagePath].empty())
        {
            imagePath = nodeValues[StateValues::imagePath];
            setTexture(imagePath);
        }
    }
    
    void Panel::renderFields(SceneEditor* sceneEditor)
    {
        Control::renderFields(sceneEditor);

        ImGui::ColorEdit4("Background Color", &backgroundColor.x);
        ImGui::DragFloat("BorderRadius", &borderRadius, 0.1f, 0.0f, 100.0f);

        ImGui::LabelText("Image", imagePath.c_str());
        if (ImGui::Button("...")) // change image button
        {
            ImGuiFileDialog::Instance()->OpenDialog("ChoosePanelImageKey", "Open Image", 
                "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}", ResourceManager::getResourceDirPathAbsolute(),
                1, nullptr, ImGuiFileDialogFlags_Modal);
        }

        ImGui::SameLine();
        if (ImGui::Button("Remove"))
        {
            removeTexture();
        }

        if(ImGuiFileDialog::Instance()->Display("ChoosePanelImageKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string path = Utils::splitString(ImGuiFileDialog::Instance()->GetFilePathName(), ResourceManager::resDirName + ResourceManager::separator()).back();
                setTexture(ResourceManager::createResourcePath(path));
            }
            
            ImGuiFileDialog::Instance()->Close();
        }
    }
    
    void Panel::setTexture(std::string path)
    {
        backgroundMesh.compositions.front().texture = Texture::create(ResourceManager::createResourcePath(path));
        useTexture = true;
    }
    
    void Panel::removeTexture()
    {
        useTexture = false;
    }
}