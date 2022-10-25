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
        shptr<Shader> shader;
        shptr<Texture> texture;

        MeshComposition(shptr<IndexBuffer> ib, shptr<Shader> shader)
            : ib(ib), shader(shader), texture(std::make_unique<Texture>()) {}

        MeshComposition(shptr<IndexBuffer> ib, shptr<Shader> shader, shptr<Texture> texture)
            : ib(ib), shader(shader), texture(texture) {}

        inline void bind() const { ib->bind(); shader->bind(); texture->bind(); }
        inline void unbind() const { ib->unbind(); shader->unbind(); texture->unbind(); }
        inline unsigned int getCount() const { return ib->getCount(); }
    };

}

#endif // __MESH_LAYOUT_HPP__