#include "sprite.hpp"
#include "primitives.hpp"
#include "renderer.hpp"
#include "gtc/matrix_transform.hpp"
#include "image.hpp"

namespace prim
{
    Sprite::Sprite(std::string name) : Node2D(name), planeMesh(Primitives::createSquareMesh())
    {
    }
    
    Sprite::Sprite(std::string name, std::string imagePath) : Node2D(name), planeMesh(Primitives::createSquareMesh())
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

        glm::vec2 globalPos = getGlobalPosition();
        glm::vec2 globalScale = getGlobalScale();
        glm::mat4 modelMat(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(globalPos.x, globalPos.y, 0.0f));
        modelMat = glm::rotate(modelMat, getGlobalRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, glm::vec3(globalScale.x, globalScale.y, 1.0f));

        renderer.setModelMat(std::move(modelMat));

        renderer.drawMesh(planeMesh);
    }
}