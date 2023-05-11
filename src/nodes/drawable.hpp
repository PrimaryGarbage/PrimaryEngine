#ifndef __DRAWABLE_HPP__
#define __DRAWABLE_HPP__

#include "node2d.hpp"

namespace prim
{
    class Shader;

    class Drawable : public Node2D
    {
    public:
        Shader* customShader = nullptr;

        Drawable();
        Drawable(std::string name);

        void drawWithShader(Renderer& renderer, Shader* shader);
    };
}


#endif // __DRAWABLE_HPP__