#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "vertex_buffer.hpp"
#include "vertex_array.hpp"
#include "mesh_composition.hpp"
#include <vector>

namespace prim
{

class Mesh
{
public:
    shptr<VertexBuffer> vb;
    shptr<VertexArray> va = std::make_shared<VertexArray>();
    std::vector<MeshComposition> compositions;

    Mesh(shptr<VertexBuffer> vb);
    void addComposition(MeshComposition&& composition);
};

}

#endif // __MESH_HPP__