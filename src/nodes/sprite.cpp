#include "sprite.hpp"
#include "primitives.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"
#include "node_utils.hpp"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "globals.hpp"

namespace prim
{
    Sprite::Sprite(std::string name)
        : Node2D(name), planeMesh(Primitives::createSquareMesh(defaultSize)), width(defaultSize), height(defaultSize), relativeWidth(1.0f), relativeHeight(1.0f)
    {
    }

    Sprite::Sprite(std::string name, std::string imagePath)
        : Node2D(name), planeMesh(Primitives::createSquareMesh(defaultSize)), width(defaultSize), height(defaultSize), relativeWidth(1.0f), relativeHeight(1.0f)
    {
        image.load(imagePath);
        planeMesh.compositions[0].texture.load(image);
    }

    Sprite::Sprite(std::unordered_map<std::string, std::string>& fieldValues)
        : Node2D(fieldValues), planeMesh(Primitives::createSquareMesh(defaultSize)), width(defaultSize), height(defaultSize), relativeWidth(1.0f), relativeHeight(1.0f)
    {
        if (!fieldValues[NodeFields::imagePath].empty())
        {
            image.load(fieldValues[NodeFields::imagePath]);
            planeMesh.compositions[0].texture.load(image);
        }
        setWidth(std::stof(fieldValues[NodeFields::width]));
        setHeight(std::stof(fieldValues[NodeFields::height]));
        setZIndex(std::stof(fieldValues[NodeFields::zIndex]));
    }

    Sprite::~Sprite()
    {
    }

    void Sprite::start()
    {
        START_CHILDREN
    }

    void Sprite::update(float deltaTime)
    {
        UPDATE_CHILDREN
    }

    void Sprite::draw(Renderer& renderer)
    {
        DRAW_CHILDREN

        glm::vec2 globalPosition = getGlobalPosition();
        glm::vec2 globalScale = getGlobalScale();
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(globalPosition.x, globalPosition.y, zIndex));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, glm::vec3(globalScale.x * relativeWidth, globalScale.y * relativeHeight, 1.0f));
        modelMat = glm::translate(modelMat, -Utils::toVec3(getPivot() * defaultSize));

        renderer.setModelMat(std::move(modelMat));

        renderer.drawMesh(planeMesh, Globals::mainRenderer->selectShader);
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
        relativeWidth = width / defaultSize;
    }

    void Sprite::setHeight(float height)
    {
        this->height = height;
        relativeHeight = height / defaultSize;
    }

    void Sprite::setImage(std::string path)
    {
        image.load(path);
        planeMesh.compositions[0].texture.load(image);
    }

    void Sprite::setZIndex(float value)
    {
        zIndex = value;
    }

    std::string Sprite::serialize() const
    {
        std::stringstream ss;
        ss << Utils::createKeyValuePair(NodeFields::type, typeName) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::name, name) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::position, Utils::serializeVec2(getPosition())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::rotation, std::to_string(getRotation())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::scale, Utils::serializeVec2(getScale())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::pivot, Utils::serializeVec2(getPivot())) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::width, std::to_string(width)) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::height, std::to_string(height)) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::zIndex, std::to_string(zIndex)) << std::endl;
        ss << Utils::createKeyValuePair(NodeFields::imagePath, image.getFilePath()) << std::endl;
        ss << NodeFields::childrenStart << std::endl;
        for (Node* child : children)
            ss << child->serialize() << std::endl;
        ss << NodeFields::childrenEnd << std::endl;
        return ss.str();
    }

    void Sprite::renderFields()
    {
        Node2D::renderFields();

        static float widthBuffer;
        widthBuffer = width;
        if (ImGui::DragFloat("Width", &widthBuffer))
        {
            setWidth(widthBuffer);
        }
        static float heightBuffer;
        heightBuffer = height;
        if (ImGui::DragFloat("Height", &heightBuffer))
        {
            setHeight(heightBuffer);
        }
        ImGui::DragFloat("Z-Index", &zIndex, 0.01f);
        ImGui::LabelText("Image", image.getFilePath().c_str());
        ImGui::SameLine();

        if (ImGui::Button("...")) // change image button
            ImGuiFileDialog::Instance()->OpenDialog("ChooseImageKey", "Open Image", "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);

        if(ImGuiFileDialog::Instance()->Display("ChooseImageKey"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string newFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
                image.load(newFilePath);
                planeMesh.compositions.front().texture.load(image);
            }
            
            ImGuiFileDialog::Instance()->Close();
        }

    }

}