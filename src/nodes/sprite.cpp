#include "sprite.hpp"
#include "primitives.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "image.hpp"
#include "utils.hpp"

namespace prim
{
    Sprite::Sprite(std::string name) : Node2D(name), planeMesh(Primitives::createSquareMesh(defaultSize))
    {
    }
    
    Sprite::Sprite(std::string name, std::string imagePath) : Node2D(name), planeMesh(Primitives::createSquareMesh(defaultSize))
    {
        planeMesh.compositions[0].texture.load(imagePath);
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

        glm::vec2 globalPos = getGlobalPosition() + getPivot();
        glm::vec2 globalScale = getGlobalScale();
        glm::vec3 pivot = Utils::toVec3(getPivot());
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, Utils::toVec3(globalPos));
        modelMat = glm::scale(modelMat, glm::vec3(globalScale.x, globalScale.y, 1.0f));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::translate(modelMat, -pivot);

        renderer.setModelMat(std::move(modelMat));

        renderer.drawMesh(planeMesh);
    }
    
    void Sprite::setCenterPivot()
    {
        setPivot(glm::vec2(1.0f, 1.0f) * defaultSize * 0.5f);
    }
}