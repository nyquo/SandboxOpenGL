#pragma once

#include "Vertex.hpp"

#include <Buffers.hpp>
#include <RendererExport.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

#include <vector>

namespace renderer {

class RENDERER_API Mesh
{
  public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh();

    void draw(Shader& shader) const;

  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

  private:
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
    VertexArray m_vertexArray;
};

}
