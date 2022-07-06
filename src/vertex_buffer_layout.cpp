#include "vertex_buffer_layout.hpp"
#include "renderer.hpp"

namespace prim
{
    void VertexBufferLayout::bind() const
    {
        unsigned int offset = 0;
        for (int i = 0; i < elements.size(); ++i)
        {
            const VertexBufferElement &element = elements[i];
            GL_CALL(glEnableVertexAttribArray(i));
            GL_CALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void *)offset));
            offset += element.count * VertexBufferElement::getSizeOfType(element.type);
        }
    }
}