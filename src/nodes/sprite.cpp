#include "sprite.hpp"
#include "primitives.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"
#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "globals.hpp"
#include "node_utils.hpp"
#include "resource_manager.hpp"

namespace prim
{
    Sprite::Sprite(): Sprite(generateNodeName(this))
    {
    }

    Sprite::Sprite(std::string name)
        : Drawable(name), planeMesh(Primitives::createSquareMesh(defaultSize)), 
        width(defaultSize), height(defaultSize), relativeWidth(1.0f), relativeHeight(1.0f),
        image(std::make_shared<Image>())
    {
    }

    Sprite::Sprite(std::string name, std::string imagePath)
        : Drawable(name), planeMesh(Primitives::createSquareMesh(defaultSize)), 
        width(defaultSize), height(defaultSize), relativeWidth(1.0f), relativeHeight(1.0f),
        image(std::make_shared<Image>())
    {
        image->load(imagePath);
        planeMesh.compositions[0].texture = Texture::create(*image);
    }

    Sprite::~Sprite()
    {
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
        modelMat = glm::translate(modelMat, Utils::toVec3(globalPosition, zIndex));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, glm::vec3(globalScale.x * relativeWidth, globalScale.y * relativeHeight, 1.0f));
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
        relativeWidth = width / defaultSize;
    }

    void Sprite::setHeight(float height)
    {
        this->height = height;
        relativeHeight = height / defaultSize;
    }

    void Sprite::setImage(std::string path)
    {
        image->load(path);
        planeMesh.compositions[0].texture = Texture::create(*image);
    }

    void Sprite::setZIndex(float value)
    {
        zIndex = value;
    }

    std::string Sprite::serialize(bool withChildren) const
    {
        std::stringstream ss;

        ss << Node2D::serialize(false);

        ss << Utils::createKeyValuePair(StateFields::width, std::to_string(width)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::height, std::to_string(height)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::zIndex, std::to_string(zIndex)) << std::endl;
        ss << Utils::createKeyValuePair(StateFields::imagePath, image->getFilePath()) << std::endl;
        
        if(withChildren) ss << serializeChildren();
        
        return ss.str();
    }
    
    void Sprite::deserialize(FieldValues& fieldValues) 
    {
        Node2D::deserialize(fieldValues);

        if (!fieldValues[StateFields::imagePath].empty())
        {
            image->load(fieldValues[StateFields::imagePath]);
            planeMesh.compositions[0].texture = Texture::create(fieldValues[StateFields::imagePath]);
        }
        setWidth(std::stof(fieldValues[StateFields::width]));
        setHeight(std::stof(fieldValues[StateFields::height]));
        setZIndex(std::stof(fieldValues[StateFields::zIndex]));
    }

    void Sprite::renderFields()
    {
        Node2D::renderFields();

        static float pivotBuffer[2];
        static float widthBuffer;

        pivotBuffer[0] = transform.pivot.x;
        pivotBuffer[1] = transform.pivot.y;
        widthBuffer = width;

        if(ImGui::DragFloat2("Pivot", pivotBuffer))
        {
            setPivot(glm::vec2(pivotBuffer[0], pivotBuffer[1]));
        }
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
        ImGui::LabelText("Image", image->getFilePath().c_str());
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
                std::string path = Utils::splitString(ImGuiFileDialog::Instance()->GetFilePathName(), ResourceManager::resDirName + "/").back();
                setImage(ResourceManager::createResourcePath(path));
            }
            
            ImGuiFileDialog::Instance()->Close();
        }

        if(cloneBound)
        {
            if(ImGui::Button("Unbind"))
            {
                unbind();
            }
        }
    }
    
    void Sprite::unbind() 
    {
        Node2D::unbind();

        std::string oldImagePath = image->empty() ? "" : image->getFilePath();
        image = std::make_shared<Image>();
        planeMesh = Primitives::createSquareMesh(defaultSize);

        if(!oldImagePath.empty())
            setImage(oldImagePath);
    }

}