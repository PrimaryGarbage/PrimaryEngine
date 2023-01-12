#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "vertex_buffer.hpp"
#include "mesh_composition.hpp"
#include <vector>

namespace prim
{

class Mesh
{
public:
    unptr<VertexBuffer> vb;
    std::vector<MeshComposition> compositions;

    Mesh(unptr<VertexBuffer> vb);
    Mesh(const Mesh& other);
    void addComposition(MeshComposition&& composition);
};

}

#endif // __MESH_HPP__