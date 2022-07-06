#include "mesh.hpp"
#include "renderer.hpp"


namespace prim
{
    Mesh::Mesh(VertexBuffer&& vb) : vb(std::move(vb))
    {
        va.addBuffer(this->vb);
    }
    
    Mesh::Mesh(Mesh&& other)
        : vb(std::move(other.vb)), va(std::move(other.va)),
        compositions(std::move(other.compositions)) 
    {}
    
    Mesh& Mesh::operator=(Mesh&& other)
    {
        vb = std::move(other.vb);
        va = std::move(other.va);
        compositions = std::move(other.compositions);

        return *this;
    }
    
    void Mesh::addComposition(MeshComposition&& composition)
    {
        compositions.push_back(std::move(composition));
    }

}