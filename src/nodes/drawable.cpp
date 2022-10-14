#include "drawable.hpp"


namespace prim
{
    Drawable::Drawable(std::string name) : Node2D(std::move(name))
    {
    }
    
    Drawable::Drawable(std::unordered_map<std::string, std::string>& fieldValues) : Node2D(fieldValues)
    {
        
    }
    
    Drawable::~Drawable() 
    {
        
    }
    
    void Drawable::draw(Renderer& renderer)
    {
        DRAW_CHILDREN
    }
}