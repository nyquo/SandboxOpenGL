#pragma once

#include <RendererExport.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

#include <vector>

namespace renderer {

struct RENDERER_API Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

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
