#ifndef __MESH_LAYOUT_HPP__
#define __MESH_LAYOUT_HPP__

#include "index_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace prim
{

    class MeshComposition
    {
    private:
        IndexBuffer ib;
        Shader shader;
        Texture texture;
    public:
        MeshComposition(IndexBuffer&& ib, Shader&& shader, Texture&& texture)
            : ib(std::move(ib)), shader(std::move(shader)), texture(std::move(texture)) {}
        MeshComposition(MeshComposition&& other)
            : ib(std::move(other.ib)), shader(std::move(other.shader)), texture(std::move(other.texture)) {}
        MeshComposition& operator=(MeshComposition&& other)
        {
            this->~MeshComposition();
            ib = std::move(other.ib);
            shader = std::move(other.shader);
            texture = std::move(other.texture);
            return *this;
        }
        
        inline void bind() const { ib.bind(); shader.bind(); texture.bind(); }
        inline void unbind() const { ib.unbind(); shader.unbind(); texture.unbind(); }
        inline unsigned int getCount() const { return ib.getCount(); }
    };

}

#endif // __MESH_LAYOUT_HPP__