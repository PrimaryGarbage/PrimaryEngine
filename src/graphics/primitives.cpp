#include "primitives.hpp"
#include "mesh.hpp"

namespace prim
{
    const float sinPiOver4 = glm::sqrt(2.0f) / 2.0f;

    Mesh Primitives::createCubeMesh(float size)
    {
        const static float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        size, 0.0f, 0.0f, 1.0f, 0.0f,
        size, size, 0.0f, 1.0f, 1.0f,
        0.0f, size, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, size, 0.0f, 0.0f,
        size, 0.0f, size, 1.0f, 0.0f,
        size, size, size, 1.0f, 1.0f,
        0.0f, size, size, 0.0f, 1.0f
        };

        const static unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
            4, 0, 3,
            4, 3, 7,
            5, 6, 2,
            5, 2, 1,
            5, 4, 7,
            5, 7, 6,
            4, 5, 1,
            4, 1, 0,
            3, 2, 6,
            3, 6, 7
        };

        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);

        VertexBuffer vb(vertices, 40 * sizeof(float), layout);

        Shader shader("res/shaders/default.shader");
        shader.bind();

        shader.setUniform1i("u_texture", 0);

        IndexBuffer ib(indices, 36);

        Mesh mesh(std::move(vb));
        MeshComposition meshComposition(std::move(ib), std::move(shader));
        mesh.addComposition(std::move(meshComposition));

        return mesh;
    }

    Mesh Primitives::createRectangleMesh(float width, float height)
    {
        const static float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        width, 0.0f, 1.0f, 0.0f,
        width, height, 1.0f, 1.0f,
        0.0f, height, 0.0f, 1.0f
        };

        const static unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);

        VertexBuffer vb(vertices, 16 * sizeof(float), layout);

        Shader shader("res/shaders/default.shader");

        shader.setUniform1i("u_texture", 0);

        IndexBuffer ib(indices, 6);

        Mesh mesh(std::move(vb));
        MeshComposition meshComposition(std::move(ib), std::move(shader));
        mesh.addComposition(std::move(meshComposition));

        return mesh;
    }

    Mesh Primitives::createSquareMesh(float size)
    {
        const static float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f, -sinPiOver4, -sinPiOver4,
        size, 0.0f, 1.0f, 0.0f, sinPiOver4, -sinPiOver4,
        size, size, 1.0f, 1.0f, sinPiOver4, sinPiOver4,
        0.0f, size, 0.0f, 1.0f, -sinPiOver4, sinPiOver4
        };

        const static unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        layout.push<float>(2, true);

        VertexBuffer vb(vertices, 24 * sizeof(float), layout);

        Shader shader("res/shaders/default.shader");

        shader.setUniform1i("u_texture", 0);

        IndexBuffer ib(indices, 6);

        Mesh mesh(std::move(vb));
        MeshComposition meshComposition(std::move(ib), std::move(shader));
        mesh.addComposition(std::move(meshComposition));

        return mesh;
    }
    
    Mesh Primitives::createSquareMesh(std::string imagePath, float size)
    {
        Mesh mesh(createSquareMesh());
        mesh.compositions.front().texture.load(imagePath);
        return mesh;
    }
}