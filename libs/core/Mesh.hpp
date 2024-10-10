#pragma once

#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

#include <vector>

namespace core {

struct CORE_API Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class CORE_API Mesh
{
  public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<Texture>&& textures);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) noexcept;
    ~Mesh();

    void draw(Shader& shader);

  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

  private:
    unsigned int m_vao{0}, m_vbo{0}, m_ebo{0};
    void setupMesh();
};

}
