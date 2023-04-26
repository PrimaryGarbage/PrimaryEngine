#include "camera_base.hpp"
#include "graphics/renderer.hpp"
#include "globals.hpp"

namespace prim
{
    CameraBase::CameraBase(std::string name) : Node2D(name), renderer(Globals::mainRenderer) 
    {
        setAsCurrent();
    }
    
    CameraBase::~CameraBase()
    {
        if(isCurrent()) renderer->setCurrentCamera(nullptr);
        Node::~Node();
    }
    
    void CameraBase::setAsCurrent()
    {
        renderer->setCurrentCamera(this);
    }
    
    bool CameraBase::isCurrent() const
    {
        return renderer->getCurrentCamera() == this;
    }

}