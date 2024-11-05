#pragma once

#include "Vertex.hpp"

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
    unsigned int m_vao{0}, m_vbo{0}, m_ebo{0};
    void setupMesh();
};

}
