#include "sprite.hpp"
#include "primitives.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "utils.hpp"

namespace prim
{
    Sprite::Sprite(std::string name) : Node2D(name), planeMesh(Primitives::createSquareMesh(defaultSize)), width(defaultSize), height(defaultSize)
    {
    }
    
    Sprite::Sprite(std::string name, std::string imagePath) : Node2D(name), planeMesh(Primitives::createSquareMesh(defaultSize)), width(defaultSize), height(defaultSize)
    {
        image.load(imagePath);
        planeMesh.compositions[0].texture.load(image);
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

        glm::vec2 globalScale = getGlobalScale();
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, Utils::toVec3(getGlobalPosition()));
        modelMat = glm::scale(modelMat, glm::vec3(globalScale.x, globalScale.y, 1.0f));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::translate(modelMat, -Utils::toVec3(getPivot()));

        renderer.setModelMat(std::move(modelMat));

        renderer.drawMesh(planeMesh);
    }
    
    void Sprite::setCenterPivot()
    {
        setPivot(glm::vec2(width, height) * 0.5f);
    }
    
    void Sprite::setSize(float width, float height)
    {
        this->width = width;
        this->height = height;
        planeMesh = Primitives::createRectangleMesh(width, height);
        planeMesh.compositions[0].texture.load(image);
    }
    
    void Sprite::setWidth(float width)
    {
        width = width;
        planeMesh = Primitives::createRectangleMesh(width, height);
        planeMesh.compositions[0].texture.load(image);
    }
    
    void Sprite::setHeight(float height)
    {
        height = height;
        planeMesh = Primitives::createRectangleMesh(width, height);
        planeMesh.compositions[0].texture.load(image);
    }
    
    void Sprite::setImage(std::string path)
    {
        image.load(path);
        planeMesh.compositions[0].texture.load(image);
    }
}