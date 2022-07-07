#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include <string>

namespace prim
{

    class Mesh;

    class Primitives
    {
    public:
        static Mesh createCubeMesh(float size = 100.0f);
        static Mesh createSquareMesh(float size = 100.0f);
        static Mesh createSquareMesh(std::string imagePath, float size = 100.0f);
    };


}

#endif // __PRIMITIVES_HPP__