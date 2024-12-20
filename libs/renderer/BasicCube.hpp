#pragma once

#include "Buffers.hpp"

#include <renderer/Shader.hpp>
#include <core/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <vector>
// TEMP TO DELETE
class BasicCube
{
  public:
    BasicCube(glm::vec3 position = glm::vec3(0.0F))
      : m_position(position)
      , m_vertexBuffer(sizeof(float) * m_vertices.size(), m_vertices.data())
      , m_indexBuffer(m_indices.size(), m_indices.data())
    {
        renderer::BufferLayout layout{renderer::BufferElement(GL_FLOAT, 3, GL_FALSE, sizeof(float))};
        m_vertexBuffer.setLayout(std::move(layout));
        m_vertexArray.addVertexBuffer(m_vertexBuffer);
        m_vertexArray.setIndexBuffer(m_indexBuffer);

        glBindVertexArray(0);

        m_shader = std::make_unique<renderer::Shader>(std::string(RESSOURCES_FOLDER) + "/shaders/LightCubeShader.vert",
                                                      std::string(RESSOURCES_FOLDER) + "/shaders/LightCubeShader.frag");
    }

    ~BasicCube() {}

    const float* getRawVertices() const { return m_vertices.data(); }

    size_t getVertexCount() const { return m_vertices.size(); }
    size_t getIndicesCount() const { return m_indices.size(); }

  private:
    std::vector<float> m_vertices{-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
                                  -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0};
    std::vector<unsigned int> m_indices{0, 2, 1, 0, 3, 2, 0, 7, 3, 0, 4, 7, 4, 5, 6, 4, 6, 7,
                                        7, 6, 2, 7, 2, 3, 5, 1, 2, 5, 2, 6, 0, 1, 5, 0, 5, 4};

  public:
    glm::mat4 m_modelMatrix{1.0F};
    glm::vec3 m_position;
    glm::vec3 m_rotationVector;

    renderer::VertexArray m_vertexArray;
    renderer::VertexBuffer m_vertexBuffer;
    renderer::IndexBuffer m_indexBuffer;

    std::unique_ptr<renderer::Shader> m_shader;
};
