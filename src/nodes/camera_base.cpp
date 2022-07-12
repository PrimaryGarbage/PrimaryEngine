#include "camera_base.hpp"
#include "renderer.hpp"

namespace prim
{
    CameraBase::CameraBase(std::string name, Renderer* renderer) : Node2D(name), renderer(renderer) 
    {
        setAsCurrent();
    }
    
    CameraBase::~CameraBase()
    {
        renderer->setCurrentCamera(nullptr);
    }
    
    void CameraBase::setAsCurrent()
    {
        CameraBase* currentCamera = renderer->getCurrentCamera();
        if(currentCamera) currentCamera->current = false;
        renderer->setCurrentCamera(this);
        current = true;
    }
}