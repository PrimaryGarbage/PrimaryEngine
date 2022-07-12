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
    {
        std::string lel;
    }
    
    Mesh& Mesh::operator=(Mesh&& other)
    {
        vb = std::move(other.vb);
        va = std::move(other.va);
        compositions.clear();
        for(MeshComposition& composition : other.compositions)
            compositions.emplace_back(std::move(composition));

        return *this;
    }
    
    void Mesh::addComposition(MeshComposition&& composition)
    {
        compositions.emplace_back(std::move(composition));
    }

}