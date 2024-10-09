#pragma once

#include <Shader.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

#include <vector>

namespace core {

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
  public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) = delete;
    Mesh operator=(const Mesh& other) = delete;
    Mesh operator=(Mesh&& other) = delete;
    ~Mesh();

    void draw(Shader& shader);

  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

  private:
    unsigned int m_vao, m_vbo, m_ebo;
    void setupMesh();
};

}
