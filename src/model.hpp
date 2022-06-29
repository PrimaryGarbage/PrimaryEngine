#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"

namespace prim
{

class Model
{
private:
    VertexBuffer vb;
    VertexArray va;
    std::vector<IndexBuffer> ibs;
    Shader shader;

public:
    void 
}

}

#endif // __MODEL_HPP__