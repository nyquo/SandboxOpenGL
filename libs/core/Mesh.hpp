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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(Shader& shader);

  private:
    unsigned int m_vao, m_vbo, m_ebo;
    void setupMesh();
};

}
