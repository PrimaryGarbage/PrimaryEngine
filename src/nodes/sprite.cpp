#include "sprite.hpp"
#include "graphics/primitives.hpp"
#include "graphics/renderer.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "utils.hpp"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "globals.hpp"
#include "node_utils.hpp"
#include "resource_manager.hpp"
#include <sstream>

namespace prim
{
    Sprite::Sprite(): Sprite(generateNodeName(this))
    {
    }

    Sprite::Sprite(std::string name)
        : Drawable(name), planeMesh(Primitives::createSquareMesh(defaultSize)), 
        width(defaultSize), height(defaultSize)
    {}

    Sprite::Sprite(std::string name, std::string imagePath)
        : Drawable(name), planeMesh(Primitives::createSquareMesh(defaultSize)), 
        width(defaultSize), height(defaultSize), imagePath(imagePath)
    {
        setTexture(this->imagePath);
    }

    void Sprite::start()
    {
        startChildren();
    }

    void Sprite::update(float deltaTime)
    {
        updateChildren(deltaTime);
    }

    void Sprite::draw(Renderer& renderer)
    {
        drawChildren(renderer);

        glm::vec2 globalPosition = getGlobalPosition();
        glm::vec2 globalScale = getGlobalScale();
        glm::mat4 modelMat(1.0f);
        planeMesh.compositions.front().shader->setUniform4f("u_color", tint);
        modelMat = glm::translate(modelMat, Utils::toVec3(globalPosition, zIndex));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, glm::vec3(globalScale.x * getRelativeWidth(), globalScale.y * getRelativeHeight(), 1.0f));
        modelMat = glm::translate(modelMat, -Utils::toVec3(getPivot() * defaultSize));

        renderer.setModelMat(std::move(modelMat));

        if(customShader)
            renderer.drawMesh(planeMesh, customShader);
        else
            renderer.drawMesh(planeMesh);
    }
    
    void Sprite::setCenterPivot()
    {
        setPivot(glm::vec2(0.5f, 0.5f));
    }

    void Sprite::setCornerPivot()
    {
        setPivot(glm::vec2(0.0f, 0.0f));
    }

    void Sprite::setSize(float width, float height)
    {
        setWidth(width);
        setHeight(height);
    }

    void Sprite::setWidth(float width)
    {
        this->width = width;
    }

    void Sprite::setHeight(float height)
    {
        this->height = height;
    }

    void Sprite::setTexture(std::string path)
    {
        planeMesh.compositions[0].texture = Texture::create(ResourceManager::createResourcePath(path));
    }

    void Sprite::setZIndex(float value)
    {
        zIndex = value;
    }
    
    float Sprite::getRelativeWidth() const
    {
        return width / defaultSize;
    }
    
    float Sprite::getRelativeHeight() const
    {
        return height / defaultSize;
    }

    std::string Sprite::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Node2D::serialize(false);

        ss << Utils::createKeyValuePair(StateValues::width, std::to_string(width)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::height, std::to_string(height)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::zIndex, std::to_string(zIndex)) << std::endl;
        ss << Utils::createKeyValuePair(StateValues::imagePath, imagePath) << std::endl;
        
        if(withChildren) ss << serializeChildren();
        
        return ss.str();
    }
    
    void Sprite::restore(NodeValues& nodeValues) 
    {
        Node2D::restore(nodeValues);

        imagePath = nodeValues[StateValues::imagePath];
        setWidth(std::stof(nodeValues[StateValues::width]));
        setHeight(std::stof(nodeValues[StateValues::height]));
        setZIndex(std::stof(nodeValues[StateValues::zIndex]));

        if (!nodeValues[StateValues::imagePath].empty())
            setTexture(imagePath);
    }

    void Sprite::renderFields(SceneEditor* sceneEditor)
    {
        Node2D::renderFields(sceneEditor);

        if(ImGui::DragFloat2("Pivot", &transform.pivot.x, 0.01f));
        if(ImGui::DragFloat("Width", &width));
        if (ImGui::DragFloat("Height", &height));
        ImGui::DragFloat("Z-Index", &zIndex, 0.01f);
        ImGui::LabelText("Image", imagePath.c_str());
        ImGui::SameLine();

        if (ImGui::Button("...")) // change image button
        {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseImageKey", "Open Image", 
                "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}", ResourceManager::getResourceDirPathAbsolute(),
                1, nullptr, ImGuiFileDialogFlags_Modal);
        }

        if(ImGuiFileDialog::Instance()->Display("ChooseImageKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string path = Utils::splitString(ImGuiFileDialog::Instance()->GetFilePathName(), ResourceManager::resDirName + ResourceManager::separator()).back();
                setTexture(ResourceManager::createResourcePath(path));
            }
            
            ImGuiFileDialog::Instance()->Close();
        }
    }
}