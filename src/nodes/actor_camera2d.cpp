#include "actor_camera2d.hpp"
#include "gtx/rotate_vector.hpp"
#include "utils.hpp"

namespace prim
{
    ActorCamera2D::ActorCamera2D(std::string name, Renderer* renderer, Node2D* target)
        : ActorCamera2D(name, renderer, -1.0f, 1.0f, target)
    {
    }
    
    ActorCamera2D::ActorCamera2D(std::string name, Renderer* renderer, float zNear, float zFar, Node2D* target)
        : Camera2D(name, renderer, zNear, zFar), target(target)
    {
    }
    
    ActorCamera2D::~ActorCamera2D()
    {
    }
    
    void ActorCamera2D::start()
    {
        START_CHILDREN

        initialOffset = getGlobalPosition() - target->getGlobalPosition();
    }
    
    void ActorCamera2D::update(float deltaTime)
    {
        UPDATE_CHILDREN

        glm::vec2 position;

        if(rotateWithTarget)
        {
            float targetAngle = target->getGlobalRotation();
            position = glm::mix(getGlobalPosition(), target->getGlobalPosition() + glm::rotate(initialOffset, targetAngle), smoothness);
            setGlobalRotation(Utils::lerpAngle(getGlobalRotation(), targetAngle, smoothness));
        }
        else
            position = glm::mix(getGlobalPosition(), target->getGlobalPosition() + initialOffset, smoothness);

        setGlobalPosition(position);
    }
    
    void ActorCamera2D::draw(Renderer& renderer)
    {
        DRAW_CHILDREN   
    }
    
    void ActorCamera2D::setTarget(Node2D* target)
    {
        
    }
}