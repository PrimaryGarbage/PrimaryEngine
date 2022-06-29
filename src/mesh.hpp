#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include <vector>

namespace prim
{

class Mesh
{
private:
    VertexBuffer vb;
    std::vector<IndexBuffer> ibs;
    VertexArray va;
public:
    Mesh(VertexArray&& vb, std::vector<IndexBuffer> ibs, VertexArray&& va)
};

}

#endif // __MESH_HPP__