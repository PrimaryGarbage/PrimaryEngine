#ifndef __DRAWABLE_HPP__
#define __DRAWABLE_HPP__

#include "node2d.hpp"

namespace prim
{
    class Shader;

    class Drawable : public Node2D
    {
    public:
        shptr<Shader> customShader;

        Drawable();
        Drawable(std::string name);
        virtual ~Drawable();

        virtual void draw(Renderer& renderer) override;
        void drawWithShader(Renderer& renderer, shptr<Shader>& shader);
    };
}


#endif // __DRAWABLE_HPP__