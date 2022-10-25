#include "mesh.hpp"
#include "renderer.hpp"


namespace prim
{
    Mesh::Mesh(shptr<VertexBuffer> vb) : vb(vb)
    {
        va->addBuffer(*vb);
    }
    
    void Mesh::addComposition(MeshComposition&& composition)
    {
        compositions.emplace_back(std::move(composition));
    }

}