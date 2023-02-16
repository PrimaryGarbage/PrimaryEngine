#ifndef __MESH_LAYOUT_HPP__
#define __MESH_LAYOUT_HPP__

#include "index_buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "typedefs.hpp"

namespace prim
{

    class MeshComposition
    {
    public:
        shptr<IndexBuffer> ib;
        Shader* shader = nullptr;
        Texture* texture = nullptr;

        MeshComposition(shptr<IndexBuffer> ib, Shader* shader)
            : ib(ib), shader(shader) {}

        MeshComposition(shptr<IndexBuffer> ib, Shader* shader, Texture* texture)
            : ib(ib), shader(shader), texture(texture) {}

        inline void bind() const { ib->bind(); shader->bind(); texture->bind(); }
        inline void unbind() const { ib->unbind(); shader->unbind(); texture->unbind(); }
        inline unsigned int getCount() const { return ib->getCount(); }
    };

}

#endif // __MESH_LAYOUT_HPP__