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
    VertexBuffer vb;
    VertexArray va;
    std::vector<MeshComposition> compositions;

    Mesh(VertexBuffer&& vb);
    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);
    void addComposition(MeshComposition&& composition);
};

}

#endif // __MESH_HPP__