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
        unptr<IndexBuffer> ib;
        Shader* shader = nullptr;
        Texture* texture = nullptr;

        MeshComposition(unptr<IndexBuffer> ib, Shader* shader)
            : ib(std::move(ib)), shader(shader) {}

        MeshComposition(unptr<IndexBuffer> ib, Shader* shader, Texture* texture)
            : ib(std::move(ib)), shader(shader), texture(texture) {}

        inline void bind() const { ib->bind(); shader->bind(); texture->bind(); }
        inline void unbind() const { ib->unbind(); shader->unbind(); texture->unbind(); }
        inline unsigned int getCount() const { return ib->getCount(); }
    };

}

#endif // __MESH_LAYOUT_HPP__