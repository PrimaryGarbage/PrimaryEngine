#include "mesh.hpp"
#include "renderer.hpp"


namespace prim
{
    Mesh::Mesh(Shp<VertexBuffer> vb) : vb(vb)
    {
    }
    
    void Mesh::addComposition(MeshComposition&& composition)
    {
        compositions.emplace_back(std::move(composition));
    }

}