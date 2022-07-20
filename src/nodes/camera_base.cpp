#include "camera_base.hpp"
#include "renderer.hpp"
#include "globals.hpp"

namespace prim
{
    CameraBase::CameraBase(std::string name) : Node2D(name), renderer(Globals::mainRenderer) 
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