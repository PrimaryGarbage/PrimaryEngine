#include "mesh.hpp"
#include "renderer.hpp"


namespace prim
{
    Mesh::Mesh(unptr<VertexBuffer> vb) : vb(std::move(vb))
    {
    }
    
    Mesh::Mesh(const Mesh& other) 
    {
        
    }
    
    void Mesh::addComposition(MeshComposition&& composition)
    {
        compositions.emplace_back(std::move(composition));
    }

}