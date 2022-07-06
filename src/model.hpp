#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "mesh.hpp"

namespace prim
{
    class Model
    {
    public:
        std::vector<Mesh> meshes;

        Model() = default;
        Model(Model&& other) : meshes(std::move(other.meshes)) {}
        Model& operator=(Model&& other) { meshes = std::move(other.meshes); return *this; }
    };
}

#endif // __MODEL_HPP__