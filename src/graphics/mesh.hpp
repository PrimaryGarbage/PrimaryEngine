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
    Shp<VertexBuffer> vb;
    std::vector<MeshComposition> compositions;

    Mesh() = default;
    Mesh(Shp<VertexBuffer> vb);
    void addComposition(MeshComposition&& composition);
};

}

#endif // __MESH_HPP__