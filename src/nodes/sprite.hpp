#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__

#include "node2d.hpp"
#include "mesh.hpp"

namespace prim
{

    enum class ImageType;

    class Sprite : public Node2D
    {
    private:
        Mesh planeMesh;
    public:
        Sprite(std::string name);
        Sprite(std::string name, std::string imagePath);
        virtual ~Sprite();

        virtual void start() override;
        virtual void update(float deltaTime) override;
        virtual void draw(Renderer& renderer) override;
    };

}

#endif // __SPRITE_HPP__