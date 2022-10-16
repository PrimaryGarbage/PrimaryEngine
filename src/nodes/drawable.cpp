#include "drawable.hpp"


namespace prim
{
    Drawable::Drawable(std::string name) : Node2D(std::move(name))
    {
    }
    
    Drawable::Drawable(FieldValues& fieldValues) : Node2D(fieldValues)
    {
        
    }
    
    Drawable::~Drawable() 
    {
        
    }
    
    void Drawable::draw(Renderer& renderer)
    {
        drawChildren(renderer);
    }
    
    void Drawable::drawWithShader(Renderer& renderer, Shader* shader)
    {
        Shader* oldCustomShader = customShader;
        customShader = shader;
        this->draw(renderer);
        customShader = oldCustomShader;
    }
}