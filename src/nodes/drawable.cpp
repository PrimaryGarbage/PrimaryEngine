#include "drawable.hpp"
#include "node_utils.hpp"


namespace prim
{
    Drawable::Drawable(): Node2D(generateNodeName(this))
    {
    }

    Drawable::Drawable(std::string name) : Node2D(std::move(name))
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